#ifndef LASERSCANNINGPROJECT_UI_H
#define LASERSCANNINGPROJECT_UI_H

#include <drivers/st7735s.h>
#include <drivers/nav5.h>
#include "../../middleware/lvgl/lvgl.h"
#include "circular_queue.h"

using namespace rtlib::util;

class ui {
  public:
    struct btn_event {
      uint32_t key;
      bool state;
    };
    ui(st7735s *lcd, nav5 *nav5);
    void init();
  private:
    static st7735s *_lcd;
    static nav5 *_nav5;
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf1[LV_HOR_RES_MAX * 10];
    static circular_queue<btn_event, 10> btn_events;

    static void flush_lcd(_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool nav5_read(_lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
};

#endif //LASERSCANNINGPROJECT_UI_H
