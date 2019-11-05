#pragma once

#include <libopencm3/stm32/spi.h>
#include "lib/gpio.h"

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

static const uint8_t COLOR_MODE_12_BITS = 0b011;
static const uint8_t COLOR_MODE_16_BITS = 0b101;
static const uint8_t COLOR_MODE_18_BITS = 0b110;

static const uint8_t REFRESH_ORDER_BOTTOM_TO_TOP = 1;
static const uint8_t REFRESH_ORDER_TOP_TO_BOTTOM = 0;

static const uint8_t REFRESH_ORDER_RIGHT_TO_LEFT = 1;
static const uint8_t REFRESH_ORDER_LEFT_TO_RIGHT = 0;

static const uint8_t COLOR_ORDER_BGR = 1;
static const uint8_t COLOR_ORDER_RGB = 0;

class st7735s {
  public:
    st7735s(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color_mode);

    void select();
    void unselect();
    void hw_reset();
    void sw_reset();
    void slp_out();
    void display_enable();

    void backlight_on();
    void backlight_off();

    void set_draw_order(uint8_t refresh_order_v, uint8_t refresh_order_h, uint8_t color_order);
    void set_inverted(bool invert);
    void set_color_mode(uint8_t color_mode);
    void set_orientation(uint8_t orientation);
    void set_gamma(uint8_t gamma);
    bool set_window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t rgb);
    void draw_screen(uint32_t rgb);

    void send_cmd(uint8_t cmd);
    void send_data(uint8_t data);

    uint8_t get_x_offset() const;
    uint8_t get_y_offset() const;
    uint8_t get_width() const;
    uint8_t get_height() const;
  private:
    gpio _sck_ = gpio(GPIOA, GPIO5);
    gpio _mosi_ = gpio(GPIOA, GPIO7);
    gpio _res_ = gpio(GPIOB, GPIO2);
    gpio _dc_ = gpio(GPIOB, GPIO10);
    gpio _cs_ = gpio(GPIOA, GPIO4);
    gpio _bl_ = gpio(GPIOB, GPIO11);

    uint8_t x_offset;
    uint8_t y_offset;
    uint8_t width;
    uint8_t height;
};
