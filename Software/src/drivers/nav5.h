#ifndef LASERSCANNINGPROJECT_NAV5_H
#define LASERSCANNINGPROJECT_NAV5_H

#include <lib/gpio.h>

/**
 * \brief Driver for the 5-way navigation switch.
 */
class nav5 {
 public:
  nav5(const gpio& up, const gpio& down, const gpio& left, const gpio& right, const gpio& center);

  bool read_up() { return _up.get(); }
  bool read_down() { return _down.get(); }
  bool read_left() { return _left.get(); }
  bool read_right() { return _right.get(); }
  bool read_center() { return _center.get(); }

  /**
   * \brief Sets the callback when the up button is pressed.
   * \param action action to execute when the button is pressed. The first argument refers to the new state of the
   * button.
   */
  void on_up(void (* action)(bool state)) { _up.set_exti_callback(action); }
  void on_down(void (* action)(bool state)) { _down.set_exti_callback(action); }
  void on_left(void (* action)(bool state)) { _left.set_exti_callback(action); }
  void on_right(void (* action)(bool state)) { _right.set_exti_callback(action); }
  void on_center(void (* action)(bool state)) { _center.set_exti_callback(action); }
 private:
  gpio _up;
  gpio _down;
  gpio _left;
  gpio _right;
  gpio _center;
};

#endif //LASERSCANNINGPROJECT_NAV5_H
