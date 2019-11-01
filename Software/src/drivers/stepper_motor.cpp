#include "stepper_motor.h"

#include "lib/systick.h"

stepper_motor::stepper_motor(gpio en, gpio dir, gpio step) :
    _en_(en), _dir_(dir), _step_(step) {
  _en_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _dir_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _step_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);

  _en_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _dir_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _step_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
}

void stepper_motor::enable() {
  _en_.state(false);
  systick::sleep_us(5);
}

void stepper_motor::disable() {
  _en_.state(true);
  systick::sleep_us(5);
}

void stepper_motor::toggle() {
  _en_.toggle();
  systick::sleep_us(5);
}
void stepper_motor::set_dir(direction dir) {
  _dir_.state(dir == direction::cw);
  systick::sleep_us(2);
}

void stepper_motor::toggle_dir() {
  _dir_.toggle();
  systick::sleep_us(2);
}

void stepper_motor::do_steps(std::uint32_t steps) {
  _step_.state(true);
  // TODO: steps not used in this context
  systick::sleep_us(2);
  _step_.state(false);
}
