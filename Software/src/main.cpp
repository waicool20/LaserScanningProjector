#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

//#include "drivers/Motor.h"
#include "drivers/stepper_motor.h"
#include "lib/rcc.h"
#include "lib/systick.h"

namespace {
void rcc_setup() {
  rcc::periph_clock_enable(RCC_GPIOA);
  rcc::periph_clock_enable(RCC_GPIOB);
  rcc::periph_clock_enable(RCC_GPIOE);
}
}  // namespace

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick::init();

  rcc_setup();

  stepper_motor motor_x(
      gpio(GPIOB, GPIO7),
      gpio(GPIOB, GPIO6),
      gpio(GPIOB, GPIO5));
  stepper_motor motor_y(
      gpio(GPIOB, GPIO4),
      gpio(GPIOB, GPIO3),
      gpio(GPIOA, GPIO15));

  /*Motor motorX{
      PortPin{GPIOB, GPIO7},
      PortPin{GPIOB, GPIO6},
      PortPin{GPIOB, GPIO5}
  };
  Motor motorY{
      PortPin{GPIOB, GPIO4},
      PortPin{GPIOB, GPIO3},
      PortPin{GPIOA, GPIO15}
  };*/

  gpio led1(GPIOA, GPIO9);
  gpio led2(GPIOA, GPIO10);

  led1.state(true);

  while (true) {
    led1.toggle();
    led2.toggle();

    for (std::size_t i = 0; i < 2'000'000; i++) {
      __asm__("nop");
    }
  }
}
