#ifndef LASERSCANNINGPROJECT_LASER_H
#define LASERSCANNINGPROJECT_LASER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <lib/gpio.h>

/**
 * Driver for laser, provides controls for turning it on/off and brightness control
 */
class laser {
  public:
    laser();

    /**
     * Turns the laser on
     */
    void enable();

    /**
     * Turns the laser off
     */
    void disable();

    /**
     * Toggles the laser on/off
     */
    void toggle();

    /**
     * Sets the brightness
     *
     * @param percent Percentage of brightness (0 - 100), passed in values will be clamped
     * @return Clamped percentage set
     */
    uint8_t set_brightness(uint8_t percent);
  private:
    gpio _en_ = gpio(GPIOA, GPIO8);
};

#endif //LASERSCANNINGPROJECT_LASER_H
