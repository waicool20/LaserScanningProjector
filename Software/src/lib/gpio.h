#pragma once

#include <climits>
#include <cstdint>

#include <libopencm3/stm32/gpio.h>

#include "helper.h"

class gpio {
 public:
  struct mmio {
    std::uint32_t mode;
    alignas(std::uint32_t) std::uint16_t otype;
    std::uint32_t ospeed;
    std::uint32_t pupd;
    alignas(std::uint32_t) std::uint16_t id;
    alignas(std::uint32_t) std::uint16_t od;
    std::uint32_t bsr;
    alignas(std::uint32_t) std::uint16_t lck;
    std::uint64_t af;
    alignas(std::uint32_t) std::uint16_t br;
  } __attribute__((packed));
  static_assert(sizeof(mmio) == mmio_size(0x28));

  gpio(std::uint32_t gpiox, std::uint16_t p) :
      port(gpiox), pin(p) {}

  void setup(std::uint8_t mode, std::uint8_t pupd);
  void output_opts(std::uint8_t otype, std::uint8_t speed);
  void set_af(uint8_t af_num);

  void state(bool new_state);
  void toggle();

  bool get();

  volatile mmio* handle();

  const std::uint32_t port;
  const std::uint16_t pin;
};
