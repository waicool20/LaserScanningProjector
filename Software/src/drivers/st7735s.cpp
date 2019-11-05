#include "st7735s.h"
#include <lib/systick.h>
#include <libopencm3/stm32/spi.h>
#include <lib/rcc.h>

st7735s::st7735s(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color_mode) :
    x_offset(x), y_offset(y), width(w), height(h) {
  // Init RCC
  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);

  // Init GPIO
  _sck_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _sck_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _sck_.set_af(GPIO_AF5);
  _mosi_.setup(GPIO_MODE_AF, GPIO_PUPD_NONE);
  _mosi_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _mosi_.set_af(GPIO_AF5);
  _res_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _res_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _dc_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _dc_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _cs_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _cs_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);
  _bl_.setup(GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
  _bl_.output_opts(GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ);

  // Init SPI
  spi_set_master_mode(SPI1);
  spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_4);
  spi_set_clock_polarity_0(SPI1);
  spi_set_clock_phase_0(SPI1);
  spi_set_bidirectional_transmit_only_mode(SPI1);
  spi_set_data_size(SPI1, SPI_CR2_DS_8BIT);
  spi_send_msb_first(SPI1);
  /*
   * Set NSS management to software.
   *
   * Note:
   * Setting nss high is very important, even if we are controlling the GPIO
   * ourselves this bit needs to be at least set to 1, otherwise the spi
   * peripheral will not send any data out.
  */
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_enable(SPI1);

  // Init LCD

  unselect();
  hw_reset();

  // LCD Command Init Block
  {
    sw_reset();
    slp_out();
    set_orientation(2);
    set_gamma(2);
    set_color_mode(color_mode);
    display_enable();
  }

  // Blank screen
  draw_screen(0xFFFFFF);

  backlight_on();
}

void st7735s::select() {
  _cs_.state(false);
}

void st7735s::unselect() {
  _cs_.state(true);
}

void st7735s::hw_reset() {
  _res_.state(false);
  systick::sleep_ms(5);
  _res_.state(true);
}

void st7735s::sw_reset() {
  send_cmd(CMD_SWRESET);
  systick::sleep_ms(150);
}

void st7735s::slp_out() {
  send_cmd(CMD_SLPOUT);
  systick::sleep_ms(150);
}

void st7735s::display_enable() {
  send_cmd(CMD_DISPON);
  systick::sleep_ms(150);
}

void st7735s::backlight_on() {
  _bl_.state(true);
}

void st7735s::backlight_off() {
  _bl_.state(false);
}

void st7735s::set_draw_order(uint8_t refresh_order_v, uint8_t refresh_order_h, uint8_t color_order) {
  send_cmd(CMD_MADCTL);
  send_data(refresh_order_v << 4 | refresh_order_h << 2 | color_order << 3);
}

void st7735s::set_inverted(bool invert) {
  send_cmd(invert ? CMD_INVON : CMD_INVOFF);
}

void st7735s::set_color_mode(uint8_t color_mode) {
  send_cmd(CMD_COLMOD);
  send_data(color_mode);
}

void st7735s::set_orientation(uint8_t orientation) {
  uint8_t current_w = width;
  uint8_t current_h = height;
  /* Memory Data Access Control */
  send_cmd(CMD_MADCTL);

  switch (orientation) {
    case 1:
      send_data(0x60); /* MX + MV */
      width = current_h;
      height = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    case 2:
      send_data(0xC0); /* MY + MX */
      width = current_w;
      height = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;

    case 3:
      send_data(0xA0); /* MY + MV */
      width = current_h;
      height = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    default:
      send_data(0x00); /* None */
      width = current_w;
      height = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;
  }
}

void st7735s::set_gamma(uint8_t gamma) {
  switch (gamma) {
    case 1:
      gamma = 2;
      break; /* GS_pin=1: 1.8; GS_pin=0: 2.5 */
    case 2:
      gamma = 4;
      break; /* GS_pin=1: 2.5; GS_pin=0: 2.2 */
    case 3:
      gamma = 8;
      break; /* GS_pin=1: 1.0; GS_pin=0: 1.8 */
    default:
      gamma = 1;
      break; /* GS_pin=1: 2.2; GS_pin=0: 1.0 */
  }

  /* Set built-in gamma */
  send_cmd(CMD_GAMSET);
  send_data(gamma);
}

bool st7735s::set_window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  /* Accept: 0 <= x1 <= x2 < activeDisplay->width */
  if (x2 < x1) return false;
  if (x2 >= width) return false;

  /* Accept: 0 <= y1 <= y2 < activeDisplay->height */
  if (y2 < y1) return false;
  if (y2 >= height) return false;

  /* Set column address */
  send_cmd(CMD_CASET);
  send_data(0);
  send_data(x1 + x_offset);
  send_data(0);
  send_data(x2 + x_offset);

  /* Set row address */
  send_cmd(CMD_RASET);
  send_data(0);
  send_data(y1 + y_offset);
  send_data(0);
  send_data(y2 + y_offset);

  /* Activate RAM write */
  send_cmd(CMD_RAMWR);

  return true;
}

void st7735s::draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t rgb) {
  /* Draw only in the display space */
  if ((w == 0) || (h == 0)) return;
  if ((x + w - 1) >= width) w = width - x;
  if ((y + h - 1) >= height) h = height - y;

  /* Draw the filed rectangle */
  if (!set_window(x, y, x + w - 1, y + h - 1)) return;
  while (w--)
    while (h--) {
      send_data(rgb >> 16 & 0xFF);
      send_data(rgb >> 8 & 0xFF);
      send_data(rgb & 0xFF);
    }
}

void st7735s::draw_screen(uint32_t rgb) {
  draw_rect(0, 0, width, height, rgb);
}

void st7735s::send_cmd(uint8_t cmd) {
  _dc_.state(false);
  select();
  spi_send8(SPI1, cmd);
  unselect();
}

void st7735s::send_data(uint8_t data) {
  _dc_.state(true);
  select();
  spi_send8(SPI1, data);
  unselect();
}

uint8_t st7735s::get_x_offset() const {
  return x_offset;
}

uint8_t st7735s::get_y_offset() const {
  return y_offset;
}

uint8_t st7735s::get_width() const {
  return width;
}

uint8_t st7735s::get_height() const {
  return height;
}
