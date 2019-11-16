#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>
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
  std::uint8_t brightness(std::uint8_t percent);

 private:
  gpio _en_ = gpio(GPIOA, GPIO8);
};
