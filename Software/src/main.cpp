#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include "drivers/stepper_motor.h"
#include "lib/rcc.h"
#include "lib/systick.h"

namespace {
void rcc_setup() {
  rcc::periph_clock_enable(RCC_GPIOB);
}

void spi_setup() {
  rcc::periph_clock_enable(RCC_SPI1);
  rcc::periph_clock_enable(RCC_GPIOA);

  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO4 | GPIO5 | GPIO7);
  gpio_set_af(GPIOA, GPIO_AF5, GPIO4 | GPIO5 | GPIO7);

  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_MSBFIRST);

  spi_set_bidirectional_mode(SPI1);
  spi_enable_software_slave_management(SPI1);
//  spi_set_nss_high(SPI1);
  spi_enable_ss_output(SPI1);
  spi_enable(SPI1);
}
}  // namespace

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick::init();

  rcc_setup();

  gpio lcd_reset(GPIOB, GPIO2);
  lcd_reset.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  lcd_reset.state(true);
  systick::sleep_ms(1);
  lcd_reset.state(false);

  gpio lcd_dc(GPIOB, GPIO10);
  lcd_dc.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);

  gpio lcd_cs(GPIOA, GPIO4);
  lcd_cs.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  lcd_cs.state(true);

  spi_setup();

  lcd_dc.state(false);
//  spi_set_nss_low(SPI1);
  lcd_cs.state(false);
  int d = spi_xfer(SPI1, 0xDA);
  int v = spi_xfer(SPI1, 0xDA);
//  spi_set_nss_high(SPI1);
  lcd_cs.state(true);
  lcd_dc.state(true);

  gpio lcd_bk(GPIOB, GPIO11);
  lcd_bk.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  lcd_bk.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ);

  while (true) {
    lcd_bk.toggle();

    systick::sleep_ms(1000);
  }
}
