#pragma once

#include <cstdint>

class systick {
 public:
  static void init();

  static std::uint64_t us();
  static std::uint64_t ms() { return us() / 1000; }

  static void sleep_us(std::uint64_t sleep_us);
  static void sleep_ms(std::uint32_t sleep_ms) { sleep_us(sleep_ms * 1000); }
};
