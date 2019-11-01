#ifndef LASERSCANNINGPROJECT_MOTOR_H
#define LASERSCANNINGPROJECT_MOTOR_H

#include <cstdint>

/**
 * Struct for holding GPIO port and pin combination
 */
struct PortPin {
  uint32_t port;
  uint16_t pin;
};

/**
 * Enum for motor direction, CW: ClockWise; CCW: CounterClockWise
 */
enum Direction {
  CW, CCW
};

/**
 * Low level interface for the basic functions of a stepper motor
 * Designed according to specification of the HB808C stepper motor driver
 *
 * <a href="http://stzhikong.com/FCKfiles/file/HB808C%20V1_0%E6%B7%B7%E5%90%88%E4%BC%BA%E6%9C%8D%E9%A9%B1%E5%8A%A8%E5%99%A8%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6(1).pdf">HB808C Datasheet</a>
 */
class Motor {
private:
  PortPin en;
  PortPin dir;
  PortPin step;
public:
  Motor(PortPin en, PortPin dir, PortPin step);

  /**
   * Enable the stepper motor, this locks the stepper motor at its current position
   */
  void enable();

  /**
   * Disables the stepper motor, allowing it to spin freely
   */
  void disable();

  /**
   * Toggles the stepper motor, switching between enable and disable status
   */
  void toggle();

  /**
   * Set the spin direction of the stepper motor
   * @param direction New direction
   */
  void setDirection(Direction direction);

  /**
   * Toggles the current direction
   */
  void toggleDirection();

  /**
   * Steps the motor the given number of steps
   * @param steps Number of steps
   */
  void doSteps(uint32_t steps);
};

#endif //LASERSCANNINGPROJECT_MOTOR_H
