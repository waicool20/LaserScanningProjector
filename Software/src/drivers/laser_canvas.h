#ifndef LASERSCANNINGPROJECT_LASER_CANVAS_H
#define LASERSCANNINGPROJECT_LASER_CANVAS_H

#include <lib/gpio.h>
#include "laser.h"
#include "stepper_motor.h"

class laser_canvas {
  public:
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
    laser_canvas(uint32_t spr, uint32_t width, uint32_t height, laser laser, stepper_motor x_motor, stepper_motor y_motor, gpio ldr);

    /**
     * Homes the laser to (0, 0) using the ldr sensor
     *
     * @return True if homing was successful, otherwise false
     */
    bool home();
  private:
    uint32_t current_x = 0;
    uint32_t current_y = 0;

    uint32_t _spr;
    uint32_t _width;
    uint32_t _height;
    laser _laser;
    stepper_motor _x_motor;
    stepper_motor _y_motor;
    gpio _ldr;
};

#endif //LASERSCANNINGPROJECT_LASER_CANVAS_H
