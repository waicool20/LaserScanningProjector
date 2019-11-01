#include "systick.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

namespace {
volatile std::uint64_t value = 0;
}  // namespace

extern "C" void sys_tick_handler() {
  ++value;
}

void systick::init() {
  systick_set_reload(8);
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  systick_counter_enable();
  systick_interrupt_enable();
}

void systick::sleep_us(std::uint64_t sleep_us) {
  std::uint64_t wake = us() + sleep_us;
  while (wake > us()) {}
}

std::uint64_t systick::us() {
  return value;
}
