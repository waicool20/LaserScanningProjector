#ifndef LASERSCANNINGPROJECT_VIEW_BASIC_H
#define LASERSCANNINGPROJECT_VIEW_BASIC_H

#include "lib/lvgl/list.h"
#include "lib/lvgl/group.h"

class view_basic {
  public:
    static void init();
    static void show(bool show);
  private:
    static bool created;
    static lvgl::list& list();
    static lvgl::group& input_group();
    static void rect_btn_cb(lv_obj_t * obj, lv_event_t event);
    static void bitmap_btn_cb(lv_obj_t * obj, lv_event_t event);
    static void tuple_btn_cb(lv_obj_t * obj, lv_event_t event);
    static void back_btn_cb(lv_obj_t * obj, lv_event_t event);
};


#endif //LASERSCANNINGPROJECT_VIEW_BASIC_H
