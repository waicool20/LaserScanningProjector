#include "main.h"
#include "view_audio.h"
#include "lib/ui.h"
#include "view_main.h"

using namespace std::literals;

bool view_audio::created = false;

void view_audio::init() {
  if (created) return;
  input_group().add(list());
  list().hidden(true);
  list().size(list().screen().w(), list().screen().h());
  auto headphones_btn = list().add_btn("Headphones"sv);
  auto headphones_xy_btn = list().add_btn("Headphones XY"sv);
  auto mic_btn = list().add_btn("Microphone"sv);
  auto back_btn = list().add_btn("Back"sv);

  headphones_btn.add_event_callback(headphones_btn_cb);
  headphones_xy_btn.add_event_callback(headphones_xy_btn_cb);
  mic_btn.add_event_callback(mic_btn_cb);
  back_btn.add_event_callback(back_btn_cb);

  created = true;
}

void view_audio::show(bool show) {
  if (show) {
    init();
    list().hidden(false);
    lv_indev_set_group(ui::get_input_device(), input_group().get());
  } else {
    list().hidden(true);
  }
}

lvgl::list &view_audio::list() {
  static lvgl::list _list;
  return _list;
}

lvgl::group &view_audio::input_group() {
  static lvgl::group _group;
  return _group;
}

void view_audio::headphones_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::AUDIO_HEADPHONES;
      break;
  }
}

void view_audio::headphones_xy_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::AUDIO_HEADPHONES_XY;
      break;
  }
}

void view_audio::mic_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      rendering = render::AUDIO_MIC;
      break;
  }
}


void view_audio::back_btn_cb(lv_obj_t * obj, lv_event_t event) {
  switch(event) {
    case LV_EVENT_PRESSED:
      show(false);
      view_main::show(true);
      rendering = render::NONE;
      break;
  }
}

