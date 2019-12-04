#pragma once

#include "object.h"

namespace lvgl {

/**
 * \brief Abstraction for LVGL button.
 */
class button : public object {
 public:
  explicit button(object& parent, const button* copy = nullptr);
  explicit button(lv_obj_t* handle);
};

}  // namespace lvgl
