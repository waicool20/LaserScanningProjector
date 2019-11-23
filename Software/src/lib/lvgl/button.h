#pragma once

#include "object.h"

namespace lvgl {

class button : public object {
 public:
  explicit button(object& parent, const button* copy = nullptr);
};

}  // namespace lvgl
