#pragma once

#include <cstdint>

#include "lib/gpio.h"

/**
 * Low level interface for the basic functions of a stepper motor
 * Designed according to specification of the HB808C stepper motor driver
 *
 * <a href="http://stzhikong.com/FCKfiles/file/HB808C%20V1_0%E6%B7%B7%E5%90%88%E4%BC%BA%E6%9C%8D%E9%A9%B1%E5%8A%A8%E5%99%A8%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6(1).pdf">HB808C Datasheet</a>
 */
class stepper_motor {
 public:
  /**
   * Enum for motor direction, CW: ClockWise; CCW: CounterClockWise
   */
  enum direction {
    cw, ccw
  };

  stepper_motor(gpio en, gpio dir, gpio step);

  /**
   * Sets stepper motor to enabled or disabled state
   * When stepper motor is enabled, the motor position will be locked and cannot be turned
   * When stepper motor is disabled, the motor can spin freely
   *
   * @param en New state
   */
  void set_enable(bool en);

  /**
   * Toggles the enable state
   */
  void toggle_enable();

  /**
   * Set the spin direction of the stepper motor
   * @param direction New direction
   */
  void set_dir(direction dir);

  /**
   * Toggles the current direction
   */
  void toggle_dir();

  /**
   * Steps the motor the given number of steps
   * @param steps Number of steps
   */
  void do_steps(std::uint32_t steps);

 private:
  gpio _en_;
  gpio _dir_;
  gpio _step_;
};
