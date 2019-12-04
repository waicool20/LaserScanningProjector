#pragma once

#include <lib/gpio.h>
#include "laser.h"
#include "stepper_motor.h"

/**
 * Abstraction for the renderable region by the laser.
 */
class laser_canvas {
 public:
  /**
   * \brief A point specification for tuple-based rendering.
   */
  struct tuple {
    /**
     * \brief X-coordinate.
     */
    std::uint16_t x;
    /**
     * \brief Y-coordinate.
     */
    std::uint16_t y;
    /**
     * \brief State of the laser when point is reached.
     */
    bool state;
  };

  /**
   * Main constructor for laser_canvas
   * @param spr Steps per Revolution, number of steps it takes for the stepper motors to make one revolution
   * @param width Width of the drawing area
   * @param height Height of the drawing area
   * @param laser Laser used for drawing
   * @param x_motor Stepper motor that controls the x direction
   * @param y_motor Stepper motor that controls the y direction
   * @param ldr Homing LDR gpio
   */
  laser_canvas(std::uint32_t spr,
               std::uint32_t width,
               std::uint32_t height,
               laser laser,
               stepper_motor x_motor,
               stepper_motor y_motor,
               gpio ldr);

  /**
   * Homes the laser to (0, 0) using the ldr sensor
   *
   * @return True if homing was successful, otherwise false
   */
  bool home();

  /**
   * \brief Moves the laser to the specified coordinate.
   *
   * This method will block for 5-8 ms for the motor to move to the specified coordinate.
   *
   * \param x x-coordinate
   * \param y y-coordinate
   * \param no_delay whether to delay to allow the motor to move
   */
  void goto_xy(std::uint32_t x, std::uint32_t y, bool no_delay);
  /**
   * \brief Draws the magnitude of a source with time as its X-axis.
   *
   * \param magnitude magnitude at the current time
   */
  void draw_magnitude_y(float magnitude);
  /**
   * \brief Draws the magnitude of two sources simultaneously.
   *
   * \param magnitude_x magnitude of x
   * \param magnitude_y magnitude of y
   */
  void draw_magnitude_xy(float magnitude_x, float magnitude_y);
  /**
   * \brief Draws a bitmap onto the canvas.
   *
   * The canvas must be resized to the appropriate size, and the data is assumed to be in packed one-bit-per-pixel
   * format.
   *
   * \param frame_data pointer to the bitmap to render
   */
  void draw_frame(const std::uint8_t* frame_data);
  /**
   * \brief Renders an array of tuples to onto the canvas.
   *
   * \param tuples pointer to the first tuple of the array
   * \param size number of tuples to render
   */
  void draw_tuples(const tuple* tuples, std::size_t size);
  /**
   * \brief Draws a rectangle to indicate the drawable region of the canvas at 128x72.
   */
  void highlight_canvas_area();
  /**
   * \brief Disables the laser.
   */
  void clear() { _laser.disable(); }

  [[nodiscard]] std::uint32_t get_current_x() const;
  [[nodiscard]] std::uint32_t get_current_y() const;
  [[nodiscard]] std::uint32_t get_width() const;
  [[nodiscard]] std::uint32_t get_height() const;

  /**
   * \brief Resizes the canvas to the specified size.
   *
   * \param w new width
   * \param h new height
   */
  void resize(uint32_t w, uint32_t h);
 private:
  /**
   * \brief Converts a coordinate to a pair of array index and its bitshift offset.
   *
   * \param x x-coordinate
   * \param y y-coordinate
   * \return Pair of array index and its bitshift offset.
   */
  [[nodiscard]] std::pair<std::size_t, std::uint8_t> index_conversion(std::uint32_t x, std::uint32_t y) const;

  std::uint32_t current_x = 0;
  std::uint32_t current_y = 0;

  std::uint32_t _spr;
  std::uint32_t _width;
  std::uint32_t _height;
  laser _laser;
  stepper_motor _x_motor;
  stepper_motor _y_motor;
  gpio _ldr;
};
