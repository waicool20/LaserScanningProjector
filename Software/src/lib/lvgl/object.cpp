#include "object.h"

namespace lvgl {

lv_obj_t* object::get() const { return _handle_; }

bool object::auto_relign() const {
  return lv_obj_get_auto_realign(get());
}

void object::auto_realign(bool enable) const {
  lv_obj_set_auto_realign(get(), enable);
}

void object::align_origin(const lvgl::object* align_base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod) const {
  lv_obj_align_origo(get(), align_base ? align_base->_handle_ : nullptr, align, x_mod, y_mod);
}

void object::pos(lv_coord_t x, lv_coord_t y) const {
  lv_obj_set_pos(get(), x, y);
}

lv_coord_t object::x() const {
  return lv_obj_get_x(get());
}

lv_coord_t object::y() const {
  return lv_obj_get_y(get());
}

void object::size(lv_coord_t w, lv_coord_t h) const {
  lv_obj_set_size(get(), w, h);
}

lv_coord_t object::w() const {
  return lv_obj_get_width(get());
}

lv_coord_t object::h() const {
  return lv_obj_get_height(get());
}

}  // namespace lvgl
