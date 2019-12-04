#include <cstdint>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "main.h"
#include "drivers/laser.h"
#include "drivers/laser_canvas.h"
#include "drivers/nav5.h"
#include "drivers/stepper_motor.h"
#include "drivers/st7735s.h"
#include "lib/rcc.h"
#include "lib/systick.h"
#include "lib/usb_cdcacm.h"
#include "lib/ui.h"
#include "views/view_init.h"
#include "views/view_main.h"
#include "drivers/mic.h"
#include "drivers/headphones.h"
#include "images/smiley.h"

using namespace std::literals;

namespace {
constexpr uint32_t default_w = 128;
constexpr uint32_t default_h = 72;
}

namespace {
constexpr std::array<laser_canvas::tuple, 4> tuples = {{
                                                           {0, 0, true},
                                                           {127, 0, true},
                                                           {127, 71, true},
                                                           {0, 71, true}
                                                       }};
}  // namespace

render rendering = render::NONE;

int main() {
  rcc::clock_setup_pll(rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);
  systick::init();

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  laser laser{};
  stepper_motor xM{gpio(GPIOB, GPIO7), gpio(GPIOB, GPIO6), gpio(GPIOB, GPIO5)};
  stepper_motor yM{gpio(GPIOB, GPIO4), gpio(GPIOB, GPIO3), gpio(GPIOA, GPIO15)};
  gpio ldr = gpio(GPIOB, GPIO0);
  laser_canvas canvas{25600, default_w,  default_h, laser, xM, yM, ldr};

  st7735s lcd{0, 0, 128, 160, st7735s::COLOR_MODE_18_BITS};
  nav5 nav5{
      gpio(GPIOC, GPIO15),
      gpio(GPIOC, GPIO13),
      gpio(GPIOC, GPIO14),
      gpio(GPIOB, GPIO8),
      gpio(GPIOB, GPIO9)
  };

  std::uint64_t last_usb_cmd_issue = 0;

  ui::init(&lcd, &nav5);
  view_init::show(true);

  mic mic{};
  headphones headphones{};
  while (true) {
    switch (rendering) {
      case render::BASIC_RECT:
        canvas.resize(default_w,  default_h);
        canvas.highlight_canvas_area();
        break;
      case render::BASIC_BITMAP:
        canvas.resize(default_w,  default_h);
        canvas.draw_frame(&smiley_map[0]);
        break;
      case render::BASIC_TUPLE:
        canvas.resize(default_w,  default_h);
        canvas.draw_tuples(tuples.data(), tuples.size());
        break;
      case render::DEBUG_HOME_LASER:
        canvas.home();
        rendering = render::NONE;
        break;
      case render::INIT_HOME_LASER:
        canvas.home();
        rendering = render::NONE;
        view_init::show(false);
        view_main::show(true);
        break;
      case render::AUDIO_MIC:
        mic.enable();
        canvas.resize(512, 512);
        canvas.draw_magnitude_y(mic.get_latest_value());
        break;
      case render::AUDIO_HEADPHONES:
        headphones.enable();
        canvas.resize(512, 512);
        canvas.draw_magnitude_y(headphones.channel_average());
        break;
      case render::AUDIO_HEADPHONES_XY:
        headphones.enable();
        canvas.resize(512, 512);
        canvas.draw_magnitude_xy(headphones.channel_x(), headphones.channel_y());
        break;
      case render::USB:
        canvas.resize(320, 240);
        if (usb_cdcacm::instance().tuple_present()) {
          last_usb_cmd_issue = systick::ms();
          const auto t = usb_cdcacm::instance().tuple_pop();
          canvas.draw_tuples(&t, 1);
        } else if (last_usb_cmd_issue - systick::ms() > 1000) {
          laser.disable();
        }
        break;
      case render::DEBUG_MANUAL:
        laser.enable();
        if (nav5.read_left()) {
          canvas.goto_xy(canvas.get_current_x() - 1, canvas.get_current_y(), false);
        } else if (nav5.read_right()) {
          canvas.goto_xy(canvas.get_current_x() + 1, canvas.get_current_y(), false);
        } else if (nav5.read_up()) {
          canvas.goto_xy(canvas.get_current_x(), canvas.get_current_y() - 1, false);
        } else if (nav5.read_down()) {
          canvas.goto_xy(canvas.get_current_x(), canvas.get_current_y() + 1, false);
        }
        break;
      case render::NONE:
      default:
        mic.disable();
        headphones.disable();
        canvas.clear();
        break;
    }
  }
}
