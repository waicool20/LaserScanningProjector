#pragma once

#include <lv_core/lv_disp.h>
#include <lv_objx/lv_cont.h>

#include "object.h"

namespace lvgl {

/**
 * \brief Abstraction for LVGL container.
 */
class container : public object {
 public:
  explicit container(lv_obj_t* parent = lv_scr_act(), const container* copy = nullptr);

  void fit(lv_fit_t policy) const;
  void fit2(lv_fit_t hz_policy, lv_fit_t vt_policy) const;
  [[nodiscard]] lv_fit_t fit_left() const;
  [[nodiscard]] lv_fit_t fit_right() const;
  [[nodiscard]] lv_fit_t fit_top() const;
  [[nodiscard]] lv_fit_t fit_bottom() const;

  void layout(lv_layout_t layout) const;
  [[nodiscard]] lv_layout_t layout() const;
};

}  // namespace lvgl
