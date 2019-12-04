#pragma once

#include <cstdint>
#include <chrono>

/**
 * \brief Abstraction for Cortex-M3 systick.
 */
class systick {
 public:
  /**
   * \brief Initializes the systick timer.
   */
  static void init();

  /**
   * \return Elapsed microseconds since init.
   */
  static std::uint64_t us();
  /**
   * \return Elapsed milliseconds since init.
   */
  static std::uint64_t ms() { return us() / 1000; }

  /**
   * \brief Blocking sleep for given time.
   *
   * \param sleep_us sleep time in microseconds.
   */
  static void sleep_us(std::uint64_t sleep_us);
  /**
   * \brief Blocking sleep for given time.
   *
   * \param sleep_ms sleep time in milliseconds.
   */
  static void sleep_ms(std::uint32_t sleep_ms) { sleep_us(sleep_ms * 1000); }

  /**
   * \brief Blocking sleep.
   *
   * \tparam Dur std::chrono::duration.
   * \param dur duration of sleep
   */
  template<typename Dur>
  static void sleep(Dur dur) {
    sleep_us(std::uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(dur).count()));
  }
};
