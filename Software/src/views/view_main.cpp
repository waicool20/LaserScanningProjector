#include "main.h"
#include "view_main.h"
#include "view_basic.h"
#include "lib/ui.h"
#include "lib/lvgl/button.h"
#include "lib/lvgl/group.h"
#include "lib/lvgl/label.h"
#include "lib/lvgl/list.h"
#include "view_audio.h"
#include "view_debug.h"
#include "view_usb.h"

using namespace std::literals;

bool view_main::created = false;

void view_main::init() {
  if (created) { return; }
  input_group().add(list());
  list().hidden(true);
  list().size(list().screen().w(), list().screen().h());
  auto basic_btn = list().add_btn("Basic"sv);
  auto usb_btn = list().add_btn("USB"sv);
  auto audio_btn = list().add_btn("Audio"sv);
  auto debug_btn = list().add_btn("Debug"sv);

  basic_btn.add_event_callback(basic_btn_cb);
  audio_btn.add_event_callback(audio_btn_cb);
  debug_btn.add_event_callback(debug_btn_cb);
  usb_btn.add_event_callback(usb_btn_cb);
  created = true;
}

void view_main::show(bool show) {
  if (show) {
    init();
    list().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    list().hidden(true);
  }
}

lvgl::list& view_main::list() {
  static lvgl::list _list;
  return _list;
}

lvgl::group& view_main::input_group() {
  static lvgl::group _group;
  return _group;
}

void view_main::basic_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    view_basic::show(true);
  }
}

void view_main::audio_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    view_audio::show(true);
  }
}

void view_main::debug_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    view_debug::show(true);
  }
}

void view_main::usb_btn_cb(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    show(false);
    view_usb::show(true);
    rendering = render::USB;
  }
}
