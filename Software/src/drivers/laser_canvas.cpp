#include "laser_canvas.h"
#include <lib/systick.h>

laser_canvas::laser_canvas(uint32_t spr, uint32_t width, uint32_t height, laser l, stepper_motor x_motor,
                           stepper_motor y_motor, gpio home_ldr)
    : spr(spr), width(width), height(height), main_laser(l), x_motor(x_motor), y_motor(y_motor), home_ldr(home_ldr) {
  home_ldr.setup(GPIO_MODE_INPUT, GPIO_PUPD_NONE);
  home();
}

bool laser_canvas::home() {
  main_laser.enable();
  x_motor.set_dir(stepper_motor::cw);
  y_motor.set_dir(stepper_motor::cw);
  uint32_t steps = (spr / 360) / 2;

  uint8_t stages = 4;
  for (uint8_t stage = 1; stage <= stages; ++stage) {
    for (uint32_t y = 0; y < spr; y += steps) {
      for (uint32_t x = 0; x < spr; ++x) {
        x_motor.do_steps(1);
        uint8_t sleep = stage - 1;
        systick::sleep_us(sleep * sleep * 10);
        if (home_ldr.get()) {
          x_motor.do_steps(spr / 360);
          if (stage == stages) {
            x_motor.do_steps(spr / 360);
            current_x = 0;
            current_y = 0;
            return true;
          } else {
            goto next_stage;
          }
        }
      }
      y_motor.do_steps(steps);
    }
    next_stage:
    y_motor.toggle_dir();
    y_motor.do_steps(round(1.25 * steps));
    y_motor.toggle_dir();
    steps /= 2;
  }
  return false;
}
