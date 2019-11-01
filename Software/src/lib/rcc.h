#pragma once

#include <libopencm3/stm32/rcc.h>

struct rcc {
  inline static void clock_setup_pll(const rcc_clock_scale& clock) { rcc_clock_setup_pll(&clock); }
  inline static void periph_clock_enable(rcc_periph_clken periph) { rcc_periph_clock_enable(periph); }
};
