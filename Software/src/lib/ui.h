#ifndef LASERSCANNINGPROJECT_UI_H
#define LASERSCANNINGPROJECT_UI_H

#include <drivers/st7735s.h>
#include <drivers/nav5.h>
#include <lvgl.h>
#include "circular_queue.h"

using namespace rtlib::util;

/**
 * \brief Abstraction layer for LVGL user interface.
 */
class ui {
 public:
  /**
   * \brief Button event.
   */
  struct btn_event {
    /**
     * \brief Key which triggered the event.
     */
    uint32_t key;
    /**
     * \brief New state of the key.
     */
    bool state;
  };
  /**
   * \brief Initializes the user interface.
   *
   * \param lcd pointer to the initialized LCD
   * \param nav5 pointer to the initialized 5-way nav switch
   */
  static void init(st7735s* lcd, nav5* nav5);

  static st7735s* get_lcd() { return _lcd; }
  static nav5* get_nav5() { return _nav5; }
  /**
   * \return LVGL handle of the input device (i.e. nav5).
   */
  static lv_indev_t* get_input_device() { return _input_device; }
  /**
   * \brief LVGL handle of the display (i.e. ST7735S)
   * \return
   */
  static lv_disp_t* get_display() { return _display; }

 private:
  ui() {};
  static st7735s* _lcd;
  static nav5* _nav5;
  static lv_indev_t* _input_device;
  static lv_disp_t* _display;

  static lv_disp_buf_t disp_buf;
  static lv_color_t buf1[LV_HOR_RES_MAX * 10];
  static circular_queue<btn_event, 10> btn_events;

  static void flush_lcd(_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
  static bool nav5_read(_lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
};

#endif //LASERSCANNINGPROJECT_UI_H
