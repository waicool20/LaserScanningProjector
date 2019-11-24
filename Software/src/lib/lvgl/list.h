#ifndef LASERSCANNINGPROJECT_LIST_H
#define LASERSCANNINGPROJECT_LIST_H

#include <lvgl.h>
#include <string_view>
#include "object.h"
#include "button.h"

namespace lvgl {

class list : public object {
  public:
    explicit list(lv_obj_t* parent = lv_scr_act(), const list* copy = nullptr);
    button add_btn(void* img, std::string_view txt);
    button add_btn(std::string_view txt) { add_btn(nullptr, txt); }
};

}
#endif //LASERSCANNINGPROJECT_LIST_H
