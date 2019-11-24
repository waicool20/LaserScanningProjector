#ifndef LASERSCANNINGPROJECT_VIEW_INIT_H
#define LASERSCANNINGPROJECT_VIEW_INIT_H

#include <lib/lvgl/group.h>
#include <lib/lvgl/button.h>
#include <lib/lvgl/label.h>
#include <lib/lvgl/container.h>

class view_init {
  public:
    static void init();
    static void show(bool show);
  private:
    static bool created;

    static void start_btn_cb(lv_obj_t *obj, lv_event_t event);
    static lvgl::container& container();
    static lvgl::label &wait_label();
};


#endif //LASERSCANNINGPROJECT_VIEW_INIT_H
