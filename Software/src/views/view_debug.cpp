#include "view_debug.h"
#include "lib/ui.h"
#include "main.h"
#include "view_main.h"
#include "view_manual.h"

using namespace std::literals;

bool view_debug::created = false;

void view_debug::init() {
  if (created) { return; }
  input_group().add(list());
  list().hidden(true);
  list().size(list().screen().w(), list().screen().h());
  auto home_laser_btn = list().add_btn("Home Laser"sv);
  auto manual_btn = list().add_btn("Manual"sv);
  auto back_btn = list().add_btn("Back"sv);

  home_laser_btn.add_event_callback(home_laser_btn_cb);
  manual_btn.add_event_callback(manual_btn_cb);
  back_btn.add_event_callback(back_btn_cb);
  created = true;
}

void view_debug::show(bool show) {
  if (show) {
    init();
    list().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    list().hidden(true);
  }
}

lvgl::list& view_debug::list() {
  static lvgl::list _list;
  return _list;
}

lvgl::group& view_debug::input_group() {
  static lvgl::group _group;
  return _group;
}

void view_debug::home_laser_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    rendering = render::DEBUG_HOME_LASER;
  }
}

void view_debug::manual_btn_cb(lv_obj_t*, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    rendering = render::DEBUG_MANUAL;
    view_manual::show(true);
  }
}

void view_debug::back_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    view_main::show(true);
  }
}
