#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include "ui.h"
#include "rcc.h"
#include <lvgl.h>
#include <lv_core/lv_style.h>

st7735s *ui::_lcd = nullptr;
nav5 *ui::_nav5 = nullptr;
lv_indev_t *ui::_input_device = nullptr;
lv_disp_t *ui::_display = nullptr;
lv_disp_buf_t ui::disp_buf = lv_disp_buf_t{};
lv_color_t ui::buf1[LV_HOR_RES_MAX * 10];
circular_queue<ui::btn_event, 10> ui::btn_events{};

extern "C" void tim6_dac_isr() {
  if (timer_get_flag(TIM6, TIM_SR_UIF)) {
    timer_clear_flag(TIM6, TIM_SR_UIF);
    lv_tick_inc(5);
  }
}

void ui::init(st7735s *lcd, nav5 *nav5) {
  _lcd = lcd;
  _nav5 = nav5;

  // Init Timer, 5ms ticks
  {
    rcc::periph_clock_enable(RCC_TIM6);
    nvic_enable_irq(NVIC_TIM6_DAC_IRQ);
    rcc::periph_reset_pulse(RST_TIM6);

    timer_set_mode(TIM6, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM6, 72000);
    timer_disable_preload(TIM6);
    timer_continuous_mode(TIM6);
    timer_set_period(TIM6, 25);
    timer_update_on_overflow(TIM6);
    timer_enable_update_event(TIM6);
    timer_enable_counter(TIM6);
    timer_enable_irq(TIM6, TIM_DIER_UIE);
  }

  // Init GPIO
  {
    _nav5->on_up([](bool state) {
      btn_events.push({LV_KEY_UP, state});
    });
    _nav5->on_down([](bool state) {
      btn_events.push({LV_KEY_DOWN, state});
    });
    _nav5->on_left([](bool state) {
      btn_events.push({LV_KEY_PREV, state});
    });
    _nav5->on_right([](bool state) {
      btn_events.push({LV_KEY_NEXT, state});
    });
    _nav5->on_center([](bool state) {
      btn_events.push({LV_KEY_ENTER, state});
    });
  }

  lv_init();

  /*Init Theme*/
  lv_theme_t* th = lv_theme_alien_init(120, NULL);
  th->style.cont->body.padding.left = 6;
  th->style.cont->body.padding.right = 6;
  th->style.cont->body.padding.top = 6;
  th->style.cont->body.padding.bottom = 6;
  th->style.btn.ina->body.radius = 5;
  th->style.btn.pr->body.radius = 5;
  th->style.btn.rel->body.radius = 5;
  th->style.btn.tgl_pr->body.radius = 5;
  th->style.btn.tgl_rel->body.radius = 5;
  lv_theme_set_current(th);

  /*Initialize the display buffer*/
  lv_disp_buf_init(&disp_buf, ui::buf1, nullptr, LV_HOR_RES_MAX);

  lv_disp_drv_t disp_drv;                     /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);                /*Basic initialization*/
  disp_drv.flush_cb = flush_lcd;              /*Set driver function*/
  disp_drv.buffer = &disp_buf;                /*Assign the buffer to the display*/
  _display = lv_disp_drv_register(&disp_drv);  /*Finally register the driver*/

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);              /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_KEYPAD;      /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = nav5_read;              /*Set your driver function*/
  _input_device = lv_indev_drv_register(&indev_drv);   /*Finally register the driver*/
}

void ui::flush_lcd(_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  if (!_lcd->set_window(area->x1, area->y1, area->x2, area->y2)) { return; }
  for (uint32_t y = area->y1; y <= area->y2; y++) {
    for (uint32_t x = area->x1; x <= area->x2; x++) {
      _lcd->send_data(color_p->ch.red);
      _lcd->send_data(color_p->ch.green);
      _lcd->send_data(color_p->ch.blue);
      color_p++;
    }
  }
  lv_disp_flush_ready(disp_drv);
}

bool ui::nav5_read(_lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  data->key = btn_events.front().key;
  data->state = btn_events.front().state ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
  btn_events.pop();
  return btn_events.size() > 0;
}
