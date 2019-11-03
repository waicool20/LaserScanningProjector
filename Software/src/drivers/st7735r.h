#pragma once

#include "lib/gpio.h"

class st7735r {
 public:

 private:
  gpio _sck_ = gpio(GPIOA, GPIO5);
  gpio _mosi_ = gpio(GPIOA, GPIO7);
  gpio _res_ = gpio(GPIOB, GPIO2);
  gpio _dc_ = gpio(GPIOB, GPIO10);
  gpio _cs_ = gpio(GPIOA, GPIO4);
  gpio _bl_ = gpio(GPIOB, GPIO11);
};
