#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

namespace {
void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOE);

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO10);
}
}  // namespace

int main() {
  gpio_setup();

  gpio_set(GPIOA, GPIO9);

  while (true) {
    gpio_toggle(GPIOE, GPIO9 | GPIO10);  // LED on/off
    for (std::size_t i = 0; i < 2'000'000; i++) {
      __asm__("nop");
    }
  }
}
