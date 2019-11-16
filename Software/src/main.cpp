#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <drivers/st7735s.h>
#include <drivers/laser.h>
#include <algorithm>
#include <drivers/laser_canvas.h>

#include "drivers/stepper_motor.h"
#include "lib/rcc.h"
#include "lib/systick.h"

namespace {
static constexpr double PI = 3.14159265;
constexpr bool use_laser = false;
}  // namespace

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick::init();

  st7735s lcd{2, 1, 128, 160, st7735s::COLOR_MODE_18_BITS};
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio up = gpio(GPIOC, GPIO15);
  gpio down = gpio(GPIOC, GPIO13);
  gpio left = gpio(GPIOC, GPIO14);
  gpio right = gpio(GPIOB, GPIO8);
  gpio center = gpio(GPIOB, GPIO9);

  gpio ldr = gpio(GPIOB, GPIO0);

  rcc_periph_clock_enable(RCC_GPIOA);

  stepper_motor xM{gpio(GPIOB, GPIO7), gpio(GPIOB, GPIO6), gpio(GPIOB, GPIO5)};
  stepper_motor yM{gpio(GPIOB, GPIO4), gpio(GPIOB, GPIO3), gpio(GPIOA, GPIO15)};
  laser laser{};

  up.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  down.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  left.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  right.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  center.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);

  laser_canvas canvas{25600, 128, 72, laser, xM, yM, ldr};
  if constexpr (use_laser) {
    canvas.home();
  }

  lcd.draw_rect(1, 1, 32, 32, 0x00979D);
  lcd.draw_rect(1, lcd.height() - 33, 32, 32, 0x00979D);
  lcd.draw_rect(lcd.width() - 33, 1, 32, 32, 0x00979D);
  lcd.draw_rect(lcd.width() - 33, lcd.height() - 33, 32, 32, 0x00979D);

  std::uint8_t x = 0;
  std::uint8_t y = 0;
  std::uint8_t dc = 50;
  while (true) {
    if (up.get()) {
      y -= 8;
      yM.set_dir(stepper_motor::cw);
      yM.do_steps(20);
    }
    if (down.get()) {
      y += 8;
      yM.set_dir(stepper_motor::ccw);
      yM.do_steps(20);
    }
    if (left.get()) {
      x -= 8;
      xM.set_dir(stepper_motor::ccw);
      xM.do_steps(20);
    }
    if (right.get()) {
      x += 8;
      xM.set_dir(stepper_motor::cw);
      xM.do_steps(20);
    }
    if (use_laser && center.get()) {
      //canvas.home();
      uint16_t deg = 0;
      uint16_t centx = canvas.get_width() / 2;
      uint16_t centy = canvas.get_height() / 2;
      while (true) {
/*        canvas.goto_xy(centx + 35 * cos(deg * PI / 180), centy + 35 * sin(deg * PI / 180));
        if (++deg == 360) deg = 0;*/
        for (uint32_t i = 0; i < canvas.get_height(); ++i) {
          canvas.goto_xy(0, i);
        }
        systick::sleep_ms(5);
        for (uint32_t i = 0; i < canvas.get_width(); ++i) {
          canvas.goto_xy(i, canvas.get_height());
        }
        systick::sleep_ms(5);
        for (uint32_t i = canvas.get_height(); i > 0; --i) {
          canvas.goto_xy(canvas.get_width(), i);
        }
        systick::sleep_ms(5);
        for (uint32_t i = canvas.get_width(); i > 0; --i) {
          canvas.goto_xy(i, 0);
        }
        systick::sleep_ms(5);
      }

      //laser.toggle();
    }
    systick::sleep_ms(45);
  }
}
