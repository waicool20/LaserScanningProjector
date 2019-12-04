#include "list.h"

lvgl::list::list(lv_obj_t* parent, const lvgl::list* copy) :
    object(lv_list_create(parent, copy ? copy->get() : nullptr)) {}

lvgl::list::list(lv_obj_t* handle) : object(handle) {}

lvgl::button lvgl::list::add_btn(void* img, std::string_view txt) {
  return button{lv_list_add_btn(get(), img, txt.data())};
}
