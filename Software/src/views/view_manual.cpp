#include "views/view_manual.h"

#include "main.h"
#include "lib/ui.h"
#include "lib/lvgl/button.h"
#include "lib/lvgl/label.h"
#include "views/view_main.h"

bool view_manual::created = false;

void view_manual::show(bool show) {
  if (show) {
    init();
    container().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    container().hidden(true);
  }
}

void view_manual::init() {
  if (created) {
    return;
  }

  container().hidden(true);
  container().fit(LV_FIT_FLOOD);
  container().layout(LV_LAYOUT_PRETTY);

  auto btn = lvgl::button(container());
  btn.size(120, 50);
  btn.add_event_callback(exit_btn_cb);
  btn.align_origin(nullptr, LV_ALIGN_CENTER, 0, 0);

  auto text = lvgl::label(btn);
  text.size(120, 50);
  text.text("Exit");

  container().hidden(false);

  input_group().add(btn);

  created = true;
}

lvgl::container& view_manual::container() {
  static lvgl::container _container;
  return _container;
}

lvgl::group& view_manual::input_group() {
  static lvgl::group group;
  return group;
}

void view_manual::exit_btn_cb(lv_obj_t*, lv_event_t event) {
  if (event == LV_EVENT_PRESSED) {
    rendering = render::NONE;
    view_manual::show(false);
    view_main::show(true);
  }
}
