#include "view_basic.h"
#include "view_main.h"
#include "main.h"
#include "lib/ui.h"

using namespace std::literals;

bool view_basic::created = false;

void view_basic::init() {
  if (created) return;
  input_group().add(list());
  list().hidden(true);
  list().size(list().screen().w(), list().screen().h());
  auto rect_btn = list().add_btn("Rectangle"sv);
  auto bitmap_btn = list().add_btn("Bitmap"sv);
  auto tuple_btn = list().add_btn("Tuple"sv);
  auto back_btn = list().add_btn("Back"sv);

  rect_btn.add_event_callback(rect_btn_cb);
  bitmap_btn.add_event_callback(bitmap_btn_cb);
  tuple_btn.add_event_callback(tuple_btn_cb);
  rect_btn.add_event_callback(rect_btn_cb);

  back_btn.add_event_callback(back_btn_cb);
  created = true;
}

void view_basic::show(bool show) {
  if (show) {
    init();
    list().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    list().hidden(true);
  }
}

lvgl::list &view_basic::list() {
  static lvgl::list _list;
  return _list;
}

lvgl::group &view_basic::input_group() {
  static lvgl::group _group;
  return _group;
}

void view_basic::rect_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::BASIC_RECT;
      break;
  }
}

void view_basic::bitmap_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::BASIC_BITMAP;
      break;
  }
}

void view_basic::tuple_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::BASIC_TUPLE;
      break;
  }
}

void view_basic::back_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::NONE;
      show(false);
      view_main::show(true);
      break;
  }
}
