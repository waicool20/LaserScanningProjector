#include "button.h"

#include <lv_objx/lv_btn.h>

namespace lvgl {

button::button(object& parent, const lvgl::button* copy) :
    object(lv_btn_create(parent.get(), copy ? copy->get() : nullptr)) {}

}  // namespace lvgl
