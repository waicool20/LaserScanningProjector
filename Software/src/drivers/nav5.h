#ifndef LASERSCANNINGPROJECT_NAV5_H
#define LASERSCANNINGPROJECT_NAV5_H

#include <lib/gpio.h>

class nav5 {
  public:
    nav5(const gpio &up, const gpio &down, const gpio &left, const gpio &right, const gpio &center);

    bool read_up() { return _up.get(); }
    bool read_down() { return _down.get(); }
    bool read_left() { return _left.get(); }
    bool read_right() { return _right.get(); }
    bool read_center() { return _center.get(); }

    void on_up(void (*action)(bool state)) { gpio::pin_callback[_up.get_pin_num()] = action; }
    void on_down(void (*action)(bool state)) { gpio::pin_callback[_down.get_pin_num()] = action; }
    void on_left(void (*action)(bool state)) { gpio::pin_callback[_left.get_pin_num()] = action; }
    void on_right(void (*action)(bool state)) { gpio::pin_callback[_right.get_pin_num()] = action; }
    void on_center(void (*action)(bool state)) { gpio::pin_callback[_center.get_pin_num()] = action; }
  private:
    gpio _up;
    gpio _down;
    gpio _left;
    gpio _right;
    gpio _center;
};


#endif //LASERSCANNINGPROJECT_NAV5_H
