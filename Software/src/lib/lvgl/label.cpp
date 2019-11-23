#include "label.h"

#include <lv_objx/lv_label.h>

namespace lvgl {

label::label(lvgl::object& parent, const lvgl::label* copy) :
    object(lv_label_create(parent.get(), copy ? copy->get() : nullptr)) {}

void label::text(std::string_view sv) const {
  lv_label_set_text(get(), sv.data());
}

char* label::text() const {
  return lv_label_get_text(get());
}

}  // namespace lvgl
