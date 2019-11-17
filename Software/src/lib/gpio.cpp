#include <libopencm3/cm3/nvic.h>
#include "gpio.h"
#include "rcc.h"

std::array<void (*)(bool), 16> gpio::pin_callback {
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop,
    gpio::callback_nop, gpio::callback_nop
};

std::array<gpio*, 16> gpio::exti_pins {};

void gpio::callback_nop(bool) {
  // Do nothing
}

extern "C" void exti0_isr() {
  exti_reset_request(EXTI0);
  if (gpio::exti_pins[0] != nullptr) gpio::pin_callback[0](gpio::exti_pins[0]->get());
  exti_set_trigger(EXTI0, gpio::exti_pins[0]->get_exti_trigger_type());
}

extern "C" void exti1_isr() {
  exti_reset_request(EXTI1);
  if (gpio::exti_pins[1] != nullptr) gpio::pin_callback[1](gpio::exti_pins[1]->get());
  exti_set_trigger(EXTI1, gpio::exti_pins[1]->get_exti_trigger_type());
}

extern "C" void exti2_tsc_isr() {
  exti_reset_request(EXTI2);
  if (gpio::exti_pins[2] != nullptr) gpio::pin_callback[2](gpio::exti_pins[2]->get());
  exti_set_trigger(EXTI2, gpio::exti_pins[2]->get_exti_trigger_type());
}

extern "C" void exti3_isr() {
  exti_reset_request(EXTI3);
  if (gpio::exti_pins[3] != nullptr) gpio::pin_callback[3](gpio::exti_pins[3]->get());
  exti_set_trigger(EXTI3, gpio::exti_pins[3]->get_exti_trigger_type());
}

extern "C" void exti4_isr() {
  exti_reset_request(EXTI4);
  if (gpio::exti_pins[4] != nullptr) gpio::pin_callback[4](gpio::exti_pins[4]->get());
  exti_set_trigger(EXTI4, gpio::exti_pins[4]->get_exti_trigger_type());
}

extern "C" void exti9_5_isr() {
  if (exti_get_flag_status(EXTI5)) {
    exti_reset_request(EXTI5);
    if (gpio::exti_pins[5] != nullptr) gpio::pin_callback[5](gpio::exti_pins[5]->get());
    exti_set_trigger(EXTI5, gpio::exti_pins[5]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI6)) {
    exti_reset_request(EXTI6);
    if (gpio::exti_pins[6] != nullptr) gpio::pin_callback[6](gpio::exti_pins[6]->get());
    exti_set_trigger(EXTI6, gpio::exti_pins[6]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI7)) {
    exti_reset_request(EXTI7);
    if (gpio::exti_pins[7] != nullptr) gpio::pin_callback[7](gpio::exti_pins[7]->get());
    exti_set_trigger(EXTI7, gpio::exti_pins[7]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI8)) {
    exti_reset_request(EXTI8);
    if (gpio::exti_pins[8] != nullptr) gpio::pin_callback[8](gpio::exti_pins[8]->get());
    exti_set_trigger(EXTI8, gpio::exti_pins[8]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI9)) {
    exti_reset_request(EXTI9);
    if (gpio::exti_pins[9] != nullptr) gpio::pin_callback[9](gpio::exti_pins[9]->get());
    exti_set_trigger(EXTI9, gpio::exti_pins[9]->get_exti_trigger_type());
  }
}

extern "C" void exti15_10_isr() {
  if (exti_get_flag_status(EXTI10)) {
    exti_reset_request(EXTI10);
    if (gpio::exti_pins[10] != nullptr) gpio::pin_callback[10](gpio::exti_pins[10]->get());
    exti_set_trigger(EXTI10, gpio::exti_pins[10]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI11)) {
    exti_reset_request(EXTI11);
    if (gpio::exti_pins[11] != nullptr) gpio::pin_callback[11](gpio::exti_pins[11]->get());
    exti_set_trigger(EXTI11, gpio::exti_pins[11]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI12)) {
    exti_reset_request(EXTI12);
    if (gpio::exti_pins[12] != nullptr) gpio::pin_callback[12](gpio::exti_pins[12]->get());
    exti_set_trigger(EXTI12, gpio::exti_pins[12]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI13)) {
    exti_reset_request(EXTI13);
    if (gpio::exti_pins[13] != nullptr) gpio::pin_callback[13](gpio::exti_pins[13]->get());
    exti_set_trigger(EXTI13, gpio::exti_pins[13]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI14)) {
    exti_reset_request(EXTI14);
    if (gpio::exti_pins[14] != nullptr) gpio::pin_callback[14](gpio::exti_pins[14]->get());
    exti_set_trigger(EXTI14, gpio::exti_pins[14]->get_exti_trigger_type());
  } else if (exti_get_flag_status(EXTI15)) {
    exti_reset_request(EXTI15);
    if (gpio::exti_pins[15] != nullptr) gpio::pin_callback[15](gpio::exti_pins[15]->get());
    exti_set_trigger(EXTI15, gpio::exti_pins[15]->get_exti_trigger_type());
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

void gpio::enable_ext_interrupt(exti_trigger_type type) {
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
  exti_pins[get_pin_num()] = this;
  // Should probably use EXTI enums but using pin for convenience
  exti_select_source(pin, port);
  exti_set_trigger(pin, type);
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

exti_trigger_type gpio::get_exti_trigger_type() const {
  return _trigger_type;
}

void gpio::set_exti_callback(void (*action)(bool)) {
  pin_callback[_pin_num] = action;
}
