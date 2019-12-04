#pragma once

#include <optional>

#include "lib/ui.h"
#include "lib/lvgl/container.h"
#include "lib/lvgl/group.h"

/**
 * \brief Manual view user interface.
 */
class view_manual {
 public:
  /**
   * \param show Whether to display/hide the panel.
   */
  static void show(bool show);
 private:
  static void init();
  static bool created;

  static lvgl::container& container();
  static lvgl::group& input_group();

  static void exit_btn_cb(lv_obj_t* obj, lv_event_t event);
};
