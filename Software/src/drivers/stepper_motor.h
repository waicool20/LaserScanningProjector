#pragma once

#include <cstdint>

#include "lib/gpio.h"

class stepper_motor {
 public:
  enum direction {
    cw, ccw
  };

  stepper_motor(gpio en, gpio dir, gpio step);

  void enable();
  void disable();
  void toggle();
  void set_dir(direction dir);
  void toggle_dir();
  void do_steps(std::uint32_t steps);

 private:
  gpio _en_;
  gpio _dir_;
  gpio _step_;
};
