#include "laser_canvas.h"
#include <cmath>
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
            goto_xy(_width / 2, _height / 2, false);
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

void laser_canvas::goto_xy(std::uint32_t x, std::uint32_t y, bool no_delay = false) {
  if (x >= _width || y >= _height) { return; }

  static auto max_dist = std::sqrt(float(_width * _width + _height * _height));

  const auto dx = std::int32_t(x - current_x);
  const auto dy = std::int32_t(y - current_y);
  _x_motor.set_dir(dx > 0 ? stepper_motor::cw : stepper_motor::ccw);
  _y_motor.set_dir(dy > 0 ? stepper_motor::ccw : stepper_motor::cw);

  {
    const auto total_steps = std::max(std::abs(dx), std::abs(dy));
    const auto step_dx = std::abs(float(dx) / total_steps);
    const auto step_dy = std::abs(float(dy) / total_steps);
    auto x_accu = 0.0f, y_accu = 0.0f;
    for (std::int32_t i = 0; i < total_steps; ++i) {
      const auto x_next = x_accu + step_dx;
      const auto y_next = y_accu + step_dy;

      if (std::round(x_next) > std::round(x_accu)) {
        _x_motor.do_steps(1);
      }
      if (std::round(y_next) > std::round(y_accu)) {
        _y_motor.do_steps(1);
      }

      x_accu = x_next;
      y_accu = y_next;
    }
  }

  current_x = x;
  current_y = y;

  if (!no_delay) {
    const auto dist = std::sqrt(float(dx * dx + dy * dy));
    systick::sleep(6ms + (2ms * dist / max_dist));
  }
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

std::pair<std::size_t, std::uint8_t> laser_canvas::index_conversion(std::uint32_t x, std::uint32_t y) const {
  std::uint32_t x_byte = x / 8;
  std::uint32_t x_bit = x % 8;

  return std::make_pair(y * (_width / 8) + x_byte, 7 - x_bit);
}

void laser_canvas::draw_frame(const std::uint8_t* frame_data) {
  if (frame_data == nullptr) { return; }

  static bool y_top_down = true;
  static bool x_left_right = true;

  _laser.disable();
  bool prev_state = false;

  for (uint32_t y = 0; y < _height; y++) {
    _y_motor.do_steps(1);
    for (uint32_t x = 0; x < _width; x++) {
      _x_motor.do_steps(2);

      systick::sleep(500us);

      std::uint32_t actual_y = y_top_down ? y : _height - y - 1;
      std::uint32_t actual_x = x_left_right ? x : _width - x - 1;

      const auto&[byte, bit] = index_conversion(actual_x, actual_y);

      bool laser_state = (frame_data[byte] >> (bit)) & 0x1;
      laser_state = !laser_state;
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

void laser_canvas::draw_tuples(const tuple* tuples, std::size_t size) {
  for (std::uint32_t i = 0; i < size; ++i) {
    goto_xy(tuples[i].x, tuples[i].y);
    if (tuples[i].state) {
      _laser.enable();
    } else {
      _laser.disable();
    }
  }
}

void laser_canvas::draw_magnitude_y(float magnitude) {
  _laser.enable();
  uint32_t new_x = current_x + _width / 64;
  if (new_x >= _width) {
    _laser.disable();
    new_x = 0;
  }
  goto_xy(new_x, roundf((_height / 2) + magnitude * (_height / 2)), true);
  if (new_x == 0) { _laser.enable(); }
}

void laser_canvas::resize(uint32_t w, uint32_t h) {
  if (_width == w && _height == h) return;
  current_x += (w - _width) / 2; // Remap width
  _width = w;
  current_y += (h - _height) / 2; // Remap height
  _height = h;
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
