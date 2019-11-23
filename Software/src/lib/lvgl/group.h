#pragma once

#include <lv_core/lv_group.h>

#include "object.h"

namespace lvgl {

class group {
 public:
  group() = default;

  [[nodiscard]] lv_group_t* get() const;

  void add(const object& obj) const;

 private:
  lv_group_t* const _handle_ = lv_group_create();
};

}  // namespace lvgl
