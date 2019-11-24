#include <lib/lvgl/group.h>
#include <lib/ui.h>
#include <main.h>
#include "view_init.h"

using namespace std::literals;

bool view_init::created = false;

void view_init::init() {
  if (created) return;

  container().hidden(true);
  container().fit(LV_FIT_FLOOD);
  container().layout(LV_LAYOUT_PRETTY);

  auto banner = lvgl::label(container());
  banner.size(50, 20);
  banner.text("ELEC 3300 LSP");
  banner.text_align(LV_LABEL_ALIGN_CENTER);
  banner.align_origin(nullptr, LV_ALIGN_CENTER, 0, -50);

  auto btn = lvgl::button(container());
  btn.add_event_callback(start_btn_cb);
  btn.align_origin(nullptr, LV_ALIGN_CENTER, 0, 20);

  auto btn_label = lvgl::label(btn);
  btn_label.text("Press to start\nhoming laser!"sv);
  btn_label.text_align(LV_LABEL_ALIGN_CENTER);

  container().hidden(false);

  auto input_group = lvgl::group();
  input_group.add(btn);
  lv_indev_set_group(ui::get_input_device(), input_group.get());
  created = true;
}

void view_init::show(bool show) {
  if (show) {
    init();
  } else {
    wait_label().del();
  }
}

lvgl::container &view_init::container() {
  static lvgl::container _container;
  return _container;
}

lvgl::label &view_init::wait_label() {
  auto scr = lvgl::active_screen();
  static lvgl::label _label(scr);
  return _label;
}

void view_init::start_btn_cb(lv_obj_t *obj, lv_event_t event) {
  switch (event) {
    case LV_EVENT_PRESSED:
      rendering = render::INIT_HOME_LASER;
      container().del();
      wait_label().text("Homing laser\nplease wait!");
      wait_label().text_align(LV_LABEL_ALIGN_CENTER);
      wait_label().align_origin(nullptr, LV_ALIGN_CENTER, 0, 0);
      break;
  }
}
