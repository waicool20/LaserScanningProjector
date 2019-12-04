#include "label.h"

#include <lv_objx/lv_label.h>

namespace lvgl {

label::label(lvgl::object& parent, const lvgl::label* copy) :
    object(lv_label_create(parent.get(), copy ? copy->get() : nullptr)) {}

label::label(lv_obj_t* handle) : object(handle) {}

void label::text(std::string_view sv) const {
  lv_label_set_text(get(), sv.data());
}

char* label::text() const {
  return lv_label_get_text(get());
}

lv_label_align_t label::text_align() {
  return lv_label_get_align(get());
}

void label::text_align(lv_label_align_t align) {
  lv_label_set_align(get(), align);
}

}  // namespace lvgl
