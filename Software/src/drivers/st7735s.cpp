#include "st7735s.h"
#include <lib/systick.h>
#include <libopencm3/stm32/spi.h>
#include <lib/rcc.h>

st7735s::st7735s(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, color_mode cm) :
    _x_offset_(x), _y_offset_(y), _width_(w), _height_(h) {
  // Init RCC
  rcc::periph_clock_enable(RCC_SPI1);
  rcc::periph_clock_enable(RCC_GPIOA);
  rcc::periph_clock_enable(RCC_GPIOB);

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
  spi_set_master_mode(_spi_);
  // 18MBit/s with 72MHz clock
  spi_set_baudrate_prescaler(_spi_, SPI_CR1_BR_FPCLK_DIV_4);
  spi_set_clock_polarity_0(_spi_);
  spi_set_clock_phase_0(_spi_);
  spi_set_bidirectional_transmit_only_mode(_spi_);
  spi_set_data_size(_spi_, SPI_CR2_DS_8BIT);
  spi_send_msb_first(_spi_);
  /*
   * Set NSS management to software.
   *
   * Note:
   * Setting nss high is very important, even if we are controlling the GPIO
   * ourselves this bit needs to be at least set to 1, otherwise the spi
   * peripheral will not send any data out.
  */
  spi_enable_software_slave_management(_spi_);
  spi_set_nss_high(_spi_);
  spi_enable(_spi_);

  // Init LCD

  unselect();
  hw_reset();

  // LCD Command Init Block
  {
    sw_reset();
    slp_out();
    set_orientation(2);
    set_gamma(2);
    set_color_mode(cm);
    display_enable();
  }

  // Blank screen
  draw_screen(0xFFFFFF);

  backlight(true);
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

void st7735s::backlight(bool state) {
  _bl_.state(state);
}

void st7735s::set_draw_order(refresh_order_v rov, refresh_order_h roh, color_order co) {
  send_cmd(CMD_MADCTL);
  send_data(rov << 4 | roh << 2 | co << 3);
}

void st7735s::set_inverted(bool invert) {
  send_cmd(invert ? CMD_INVON : CMD_INVOFF);
}

void st7735s::set_color_mode(color_mode cm) {
  send_cmd(CMD_COLMOD);
  send_data(cm);
}

void st7735s::set_orientation(std::uint8_t orientation) {
  uint8_t current_w = _width_;
  uint8_t current_h = _height_;
  send_cmd(CMD_MADCTL);

  switch (orientation) {
    case 1:
      send_data(0x60); /* MX + MV */
      _width_ = current_h;
      _height_ = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    case 2:
      send_data(0xC0); /* MY + MX */
      _width_ = current_w;
      _height_ = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;

    case 3:
      send_data(0xA0); /* MY + MV */
      _width_ = current_h;
      _height_ = current_w;
      set_window(0, 0, current_h - 1, current_w - 1);
      break;

    default:
      send_data(0x00); /* None */
      _width_ = current_w;
      _height_ = current_h;
      set_window(0, 0, current_w - 1, current_h - 1);
      break;
  }
}

void st7735s::set_gamma(std::uint8_t gamma) {
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

  send_cmd(CMD_GAMSET);
  send_data(gamma);
}

bool st7735s::set_window(std::uint8_t x1, std::uint8_t y1, std::uint8_t x2, std::uint8_t y2) {
  /* Accept: 0 <= x1 <= x2 < width */
  if (x2 < x1) { return false; }
  if (x2 >= _width_) { return false; }

  /* Accept: 0 <= y1 <= y2 < height */
  if (y2 < y1) { return false; }
  if (y2 >= _height_) { return false; }

  /* Set column address */
  send_cmd(CMD_CASET);
  send_data(0);
  send_data(x1 + _x_offset_);
  send_data(0);
  send_data(x2 + _x_offset_);

  /* Set row address */
  send_cmd(CMD_RASET);
  send_data(0);
  send_data(y1 + _y_offset_);
  send_data(0);
  send_data(y2 + _y_offset_);

  /* Activate RAM write */
  send_cmd(CMD_RAMWR);
  return true;
}

void st7735s::draw_rect(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, std::uint32_t rgb) {
  /* Draw only in the display space */
  if ((w == 0) || (h == 0)) { return; }
  if ((x + w - 1) >= _width_) { w = _width_ - x; }
  if ((y + h - 1) >= _height_) { h = _height_ - y; }

  /* Draw the filed rectangle */
  if (!set_window(x, y, x + w - 1, y + h - 1)) { return; }
  while (w--) {
    while (h--) {
      send_data(rgb >> 16 & 0xFF);
      send_data(rgb >> 8 & 0xFF);
      send_data(rgb & 0xFF);
    }
  }
}

void st7735s::draw_screen(std::uint32_t rgb) {
  draw_rect(0, 0, _width_, _height_, rgb);
}

void st7735s::send_cmd(command cmd) {
  _dc_.state(false);
  select();
  spi_send8(_spi_, cmd);
  unselect();
}

void st7735s::send_data(std::uint8_t data) {
  _dc_.state(true);
  select();
  spi_send8(_spi_, data);
  unselect();
}

uint8_t st7735s::x_offset() const {
  return _x_offset_;
}

uint8_t st7735s::y_offset() const {
  return _y_offset_;
}

uint8_t st7735s::width() const {
  return _width_;
}

uint8_t st7735s::height() const {
  return _height_;
}
