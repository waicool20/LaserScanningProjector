#ifndef LASERSCANNINGPROJECT_USART_H
#define LASERSCANNINGPROJECT_USART_H

#include "gpio.h"
#include <libopencm3/stm32/usart.h>

class usart {
  public:
    usart(uint32_t baudrate);

    uint16_t recv_blocking();
    uint32_t recv_string_blocking(char* string, uint32_t len);
    void send_blocking(uint16_t data);
    void send_blocking(std::string_view string);
    void send_blocking(char* string, uint32_t len);
  private:
    const uint32_t _usart = USART2;

    gpio _tx = gpio{GPIOA, GPIO2};
    gpio _rx = gpio{GPIOA, GPIO3};
};


#endif //LASERSCANNINGPROJECT_USART_H
