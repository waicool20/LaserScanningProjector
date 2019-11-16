#include "nav5.h"

nav5::nav5(const gpio &up, const gpio &down, const gpio &left, const gpio &right, const gpio &center) :
    _up(up), _down(down), _left(left), _right(right), _center(center) {
  _up.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  _down.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  _left.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  _right.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);
  _center.setup(GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN);

  _up.enable_ext_interrupt(EXTI_TRIGGER_BOTH);
  _down.enable_ext_interrupt(EXTI_TRIGGER_BOTH);
  _left.enable_ext_interrupt(EXTI_TRIGGER_BOTH);
  _right.enable_ext_interrupt(EXTI_TRIGGER_BOTH);
  _center.enable_ext_interrupt(EXTI_TRIGGER_BOTH);
}
