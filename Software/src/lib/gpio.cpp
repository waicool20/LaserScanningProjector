#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include "gpio.h"
#include "rcc.h"

void (*gpio::pin_callback[16])() = {
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop
};

void gpio::callback_nop() {
  // Do nothing
}

extern "C" void exti0_isr() {
  exti_reset_request(EXTI0);
  gpio::pin_callback[0]();
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
}

extern "C" void exti1_isr() {
  exti_reset_request(EXTI1);
  gpio::pin_callback[1]();
  exti_set_trigger(EXTI1, EXTI_TRIGGER_RISING);
}

extern "C" void exti2_tsc_isr() {
  exti_reset_request(EXTI2);
  gpio::pin_callback[2]();
  exti_set_trigger(EXTI2, EXTI_TRIGGER_RISING);
}

extern "C" void exti3_isr() {
  exti_reset_request(EXTI3);
  gpio::pin_callback[3]();
  exti_set_trigger(EXTI3, EXTI_TRIGGER_RISING);
}

extern "C" void exti4_isr() {
  exti_reset_request(EXTI4);
  gpio::pin_callback[4]();
  exti_set_trigger(EXTI4, EXTI_TRIGGER_RISING);
}

extern "C" void exti9_5_isr() {
  if (exti_get_flag_status(EXTI5)) {
    exti_reset_request(EXTI5);
    gpio::pin_callback[5]();
    exti_set_trigger(EXTI5, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI6)) {
    exti_reset_request(EXTI6);
    gpio::pin_callback[6]();
    exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI7)) {
    exti_reset_request(EXTI7);
    gpio::pin_callback[7]();
    exti_set_trigger(EXTI7, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI8)) {
    exti_reset_request(EXTI8);
    gpio::pin_callback[8]();
    exti_set_trigger(EXTI8, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI9)) {
    exti_reset_request(EXTI9);
    gpio::pin_callback[9]();
    exti_set_trigger(EXTI9, EXTI_TRIGGER_RISING);
  }
}

extern "C" void exti15_10_isr() {
  if (exti_get_flag_status(EXTI10)) {
    exti_reset_request(EXTI10);
    gpio::pin_callback[10]();
    exti_set_trigger(EXTI10, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI11)) {
    exti_reset_request(EXTI11);
    gpio::pin_callback[11]();
    exti_set_trigger(EXTI11, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI12)) {
    exti_reset_request(EXTI12);
    gpio::pin_callback[12]();
    exti_set_trigger(EXTI12, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI13)) {
    exti_reset_request(EXTI13);
    gpio::pin_callback[13]();
    exti_set_trigger(EXTI13, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI14)) {
    exti_reset_request(EXTI14);
    gpio::pin_callback[14]();
    exti_set_trigger(EXTI14, EXTI_TRIGGER_RISING);
  } else if (exti_get_flag_status(EXTI15)) {
    exti_reset_request(EXTI15);
    gpio::pin_callback[15]();
    exti_set_trigger(EXTI15, EXTI_TRIGGER_RISING);
  }
}

gpio::gpio(std::uint32_t gpiox, std::uint16_t p) : port(gpiox), pin(p) {
  while (p = p >> 1) ++_pin_num;
}

void gpio::setup(std::uint8_t mode, std::uint8_t pupd) {
  gpio_mode_setup(port, mode, pupd, pin);
}

void gpio::output_opts(std::uint8_t otype, std::uint8_t speed) {
  gpio_set_output_options(port, otype, speed, pin);
}

void gpio::set_af(uint8_t af_num) {
  gpio_set_af(port, af_num, pin);
}

void gpio::enable_ext_interrupt() {
  rcc::periph_clock_enable(RCC_SYSCFG);
  switch (pin) {
    case EXTI0:
      nvic_enable_irq(NVIC_EXTI0_IRQ);
      break;
    case EXTI1:
      nvic_enable_irq(NVIC_EXTI1_IRQ);
      break;
    case EXTI2:
      nvic_enable_irq(NVIC_EXTI2_TSC_IRQ);
      break;
    case EXTI3:
      nvic_enable_irq(NVIC_EXTI3_IRQ);
      break;
    case EXTI4:
      nvic_enable_irq(NVIC_EXTI4_IRQ);
      break;
    case EXTI5:
    case EXTI6:
    case EXTI7:
    case EXTI8:
    case EXTI9:
      nvic_enable_irq(NVIC_EXTI9_5_IRQ);
      break;
    case EXTI10:
    case EXTI11:
    case EXTI12:
    case EXTI13:
    case EXTI14:
    case EXTI15:
      nvic_enable_irq(NVIC_EXTI15_10_IRQ);
      break;
    default:
      return;
  }
  // Should probably use EXTI enums but using pin for convenience
  exti_select_source(pin, port);
  exti_set_trigger(pin, EXTI_TRIGGER_RISING);
  exti_enable_request(pin);
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

volatile gpio::mmio *gpio::handle() {
  return reinterpret_cast<mmio *>(port);
}

uint8_t gpio::get_pin_num() const {
  return _pin_num;
}
