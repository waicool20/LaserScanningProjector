#ifndef LASERSCANNINGPROJECT_USART_H
#define LASERSCANNINGPROJECT_USART_H

#include "gpio.h"
#include <libopencm3/stm32/usart.h>

class usart {
  public:
    usart(uint32_t baudrate);

    uint16_t recv_blocking();
    void send_blocking(uint16_t data);
    void send_blocking(std::string_view string);
  private:
    const uint32_t _usart = USART2;

    gpio _tx = gpio{GPIOA, GPIO2};
    gpio _rx = gpio{GPIOA, GPIO3};
};


#endif //LASERSCANNINGPROJECT_USART_H
