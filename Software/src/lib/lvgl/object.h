#pragma once

#include <lvgl.h>

namespace lvgl {

/**
 * \brief Abstraction for LVGL screen.
 */
class object {
 public:
  explicit object(lv_obj_t* handle) : _handle_(handle) {}
  [[nodiscard]] lv_obj_t* get() const;

  [[nodiscard]] bool auto_relign() const;
  void auto_realign(bool enable) const;

  void align_origin(const object* align_base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod) const;

  void pos(lv_coord_t x, lv_coord_t y) const;
  [[nodiscard]] lv_coord_t x() const;
  [[nodiscard]] lv_coord_t y() const;

  void size(lv_coord_t w, lv_coord_t h) const;
  [[nodiscard]] lv_coord_t w() const;
  [[nodiscard]] lv_coord_t h() const;

  void parent(object parent) const;
  object parent() const;
  object screen() const;

  bool hidden() const;
  void hidden(bool hide) const;
  void del();
  void add_event_callback(lv_event_cb_t event);
 private:
  lv_obj_t* const _handle_;
};

static object active_screen() { return object(lv_scr_act()); }

}  // namespace lvgl
