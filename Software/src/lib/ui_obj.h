#ifndef LASERSCANNINGPROJECT_UI_OBJ_H
#define LASERSCANNINGPROJECT_UI_OBJ_H


#include <src/lv_core/lv_obj.h>

class ui_obj {
  public:
    bool get_auto_realign();
    void set_auto_realign(bool state);
    void set_align_origo(ui_obj* base, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod);
    lv_obj_t* obj = nullptr;
};


#endif //LASERSCANNINGPROJECT_UI_OBJ_H
