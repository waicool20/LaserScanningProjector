#include "laser_canvas.h"
#include <lib/systick.h>

using namespace std::chrono_literals;

laser_canvas::laser_canvas(std::uint32_t spr,
                           std::uint32_t width,
                           std::uint32_t height,
                           laser laser,
                           stepper_motor x_motor,
                           stepper_motor y_motor,
                           gpio ldr)
    : _spr(spr), _width(width), _height(height), _laser(laser), _x_motor(x_motor), _y_motor(y_motor), _ldr(ldr) {
  ldr.setup(GPIO_MODE_INPUT, GPIO_PUPD_NONE);
}

bool laser_canvas::home() {
  _laser.enable();
  _x_motor.set_dir(stepper_motor::cw);
  _y_motor.set_dir(stepper_motor::cw);
  std::uint32_t steps = (_spr / 360) / 2;

  std::uint8_t stages = 4;
  for (std::uint8_t stage = 1; stage <= stages; ++stage) {
    for (std::uint32_t y = 0; y < _spr; y += steps) {
      for (std::uint32_t x = 0; x < _spr; ++x) {
        _x_motor.do_steps(1);
        std::uint8_t sleep = stage - 1;
        systick::sleep_us(std::uint64_t(sleep * sleep * 10));
        if (_ldr.get()) {
          _x_motor.do_steps(_spr / 360);
          if (stage == stages) {
            _x_motor.do_steps(_spr / 70);
            _y_motor.set_dir(stepper_motor::ccw);
            _y_motor.do_steps(_spr / 90);
            current_x = 0;
            current_y = 0;
            goto_xy(_width / 2, _height / 2);
            return true;
          } else {
            goto next_stage;
          }
        }
      }
      _y_motor.do_steps(steps);
    }
    next_stage:
    _y_motor.toggle_dir();
    _y_motor.do_steps(std::round(1.25 * steps));
    _y_motor.toggle_dir();
    steps /= 2;
  }
  return false;
}

void laser_canvas::goto_xy(std::uint32_t x, std::uint32_t y) {
  if (x >= _width || y >= _height) { return; }
  auto dx = std::int32_t(x - current_x);
  auto dy = std::int32_t(y - current_y);
  _x_motor.set_dir(dx > 0 ? stepper_motor::cw : stepper_motor::ccw);
  _y_motor.set_dir(dy > 0 ? stepper_motor::ccw : stepper_motor::cw);
  _x_motor.do_steps(std::uint32_t(std::abs(dx)));
  _y_motor.do_steps(std::uint32_t(std::abs(dy)));
  current_x = x;
  current_y = y;
}

void laser_canvas::highlight_canvas_area() {
  _laser.disable();
  goto_xy(0, 0);
  _laser.enable();
  systick::sleep(10ms);
  goto_xy(0, _height - 1);
  systick::sleep(10ms);
  goto_xy(_width - 1, _height - 1);
  systick::sleep(10ms);
  goto_xy(_width - 1, 0);
  systick::sleep(10ms);
  goto_xy(0, 0);
  systick::sleep(10ms);
  goto_xy(_width - 1, _height - 1);
  systick::sleep(10ms);
  _laser.disable();
  goto_xy(_width - 1, 0);
  _laser.enable();
  systick::sleep(10ms);
  goto_xy(0, _height - 1);
  systick::sleep(10ms);
}

void laser_canvas::draw_frame() {
  if (frame_data == nullptr) return;

  static bool y_top_down = true;
  static bool x_left_right = true;

  std::uint32_t cols = _width / 8;

  bool prev_state = false;
  for (uint32_t y = 0; y < _height; y++) {
    _y_motor.do_steps(1);
    for (uint32_t x = 0; x < _width; x++) {
      _x_motor.do_steps(2);

      systick::sleep(500us);

      std::uint32_t actual_y = y_top_down ? y : _height - y;
      std::uint32_t actual_x = x_left_right ? x : _width - x;

      std::uint32_t x_byte = actual_x / 8;
      std::uint32_t x_bit = actual_x % 8;

      bool laser_state = (actual_y < _height / 2 && actual_x < _width / 2) || (actual_y >= _height / 2 && actual_x >= _width / 2);
      if (prev_state != laser_state) {
        prev_state = laser_state;
        laser_state ? _laser.enable() : _laser.disable();
      }
    }

    _x_motor.toggle_dir();
    x_left_right = !x_left_right;
  }

  _y_motor.toggle_dir();
  y_top_down = !y_top_down;
}

void laser_canvas::draw_tuples() {
  struct tuple {
    std::uint32_t x;
    std::uint32_t y;
    bool state;
  };

  static bool y_top_down = true;
  static bool x_left_right = true;
  for (std::uint32_t y = 0; y < _height; ++y) {
    std::uint32_t actual_y = y_top_down ? y : _height - y;

    std::uint32_t x_begin = actual_y >= _height / 2 ? _width / 2 : 0;
    std::uint32_t x_end = actual_y >= _height / 2 ? _width - 1 : _width / 2;

    if (!x_left_right) {
      std::swap(x_begin, x_end);
    }

    goto_xy(x_begin, actual_y);
    systick::sleep(500us);
    _laser.enable();
    goto_xy(x_end, actual_y);
    systick::sleep(7500us);
    _laser.disable();

    x_left_right = !x_left_right;
  }

  y_top_down = !y_top_down;
}

std::uint32_t laser_canvas::get_width() const {
  return _width;
}

std::uint32_t laser_canvas::get_height() const {
  return _height;
}

std::uint32_t laser_canvas::get_current_x() const {
  return current_x;
}

std::uint32_t laser_canvas::get_current_y() const {
  return current_y;
}
