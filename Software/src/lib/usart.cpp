#include <libopencm3/stm32/rcc.h>
#include "usart.h"
#include "rcc.h"

usart::usart(uint32_t baudrate) {
  rcc::periph_clock_enable(RCC_USART2);
  rcc::periph_clock_enable(RCC_GPIOA);

  _tx.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _rx.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);

  _tx.set_af(GPIO_AF7);
  _rx.set_af(GPIO_AF7);

  usart_set_baudrate(_usart, baudrate);
  usart_set_databits(_usart, 8);
  usart_set_stopbits(_usart, USART_STOPBITS_1);
  usart_set_mode(_usart, USART_MODE_TX_RX);
  usart_set_parity(_usart, USART_PARITY_NONE);
  usart_set_flow_control(_usart, USART_FLOWCONTROL_NONE);

  usart_enable(USART2);
}

uint16_t usart::recv_blocking() {
  return usart_recv_blocking(_usart);
}

std::string usart::recv_string_blocking() {
  uint8_t data;
  std::string string {};
  while(true) {
    data = recv_blocking();
    string.push_back(static_cast<char>(data));
    if (data == '\n') break;
  }
  return string;
}

void usart::send_blocking(uint16_t data) {
  usart_send_blocking(_usart, data);
}

void usart::send_blocking(std::string_view sv) {
  for (char c : sv) {
    send_blocking(static_cast<unsigned char>(c));
  }
}
