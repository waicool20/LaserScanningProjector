#include "lib/lvgl/container.h"

namespace lvgl {

container::container(lv_obj_t* parent, const container* copy)
    : object(lv_cont_create(parent, copy ? copy->get() : nullptr)) {}

void container::fit2(lv_fit_t hz_policy, lv_fit_t vt_policy) const {
  lv_cont_set_fit2(get(), hz_policy, vt_policy);
}

lv_fit_t container::fit_bottom() const {
  return lv_cont_get_fit_bottom(get());
}

lv_fit_t container::fit_top() const {
  return lv_cont_get_fit_top(get());
}

lv_fit_t container::fit_left() const {
  return lv_cont_get_fit_left(get());
}

lv_fit_t container::fit_right() const {
  return lv_cont_get_fit_right(get());
}

void container::layout(lv_layout_t layout) const {
  lv_cont_set_layout(get(), layout);
}

lv_layout_t container::layout() const {
  return lv_cont_get_layout(get());
}

}  // namespace lvgl

