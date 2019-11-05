#include "gpio.h"

void gpio::setup(std::uint8_t mode, std::uint8_t pupd) {
  gpio_mode_setup(port, mode, pupd, pin);
}

void gpio::output_opts(std::uint8_t otype, std::uint8_t speed) {
  gpio_set_output_options(port, otype, speed, pin);
}

void gpio::set_af(uint8_t af_num) {
  gpio_set_af(port, af_num, pin);
}

void gpio::state(bool new_state) {
  if (new_state) {
    gpio_set(port, pin);
  } else {
    gpio_clear(port, pin);
  }
}

void gpio::toggle() {
  gpio_toggle(port, pin);
}

bool gpio::get() {
  return gpio_get(port, pin) != 0;
}

volatile gpio::mmio* gpio::handle() {
  return reinterpret_cast<mmio*>(port);
}
