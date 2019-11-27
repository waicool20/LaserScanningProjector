#ifndef LASERSCANNINGPROJECT_MIC_H
#define LASERSCANNINGPROJECT_MIC_H

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include "lib/gpio.h"

class mic {
  public:
    mic();

    float get_latest_value();
    void enable();
    void disable();
  private:
    static uint32_t _adc;
    static uint8_t _adc_channels[1];
    static gpio _mic;
    volatile static uint32_t latest_value;
    friend void adc3_isr();
    bool enabled = false;
};


#endif //LASERSCANNINGPROJECT_MIC_H
