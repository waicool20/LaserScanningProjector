#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <drivers/Motor.h>

namespace {
void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOE);

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO10);
}
}  // namespace

int main() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  Motor motorX{
      PortPin{GPIOB, GPIO7},
      PortPin{GPIOB, GPIO6},
      PortPin{GPIOB, GPIO5}
  };
  Motor motorY{
      PortPin{GPIOB, GPIO4},
      PortPin{GPIOB, GPIO3},
      PortPin{GPIOA, GPIO15}
  };
  gpio_setup();

  gpio_set(GPIOA, GPIO9);

  while (true) {
    gpio_toggle(GPIOE, GPIO9 | GPIO10);  // LED on/off
    for (std::size_t i = 0; i < 2'000'000; i++) {
      __asm__("nop");
    }
  }
}
