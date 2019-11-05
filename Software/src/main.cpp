#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <drivers/st7735s.h>

#include "drivers/stepper_motor.h"
#include "lib/rcc.h"
#include "lib/systick.h"

namespace {
}  // namespace

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick::init();

  st7735s lcd{2, 1, 128, 160, COLOR_MODE_18_BITS};
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio up = gpio(GPIOC, GPIO15);
  gpio down = gpio(GPIOC, GPIO13);
  gpio left = gpio(GPIOC, GPIO14);
  gpio right = gpio(GPIOB, GPIO8);


  up.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  down.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  left.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  right.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  lcd.draw_rect(1, 1, 32, 32, 0x00979D);
  lcd.draw_rect(1, lcd.get_height() - 33, 32, 32, 0x00979D);
  lcd.draw_rect(lcd.get_width() - 33, 1, 32, 32, 0x00979D);
  lcd.draw_rect(lcd.get_width() - 33, lcd.get_height() - 33, 32, 32, 0x00979D);
  uint8_t x = 0;
  uint8_t y = 0;
  while (true) {
    if (up.get()) {
      y -= 8;
      lcd.draw_screen(0xFFFFFF);
    }
    if (down.get()) {
      y += 8;
      lcd.draw_screen(0xFFFFFF);
    }
    if (left.get()) {
      x -= 8;
      lcd.draw_screen(0xFFFFFF);
    }
    if (right.get()) {
      x += 8;
      lcd.draw_screen(0xFFFFFF);
    }
    lcd.draw_rect(lcd.get_width() / 2 + x, lcd.get_height() / 2 + y, 8, 8, 0x00);
  }
}
