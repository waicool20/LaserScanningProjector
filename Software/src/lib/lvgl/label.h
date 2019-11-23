#pragma once

#include <string_view>

#include "object.h"

namespace lvgl {

class label : public object {
 public:
  explicit label(object& parent, const label* copy = nullptr);

  void text(std::string_view sv) const;
  [[nodiscard]] char* text() const;
};

}  // namespace lvgl
