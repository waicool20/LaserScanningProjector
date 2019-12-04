#pragma once

#include <string_view>

#include "object.h"

namespace lvgl {

/**
 * \brief Abstraction for LVGL label.
 */
class label : public object {
 public:
  explicit label(object& parent, const label* copy = nullptr);
  explicit label(lv_obj_t* handle);

  void text(std::string_view sv) const;
  [[nodiscard]] char* text() const;

  lv_label_align_t text_align();
  void text_align(lv_label_align_t align);
};

}  // namespace lvgl
