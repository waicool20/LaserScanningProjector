#ifndef LASERSCANNINGPROJECT_UI_H
#define LASERSCANNINGPROJECT_UI_H

#include <drivers/st7735s.h>
#include "../../middleware/lvgl/lvgl.h"



class ui {
  public:
    ui(st7735s* lcd) { _lcd = lcd; };
    void init();
  private:
    static st7735s *_lcd;
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf1[LV_HOR_RES_MAX * 10];

    static void flush_lcd(struct _disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool nav5_read(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
};

#endif //LASERSCANNINGPROJECT_UI_H
