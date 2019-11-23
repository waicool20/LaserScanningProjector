#include <lvgl.h>
#include "ui_container.h"

ui_container::ui_container() {
  obj = lv_cont_create(lv_scr_act(), NULL);
}
