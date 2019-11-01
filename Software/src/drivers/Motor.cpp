#include "Motor.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

Motor::Motor(PortPin en, PortPin dir, PortPin step)
    : en(en), dir(dir), step(step) {
  gpio_mode_setup(en.port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, en.pin);
  gpio_mode_setup(dir.port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, dir.pin);
  gpio_mode_setup(step.port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, step.pin);

  gpio_set_output_options(en.port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, en.pin);
  gpio_set_output_options(dir.port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, dir.pin);
  gpio_set_output_options(step.port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, step.pin);
}

void Motor::enable() {
  gpio_clear(en.port, en.pin);
  // TODO delay 5us
}

void Motor::disable() {
  gpio_set(en.port, en.pin);
  // TODO delay 5us
}

void Motor::toggle() {
  gpio_toggle(en.port, en.pin);
  // TODO delay 5us
}

void Motor::setDirection(Direction direction) {
  if (direction == Direction::CW) {
    gpio_set(dir.port, dir.pin);
    // TODO delay 2us
  } else {
    gpio_clear(dir.port, dir.pin);
    // TODO delay 2us
  }
}

void Motor::toggleDirection() {
  gpio_toggle(dir.port, dir.pin);
  // TODO delay 2us
}

void Motor::doSteps(uint32_t steps) {
  gpio_set(step.port, step.pin);
  // TODO delay 2us
  gpio_clear(step.port, step.pin);
}
