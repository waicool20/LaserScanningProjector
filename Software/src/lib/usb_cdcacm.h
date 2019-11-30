#pragma once

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include "lib/gpio.h"

class usb_cdcacm {
 public:
  void poll();

  static usb_cdcacm& instance();

 private:
  usb_cdcacm();
  void periph_setup();

  gpio _usb_dm_;
  gpio _usb_dp_;
  usbd_device* _usbd_dev;
};
