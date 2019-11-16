#pragma once

#include <libopencm3/stm32/spi.h>
#include "lib/gpio.h"

/**
 * LCD screen with st7735s chip as its driver
 *
 * @see <a href="https://www.crystalfontz.com/controllers/Sitronix/ST7735S/320">Datasheet</a>
 */
class st7735s {
 public:
  enum command : std::uint8_t {
    CMD_NOP = 0x00,
    CMD_SWRESET = 0x01,
    CMD_RDDID = 0x04,
    CMD_RDDST = 0x09,

    CMD_SLPIN = 0x10,
    CMD_SLPOUT = 0x11,
    CMD_PTLON = 0x12,
    CMD_NORON = 0x13,

    CMD_INVOFF = 0x20,
    CMD_INVON = 0x21,
    CMD_GAMSET = 0x26,
    CMD_DISPOFF = 0x28,
    CMD_DISPON = 0x29,
    CMD_CASET = 0x2A,
    CMD_RASET = 0x2B,
    CMD_RAMWR = 0x2C,
    CMD_RAMRD = 0x2E,

    CMD_PTLAR = 0x30,
    CMD_COLMOD = 0x3A,
    CMD_MADCTL = 0x36,

    CMD_FRMCTR1 = 0xB1,
    CMD_FRMCTR2 = 0xB2,
    CMD_FRMCTR3 = 0xB3,
    CMD_INVCTR = 0xB4,
    CMD_DISSET5 = 0xB6,

    CMD_PWCTR1 = 0xC0,
    CMD_PWCTR2 = 0xC1,
    CMD_PWCTR3 = 0xC2,
    CMD_PWCTR4 = 0xC3,
    CMD_PWCTR5 = 0xC4,
    CMD_VMCTR1 = 0xC5,

    CMD_RDID1 = 0xDA,
    CMD_RDID2 = 0xDB,
    CMD_RDID3 = 0xDC,
    CMD_RDID4 = 0xDD,

    CMD_PWCTR6 = 0xFC,

    CMD_GMCTRP1 = 0xE0,
    CMD_GMCTRN1 = 0xE1,
  };

  enum color_mode : std::uint8_t {
    COLOR_MODE_12_BITS = 0b011,
    COLOR_MODE_16_BITS = 0b101,
    COLOR_MODE_18_BITS = 0b110,
  };

  enum refresh_order_v : std::uint8_t {
    REFRESH_ORDER_BOTTOM_TO_TOP = 1,
    REFRESH_ORDER_TOP_TO_BOTTOM = 0,
  };

  enum refresh_order_h : std::uint8_t {
    REFRESH_ORDER_RIGHT_TO_LEFT = 1,
    REFRESH_ORDER_LEFT_TO_RIGHT = 0,
  };

  enum color_order : std::uint8_t {
    COLOR_ORDER_BGR = 1,
    COLOR_ORDER_RGB = 0,
  };

  /**
   * Main constructor for LCD functions
   * @param x x offset where pixels start drawing
   * @param y y offset where pixels start drawing
   * @param w width of lcd in pixels
   * @param h height of lcd in pixels
   * @param cm color bit depth of the lcd (12/16/18)
   */
  st7735s(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, color_mode cm);

  /**
   * Chip select for lcd, primes lcd for writing data
   */
  void select();

  /**
   * Chip unselect for lcd
   */
  void unselect();

  /**
   * Resets lcd by pulling reset pin low then high
   */
  void hw_reset();

  /**
   * Sends reset command to lcd
   */
  void sw_reset();

  /**
   * Sets lcd to exit sleep mode
   */
  void slp_out();

  /**
   * Applies all configurations and enables the display.
   */
  void display_enable();

  /**
   * \brief Sets the state of the backlight.
   *
   * \param state `true` if the backlight should be on.
   */
  void backlight(bool state);

  /**
   * Sets the order in which pixels are drawn, can be used to change orientation
   * however in that case using set_orientation() is recommended
   *
   * @param rov Vertical refresh order
   * @param roh Horizontal refresh order
   * @param co Color order, BGR or RGB
   */
  void set_draw_order(refresh_order_v rov, refresh_order_h roh, color_order co);

  /**
   * Inverts the display
   *
   * @param invert true to invert display
   */
  void set_inverted(bool invert);

  /**
   * Sets the color bit depth
   *
   * @param cm new bit depth
   */
  void set_color_mode(color_mode cm);

  /**
   * Convenience method for changing lcd orientation
   *
   * @param orientation new orientation, valid values are 0-3
   */
  void set_orientation(std::uint8_t orientation);

  /**
   * Sets lcd internal gamma
   *
   * @param gamma new gamma value, valid values are 0-3
   */
  void set_gamma(std::uint8_t gamma);

  /**
   * Sets the display memory window for the consequent data writes
   * @param x1 x coordinate of top left corner of window
   * @param y1 y coordinate of top left corner of window
   * @param x2 x coordinate of bottom right corner of window
   * @param y2 y coordinate of bottom right corner of window
   * @return
   */
  bool set_window(std::uint8_t x1, std::uint8_t y1, std::uint8_t x2, std::uint8_t y2);

  /**
   * Draws a pixel on screen
   * @param x x coordinate
   * @param y y coordinate
   * @param rgb rgb color code of pixel
   */
  void draw_pixel(std::uint8_t x, std::uint8_t y, std::uint32_t rgb) { draw_rect(x, y, x, y, rgb); }

  /**
   * Draws a rectangle on screen
   * @param x x coordinate
   * @param y y coordinate
   * @param w rectangle width
   * @param h rectangle height
   * @param rgb rgb color code of rectangle
   */
  void draw_rect(std::uint8_t x, std::uint8_t y, std::uint8_t w, std::uint8_t h, std::uint32_t rgb);

  /**
   * Fills screen with given color
   *
   * @param rgb rgb color code
   */
  void draw_screen(std::uint32_t rgb);

  /**
   * Send a command packet to lcd
   *
   * @param cmd command to send
   */
  void send_cmd(command cmd);

  /**
   * Send a data packet to lcd
   *
   * @param data data to send
   */
  void send_data(std::uint8_t data);

  /**
   * Get the start x offset
   *
   * @return x_offset
   */
  [[nodiscard]] uint8_t x_offset() const;

  /**
   * Get the start y offset
   *
   * @return y_offset
   */
  [[nodiscard]] uint8_t y_offset() const;

  /**
   * Get the lcd width
   * @return lcd pixel width
   */
  [[nodiscard]] uint8_t width() const;

  /**
   * Get the lcd height
   * @return lcd pixel height
   */
  [[nodiscard]] uint8_t height() const;

 private:
  const std::uint32_t _spi_ = SPI1;

  // Clock (SCK/SCL)
  gpio _sck_ = gpio(GPIOA, GPIO5);
  // Data (MOSI/SDA)
  gpio _mosi_ = gpio(GPIOA, GPIO7);
  // Reset
  gpio _res_ = gpio(GPIOB, GPIO2);
  // Data/Command (Command = 0, Data = 1)
  gpio _dc_ = gpio(GPIOB, GPIO10);
  // Chip Select
  gpio _cs_ = gpio(GPIOA, GPIO4);
  // ~Backlight
  gpio _bl_ = gpio(GPIOB, GPIO11);

  const std::uint8_t _x_offset_;
  const std::uint8_t _y_offset_;
  std::uint8_t _width_;
  std::uint8_t _height_;
};
