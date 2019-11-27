#include "lib/rcc.h"
#include "mic.h"
#include "lib/systick.h"

uint32_t mic::_adc  = ADC3;
uint8_t  mic::_adc_channels[1] = { 1 };
gpio mic::_mic = gpio{GPIOB, GPIO1};

volatile uint32_t mic::latest_value = 0;

extern "C" void adc3_isr() {
  mic::latest_value = adc_read_regular(mic::_adc);
}

mic::mic() {
  rcc::periph_clock_enable(RCC_ADC12);
  rcc::periph_clock_enable(RCC_ADC34);
  rcc::periph_clock_enable(RCC_GPIOB);

  _mic.setup(GPIO_MODE_ANALOG, GPIO_PUPD_NONE);

  // ADC
  adc_enable_regulator(_adc);
  systick::sleep_ms(5);

  adc_power_off(_adc);
  adc_set_clk_prescale(_adc, ADC_CCR_CKMODE_DIV2);
  adc_set_continuous_conversion_mode(_adc);
  adc_disable_external_trigger_regular(_adc);
  adc_set_right_aligned(_adc);
  adc_set_sample_time_on_all_channels(_adc, ADC_SMPR_SMP_61DOT5CYC);
  adc_set_regular_sequence(_adc, 1, _adc_channels);
  adc_set_resolution(_adc, ADC_CFGR1_RES_12_BIT);
  adc_disable_analog_watchdog_regular(_adc);
  adc_enable_eoc_interrupt(_adc);
  nvic_enable_irq(NVIC_ADC3_IRQ);
}

float mic::get_latest_value() {
  return latest_value / float(4096) - 1;
}

void mic::enable() {
  if (enabled) return;
  adc_power_on(_adc);
  systick::sleep_ms(5);
  adc_start_conversion_regular(_adc);
  enabled = true;
}

void mic::disable() {
  if (!enabled) return;
  adc_power_off(_adc);
  enabled = false;
}
