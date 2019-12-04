#ifndef LASERSCANNINGPROJECT_USART_H
#define LASERSCANNINGPROJECT_USART_H

#include "gpio.h"
#include <libopencm3/stm32/usart.h>

/**
 * \brief Abstraction for USART.
 */
class usart {
 public:
  /**
   * \param baudrate baud rate for the UART peripheral
   */
  usart(uint32_t baudrate);

  /**
   * \brief Blockingly receives a character.
   *
   * \return Received character.
   */
  uint16_t recv_blocking();
  /**
   * \brief Blockingly receives a string.
   *
   * \param string pointer to the start of the buffer
   * \param len length of the buffer
   * \return Actual received size.
   */
  uint32_t recv_string_blocking(char* string, uint32_t len);
  /**
   * \brief Blockingly sends a character.
   *
   * \param data data to send
   */
  void send_blocking(uint16_t data);
  /**
   * \brief Blockingly sends a string
   *
   * \param string string_view to send
   */
  void send_blocking(std::string_view string);
  /**
   * \brief Blockingly sends a string
   *
   * \param string pointer to first character to send
   * \param len length of string
   */
  void send_blocking(char* string, uint32_t len);
 private:
  const uint32_t _usart = USART2;

  gpio _tx = gpio{GPIOA, GPIO2};
  gpio _rx = gpio{GPIOA, GPIO3};
};

#endif //LASERSCANNINGPROJECT_USART_H
