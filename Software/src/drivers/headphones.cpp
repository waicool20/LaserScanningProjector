#include "headphones.h"

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/adc.h>

#include "lib/rcc.h"
#include "lib/systick.h"

uint32_t headphones::_dma = DMA1;
uint32_t headphones::_dma_channel = DMA_CHANNEL1;

uint32_t headphones::_adc = ADC1;
uint8_t headphones::_adc_channels[2] = {1, 2};

gpio headphones::_left_ch = gpio{GPIOA, GPIO1};
gpio headphones::_right_ch = gpio{GPIOA, GPIO0};

volatile uint16_t headphones::channel_data[2] = {0, 0};

headphones::headphones() {
  // ADC
  {
    rcc::periph_clock_enable(RCC_ADC12);
    rcc::periph_clock_enable(RCC_ADC34);
    rcc::periph_clock_enable(RCC_GPIOA);

    _left_ch.setup(GPIO_MODE_ANALOG, GPIO_PUPD_NONE);
    _right_ch.setup(GPIO_MODE_ANALOG, GPIO_PUPD_NONE);
    
    adc_enable_regulator(_adc);
    systick::sleep_ms(5);

    adc_power_off(_adc);
    adc_set_clk_prescale(_adc, ADC_CCR_CKMODE_DIV2);
    adc_set_continuous_conversion_mode(_adc);
    adc_disable_external_trigger_regular(_adc);
    adc_set_right_aligned(_adc);
    adc_set_sample_time_on_all_channels(_adc, ADC_SMPR_SMP_61DOT5CYC);
    adc_set_regular_sequence(_adc, 2, _adc_channels);
    adc_set_resolution(_adc, ADC_CFGR1_RES_12_BIT);
    adc_disable_analog_watchdog_regular(_adc);
    adc_enable_dma_circular_mode(_adc);
  }

  // DMA
  {
    rcc::periph_clock_enable(RCC_DMA1);

    dma_enable_mem2mem_mode(_dma, _dma_channel);
    dma_set_memory_size(_dma, _dma_channel, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(_dma, _dma_channel, DMA_CCR_PSIZE_16BIT);
    dma_enable_memory_increment_mode(_dma, _dma_channel);
    dma_set_read_from_peripheral(_dma, _dma_channel);
    dma_set_peripheral_address(_dma, _dma_channel, reinterpret_cast<uint32_t>(&ADC_DR(_adc)));
    dma_set_memory_address(_dma, _dma_channel, reinterpret_cast<uint32_t>(&channel_data));
    dma_set_number_of_data(_dma, _dma_channel, 2);
    dma_enable_circular_mode(_dma, _dma_channel);
  }
}

void headphones::enable() {
  if (enabled) return;
  adc_enable_dma(_adc);
  dma_enable_channel(_dma, _dma_channel);

  adc_power_on(_adc);
  systick::sleep_ms(5);
  adc_start_conversion_regular(_adc);
  enabled = true;
}

void headphones::disable() {
  if (!enabled) return;
  adc_power_off(_adc);
  dma_disable_channel(_dma, _dma_channel);
  enabled = false;
}
