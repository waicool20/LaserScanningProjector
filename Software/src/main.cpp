#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "drivers/laser.h"
#include "drivers/laser_canvas.h"
#include "drivers/nav5.h"
#include "drivers/stepper_motor.h"
#include "drivers/st7735s.h"
#include "lib/rcc.h"
#include "lib/systick.h"
#include "lib/ui.h"
#include "lib/lvgl/button.h"
#include "lib/lvgl/container.h"
#include "lib/lvgl/group.h"
#include "lib/lvgl/label.h"

using namespace std::literals;

namespace {
  static constexpr double PI = 3.14159265;
  constexpr bool use_laser = false;
}  // namespace

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick::init();

  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOB);

  st7735s lcd{0, 0, 128, 160, st7735s::COLOR_MODE_18_BITS};
  nav5 nav5{
      gpio(GPIOC, GPIO15),
      gpio(GPIOC, GPIO13),
      gpio(GPIOC, GPIO14),
      gpio(GPIOB, GPIO8),
      gpio(GPIOB, GPIO9)
  };

  ui::init(&lcd, &nav5);
  auto cont = lvgl::container();
  cont.auto_realign(true);  /*Auto realign when the size changes*/
  cont.align_origin(nullptr, LV_ALIGN_CENTER, 0, 0);  /*This parametrs will be sued when realigned*/
  cont.fit2(LV_FIT_FLOOD, LV_FIT_FLOOD);
  cont.layout(LV_LAYOUT_PRETTY);

  auto btn = lvgl::button(cont);  /*Add a button the current screen*/
  btn.pos(20, 20);  /* Set its position */
  btn.size(50, 50);  /* Set its size */

  auto label = lvgl::label(btn);  /*Add a label to the button*/
  label.text("Button"sv);  /*Set the labels text*/

  auto btn2 = lvgl::button(cont);  /*Add a button the current screen*/
  btn2.pos(20, 50);  /*Set its position*/
  btn2.size(50, 50);  /*Set its size*/

  auto label2 = lvgl::label(btn2);  /*Add a label to the button*/
  label2.text("Button"sv);  /*Set the labels text*/

  auto g = lvgl::group();
  g.add(btn);
  g.add(btn2);

  lv_indev_set_group(ui::get_input_device(), g.get());

  gpio ldr = gpio(GPIOB, GPIO0);

  rcc_periph_clock_enable(RCC_GPIOA);

  stepper_motor xM{gpio(GPIOB, GPIO7), gpio(GPIOB, GPIO6), gpio(GPIOB, GPIO5)};
  stepper_motor yM{gpio(GPIOB, GPIO4), gpio(GPIOB, GPIO3), gpio(GPIOA, GPIO15)};
  laser laser{};

  laser_canvas canvas{25600, 128, 72, laser, xM, yM, ldr};
  if constexpr (use_laser) {
    canvas.home();
  }

  while (true) {
    systick::sleep(2ms);
    lv_task_handler();
  }
}
