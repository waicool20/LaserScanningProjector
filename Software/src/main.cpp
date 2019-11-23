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
#include <lv_core/lv_style.h>
#include <lib/usart.h>

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
  lv_obj_t *cont = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_auto_realign(cont, true);                    /*Auto realign when the size changes*/
  lv_obj_align_origo(cont, NULL, LV_ALIGN_CENTER, 0, 0);  /*This parametrs will be sued when realigned*/
  lv_cont_set_fit2(cont, LV_FIT_FLOOD, LV_FIT_FLOOD);
  lv_cont_set_layout(cont, LV_LAYOUT_PRETTY);

  lv_obj_t *btn = lv_btn_create(cont, NULL);     /*Add a button the current screen*/
  lv_obj_set_pos(btn, 20, 20);                            /*Set its position*/
  lv_obj_set_size(btn, 50, 50);                          /*Set its size*/

  lv_obj_t *label = lv_label_create(btn, NULL);          /*Add a label to the button*/
  lv_label_set_text(label, "Button");                     /*Set the labels text*/

  lv_obj_t *btn2 = lv_btn_create(cont, NULL);     /*Add a button the current screen*/
  lv_obj_set_pos(btn2, 20, 50);                            /*Set its position*/
  lv_obj_set_size(btn2, 50, 50);                          /*Set its size*/

  lv_obj_t *label2 = lv_label_create(btn2, NULL);          /*Add a label to the button*/
  lv_label_set_text(label2, "Button");                     /*Set the labels text*/

  lv_group_t *g = lv_group_create();
  lv_group_add_obj(g, btn);
  lv_group_add_obj(g, btn2);

  lv_indev_set_group(ui::get_input_device(), g);

  gpio ldr = gpio(GPIOB, GPIO0);

  rcc_periph_clock_enable(RCC_GPIOA);

  stepper_motor xM{gpio(GPIOB, GPIO7), gpio(GPIOB, GPIO6), gpio(GPIOB, GPIO5)};
  stepper_motor yM{gpio(GPIOB, GPIO4), gpio(GPIOB, GPIO3), gpio(GPIOA, GPIO15)};
  laser laser{};

  laser_canvas canvas{25600, 128, 72, laser, xM, yM, ldr};
  if constexpr (use_laser) {
    canvas.home();
  }

  char string[16];

  usart usart{115200};
  while (true) {
    //usart.send_blocking("Hello World\n");
    systick::sleep_ms(2);
    lv_task_handler();
    usart.recv_string_blocking(string, 16);
    usart.send_blocking(std::string_view{string});
    lv_label_set_text(label, string);
  }
}
