#include "laser.h"
#include <libopencm3/stm32/f3/rcc.h>
#include <libopencm3/stm32/f3/timer.h>
#include <algorithm>

laser::laser() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_TIM1);

  _en_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _en_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _en_.set_af(GPIO_AF6);

  timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
  timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM2);
  timer_enable_oc_output(TIM1, TIM_OC1);
  timer_enable_break_main_output(TIM1);
  timer_set_oc_value(TIM1, TIM_OC1, 200);
  timer_set_period(TIM1, 1000);
  timer_enable_counter(TIM1);

  disable();
}

void laser::enable() {
  _en_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _en_.state(true);
}

void laser::disable() {
  _en_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _en_.state(false);
}

void laser::toggle() {
  if (_en_.get()) {
    disable();
  } else {
    enable();
  }
}

uint8_t laser::set_brightness(uint8_t percent) {
  uint8_t p = std::clamp((int) percent, 0, 100);
  timer_set_oc_value(TIM1, TIM_OC1, p * 10);
  return p;
}
