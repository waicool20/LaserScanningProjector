#pragma once

#include <optional>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include "drivers/laser_canvas.h"
#include "lib/gpio.h"

class usb_cdcacm {
 public:
  /**
   * \brief Polls the USB for data.
   */
  void poll();

  /**
   * \return Whether a tuple is waiting for service.
   */
  bool tuple_present() const;
  /**
   * \brief Adds the tuple for handling.
   *
   * \param tuple newly received tuple
   */
  void tuple_push(laser_canvas::tuple tuple);
  /**
   * \brief Consumes and returns the tuple.
   *
   * \return The cached tuple.
   */
  laser_canvas::tuple tuple_pop();

  static usb_cdcacm& instance();

 private:
  usb_cdcacm();
  void periph_setup();

  bool _timer_en_ = false;

  std::optional<laser_canvas::tuple> _tuple_;

  gpio _usb_dm_;
  gpio _usb_dp_;
  usbd_device* _usbd_dev;
};
