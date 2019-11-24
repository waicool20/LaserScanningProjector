#ifndef LASERSCANNINGPROJECT_VIEW_MAIN_H
#define LASERSCANNINGPROJECT_VIEW_MAIN_H

#include "lib/lvgl/list.h"
#include "lib/lvgl/group.h"

class view_main {
  public:
    static void init();
    static void show(bool show);

  private:
    static bool created;
    static lvgl::list& list();
    static lvgl::group& input_group();
    static void basic_btn_cb(lv_obj_t * obj, lv_event_t event);
};


#endif //LASERSCANNINGPROJECT_VIEW_MAIN_H
