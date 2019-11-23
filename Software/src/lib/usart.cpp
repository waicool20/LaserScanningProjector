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

uint32_t usart::recv_string_blocking(char* string, uint32_t len) {
  std::uint32_t read;
  for (read = 0; read < len - 1; read++) {
    std::uint8_t data = recv_blocking();
    if (data == '\n'){
      break;
    }
    string[read] = static_cast<char>(data);
  }
  string[read] = '\0';

  return read;
}

void usart::send_blocking(uint16_t data) {
  usart_send_blocking(_usart, data);
}

void usart::send_blocking(std::string_view sv) {
  for (char c : sv) {
    send_blocking(static_cast<unsigned char>(c));
  }
}

void usart::send_blocking(char *string, uint32_t len) {
  char data;
  for(uint32_t i = 0; i < len; i++) {
    data = string[i];
    if (data == '\0') {
      send_blocking('\n');
      break;
    }
    send_blocking(static_cast<uint16_t>(data));
  }
}
