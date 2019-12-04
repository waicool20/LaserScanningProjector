#include "main.h"
#include "view_usb.h"
#include "lib/ui.h"
#include "view_main.h"

using namespace std::literals;

bool view_usb::created = false;

void view_usb::init() {
  if (created) return;
  input_group().add(list());
  list().hidden(true);
  list().size(list().screen().w(), list().screen().h());
  auto back_btn = list().add_btn("Back"sv);

  back_btn.add_event_callback(back_btn_cb);

  created = true;
}

void view_usb::show(bool show) {
  if (show) {
    init();
    list().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    list().hidden(true);
  }
}

lvgl::list &view_usb::list() {
  static lvgl::list _list;
  return _list;
}

lvgl::group &view_usb::input_group() {
  static lvgl::group _group;
  return _group;
}

void view_usb::back_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      show(false);
      view_main::show(true);
      rendering = render::NONE;
      break;
  }
}

