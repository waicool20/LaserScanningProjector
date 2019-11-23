#include "ui_obj.h"

bool ui_obj::get_auto_realign() {
  return lv_obj_get_auto_realign(obj);
}

void ui_obj::set_auto_realign(bool state) {
  lv_obj_set_auto_realign(obj, state);
}

void ui_obj::set_align_origo(ui_obj *base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod) {
  lv_obj_align_origo(obj, base->obj, align, x_mod, y_mod);
}
