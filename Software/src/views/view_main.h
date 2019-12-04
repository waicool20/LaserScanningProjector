#ifndef LASERSCANNINGPROJECT_VIEW_MAIN_H
#define LASERSCANNINGPROJECT_VIEW_MAIN_H

#include "lib/lvgl/list.h"
#include "lib/lvgl/group.h"

/**
 * \brief Main view user interface.
 */
class view_main {
 public:
  /**
   * \param show Whether to display/hide the panel.
   */
  static void show(bool show);
 private:
  static void init();
  static bool created;
  static lvgl::list& list();
  static lvgl::group& input_group();
  static void basic_btn_cb(lv_obj_t* obj, lv_event_t event);
  static void debug_btn_cb(lv_obj_t* obj, lv_event_t event);
  static void audio_btn_cb(lv_obj_t* obj, lv_event_t event);
  static void usb_btn_cb(lv_obj_t* obj, lv_event_t event);
};

#endif //LASERSCANNINGPROJECT_VIEW_MAIN_H
