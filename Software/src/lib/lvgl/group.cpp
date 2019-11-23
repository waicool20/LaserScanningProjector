#include "group.h"

namespace lvgl {

lv_group_t* group::get() const {
  return _handle_;
}

void group::add(const lvgl::object& obj) const {
  lv_group_add_obj(_handle_, obj.get());
}

}  // namespace lvgl
