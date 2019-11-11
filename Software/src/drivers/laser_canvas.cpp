#include "laser_canvas.h"
#include <lib/systick.h>

laser_canvas::laser_canvas(uint32_t spr, uint32_t width, uint32_t height, laser laser, stepper_motor x_motor,
                           stepper_motor y_motor, gpio ldr)
    : _spr(spr), _width(width), _height(height), _laser(laser), _x_motor(x_motor), _y_motor(yM), _ldr(ldr) {
  ldr.setup(GPIO_MODE_INPUT, GPIO_PUPD_NONE);
  home();
}

bool laser_canvas::home() {
  _laser.enable();
  _x_motor.set_dir(stepper_motor::cw);
  _y_motor.set_dir(stepper_motor::cw);
  uint32_t steps = (_spr / 360) / 2;

  uint8_t stages = 4;
  for (uint8_t stage = 1; stage <= stages; ++stage) {
    for (uint32_t y = 0; y < _spr; y += steps) {
      for (uint32_t x = 0; x < _spr; ++x) {
        _x_motor.do_steps(1);
        uint8_t sleep = stage - 1;
        systick::sleep_us(sleep * sleep * 10);
        if (_ldr.get()) {
          _x_motor.do_steps(_spr / 360);
          if (stage == stages) {
            _x_motor.do_steps(_spr / 360);
            current_x = 0;
            current_y = 0;
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
    _y_motor.do_steps(round(1.25 * steps));
    _y_motor.toggle_dir();
    steps /= 2;
  }
  return false;
}

uint32_t laser_canvas::get_width() const {
  return _width;
}

uint32_t laser_canvas::get_height() const {
  return _height;
}

uint32_t laser_canvas::get_current_x() const {
  return current_x;
}

uint32_t laser_canvas::get_current_y() const {
  return current_y;
}
