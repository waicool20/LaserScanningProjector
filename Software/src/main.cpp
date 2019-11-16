#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <drivers/st7735s.h>
#include <drivers/laser.h>
#include <algorithm>
#include <drivers/laser_canvas.h>
#include <lib/ui.h>
#include <drivers/nav5.h>

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

  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOB);

  st7735s lcd{2, 1, 128, 160, st7735s::COLOR_MODE_18_BITS};
  nav5 nav5 {
      gpio(GPIOC, GPIO15),
      gpio(GPIOC, GPIO13),
      gpio(GPIOC, GPIO14),
      gpio(GPIOB, GPIO8),
      gpio(GPIOB, GPIO9)
  };

  ui ui { &lcd, &nav5 };
  ui.init();

  lv_obj_t* btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
  lv_obj_set_pos(btn, 20, 20);                            /*Set its position*/
  lv_obj_set_size(btn, 50, 20);                          /*Set its size*/

  lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
  lv_label_set_text(label, "Button");                     /*Set the labels text*/

  gpio ldr = gpio(GPIOB, GPIO0);

  rcc_periph_clock_enable(RCC_GPIOA);

  stepper_motor xM{gpio(GPIOB, GPIO7), gpio(GPIOB, GPIO6), gpio(GPIOB, GPIO5)};
  stepper_motor yM{gpio(GPIOB, GPIO4), gpio(GPIOB, GPIO3), gpio(GPIOA, GPIO15)};
/*  laser laser{};

  laser_canvas canvas{25600, 128, 72, laser, xM, yM, ldr};
  if constexpr (use_laser) {
    canvas.home();
  }*/

  while (true) {
    systick::sleep_ms(2);
    lv_task_handler();
  }
}
