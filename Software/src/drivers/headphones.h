#ifndef LASERSCANNINGPROJECT_HEADPHONES_H
#define LASERSCANNINGPROJECT_HEADPHONES_H

#include <cstdint>
#include "lib/gpio.h"

class headphones {
  public:
    headphones();
    void enable();
    void disable();
    float channel_average() {return (normalize(channel_data[0]) + normalize(channel_data[1])) / 2; };
    float channel_x() { return normalize(channel_data[0]); };
    float channel_y() { return normalize(channel_data[1]); };
  private:
    static uint32_t _dma;
    static uint32_t _dma_channel;

    static uint32_t _adc;
    static uint8_t _adc_channels[2];

    static gpio _left_ch;
    static gpio _right_ch;

    volatile static uint16_t channel_data[2];

    bool enabled = false;

    float normalize(uint16_t data) { return (data / float(4096) - 0.5) * 2 * 1.2; }
};


#endif //LASERSCANNINGPROJECT_HEADPHONES_H
