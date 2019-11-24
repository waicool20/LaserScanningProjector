#pragma once

#include "object.h"

namespace lvgl {

class button : public object {
 public:
  explicit button(object& parent, const button* copy = nullptr);
  explicit button(lv_obj_t* handle);
};

}  // namespace lvgl
