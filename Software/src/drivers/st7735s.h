#pragma once

#include <libopencm3/stm32/spi.h>
#include "lib/gpio.h"

//<editor-fold desc="LCD Commands">

static const uint8_t CMD_NOP = 0x00;
static const uint8_t CMD_SWRESET = 0x01;
static const uint8_t CMD_RDDID = 0x04;
static const uint8_t CMD_RDDST = 0x09;

static const uint8_t CMD_SLPIN = 0x10;
static const uint8_t CMD_SLPOUT = 0x11;
static const uint8_t CMD_PTLON = 0x12;
static const uint8_t CMD_NORON = 0x13;

static const uint8_t CMD_INVOFF = 0x20;
static const uint8_t CMD_INVON = 0x21;
static const uint8_t CMD_GAMSET = 0x26;
static const uint8_t CMD_DISPOFF = 0x28;
static const uint8_t CMD_DISPON = 0x29;
static const uint8_t CMD_CASET = 0x2A;
static const uint8_t CMD_RASET = 0x2B;
static const uint8_t CMD_RAMWR = 0x2C;
static const uint8_t CMD_RAMRD = 0x2E;

static const uint8_t CMD_PTLAR = 0x30;
static const uint8_t CMD_COLMOD = 0x3A;
static const uint8_t CMD_MADCTL = 0x36;

static const uint8_t CMD_FRMCTR1 = 0xB1;
static const uint8_t CMD_FRMCTR2 = 0xB2;
static const uint8_t CMD_FRMCTR3 = 0xB3;
static const uint8_t CMD_INVCTR = 0xB4;
static const uint8_t CMD_DISSET5 = 0xB6;

static const uint8_t CMD_PWCTR1 = 0xC0;
static const uint8_t CMD_PWCTR2 = 0xC1;
static const uint8_t CMD_PWCTR3 = 0xC2;
static const uint8_t CMD_PWCTR4 = 0xC3;
static const uint8_t CMD_PWCTR5 = 0xC4;
static const uint8_t CMD_VMCTR1 = 0xC5;

static const uint8_t CMD_RDID1 = 0xDA;
static const uint8_t CMD_RDID2 = 0xDB;
static const uint8_t CMD_RDID3 = 0xDC;
static const uint8_t CMD_RDID4 = 0xDD;

static const uint8_t CMD_PWCTR6 = 0xFC;

static const uint8_t CMD_GMCTRP1 = 0xE0;
static const uint8_t CMD_GMCTRN1 = 0xE1;

//</editor-fold>

//<editor-fold desc="LCD Command Parameters">

static const uint8_t COLOR_MODE_12_BITS = 0b011;
static const uint8_t COLOR_MODE_16_BITS = 0b101;
static const uint8_t COLOR_MODE_18_BITS = 0b110;

static const uint8_t REFRESH_ORDER_BOTTOM_TO_TOP = 1;
static const uint8_t REFRESH_ORDER_TOP_TO_BOTTOM = 0;

static const uint8_t REFRESH_ORDER_RIGHT_TO_LEFT = 1;
static const uint8_t REFRESH_ORDER_LEFT_TO_RIGHT = 0;

static const uint8_t COLOR_ORDER_BGR = 1;
static const uint8_t COLOR_ORDER_RGB = 0;

//</editor-fold>

/**
 * LCD screen with st7735s chip as its driver
 *
 * @see <a href="https://www.crystalfontz.com/controllers/Sitronix/ST7735S/320">Datasheet</a>
 */
class st7735s {
  public:
    /**
     * Main constructor for LCD functions
     * @param x x offset where pixels start drawing
     * @param y y offset where pixels start drawing
     * @param w width of lcd in pixels
     * @param h height of lcd in pixels
     * @param color_mode color bit depth of the lcd (12/16/18)
     */
    st7735s(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color_mode);

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
     * Turns on the backlight
     */
    void backlight_on();

    /**
     * Turns off the backlight
     */
    void backlight_off();

    /**
     * Sets the order in which pixels are drawn, can be used to change orientation
     * however in that case using set_orientation() is recommended
     *
     * @param refresh_order_v Vertical refresh order
     * @param refresh_order_h Horizontal refresh order
     * @param color_order Color order, BGR or RGB
     */
    void set_draw_order(uint8_t refresh_order_v, uint8_t refresh_order_h, uint8_t color_order);

    /**
     * Inverts the display
     *
     * @param invert true to invert display
     */
    void set_inverted(bool invert);

    /**
     * Sets the color bit depth
     *
     * @param color_mode new bit depth
     */
    void set_color_mode(uint8_t color_mode);

    /**
     * Convenience method for changing lcd orientation
     *
     * @param orientation new orientation, valid values are 0-3
     */
    void set_orientation(uint8_t orientation);

    /**
     * Sets lcd internal gamma
     *
     * @param gamma new gamma value, valid values are 0-3
     */
    void set_gamma(uint8_t gamma);

    /**
     * Sets the display memory window for the consequent data writes
     * @param x1 x coordinate of top left corner of window
     * @param y1 y coordinate of top left corner of window
     * @param x2 x coordinate of bottom right corner of window
     * @param y2 y coordinate of bottom right corner of window
     * @return
     */
    bool set_window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    /**
     * Draws a pixel on screen
     * @param x x coordinate
     * @param y y coordinate
     * @param rgb rgb color code of pixel
     */
    void draw_pixel(uint8_t x, uint8_t y, uint32_t rgb) { draw_rect(x, y, x, y, rgb); }

    /**
     * Draws a rectangle on screen
     * @param x x coordinate
     * @param y y coordinate
     * @param w rectangle width
     * @param h rectangle height
     * @param rgb rgb color code of rectangle
     */
    void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t rgb);

    /**
     * Fills screen with given color
     *
     * @param rgb rgb color code
     */
    void draw_screen(uint32_t rgb);

    /**
     * Send a command packet to lcd
     *
     * @param cmd command to send
     */
    void send_cmd(uint8_t cmd);

    /**
     * Send a data packet to lcd
     *
     * @param data data to send
     */
    void send_data(uint8_t data);

    /**
     * Get the start x offset
     *
     * @return x_offset
     */
    uint8_t get_x_offset() const;

    /**
     * Get the start y offset
     *
     * @return y_offset
     */
    uint8_t get_y_offset() const;

    /**
     * Get the lcd width
     * @return lcd pixel width
     */
    uint8_t get_width() const;

    /**
     * Get the lcd height
     * @return lcd pixel height
     */
    uint8_t get_height() const;
  private:
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

    uint8_t x_offset;
    uint8_t y_offset;
    uint8_t width;
    uint8_t height;
};
