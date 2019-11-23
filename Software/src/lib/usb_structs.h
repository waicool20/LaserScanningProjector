#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

namespace {
constexpr usb_device_descriptor _dev = {
    USB_DT_DEVICE_SIZE,
    USB_DT_DEVICE,
    0x0200,
    USB_CLASS_CDC,
    0,
    0,
    64,
    0x0483,
    0x5740,
    0x0200,
    1,
    2,
    3,
    1,
};

/*
 * This notification endpoint isn't implemented. According to CDC spec its
 * optional, but its absence causes a NULL pointer dereference in Linux
 * cdc_acm driver.
 */
constexpr std::array<usb_endpoint_descriptor, 1> _comm_endp = {{
                                                                   {
                                                                       USB_DT_ENDPOINT_SIZE,
                                                                       USB_DT_ENDPOINT,
                                                                       0x83,
                                                                       USB_ENDPOINT_ATTR_INTERRUPT,
                                                                       16,
                                                                       255,
                                                                       {},
                                                                       {}
                                                                   }
                                                               }};
constexpr std::array<usb_endpoint_descriptor, 2> _data_endp = {{
                                                                   {
                                                                       USB_DT_ENDPOINT_SIZE,
                                                                       USB_DT_ENDPOINT,
                                                                       0x01,
                                                                       USB_ENDPOINT_ATTR_BULK,
                                                                       64,
                                                                       1,
                                                                       {},
                                                                       {}
                                                                   },
                                                                   {
                                                                       USB_DT_ENDPOINT_SIZE,
                                                                       USB_DT_ENDPOINT,
                                                                       0x82,
                                                                       USB_ENDPOINT_ATTR_BULK,
                                                                       64,
                                                                       1,
                                                                       {},
                                                                       {}
                                                                   }
                                                               }};

constexpr struct {
  usb_cdc_header_descriptor header;
  usb_cdc_call_management_descriptor call_mgmt;
  usb_cdc_acm_descriptor acm;
  usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) _cdcacm_functional_descriptors = {
    {
        sizeof(usb_cdc_header_descriptor),
        CS_INTERFACE,
        USB_CDC_TYPE_HEADER,
        0x0110
    },
    {
        sizeof(usb_cdc_call_management_descriptor),
        CS_INTERFACE,
        USB_CDC_TYPE_CALL_MANAGEMENT,
        0,
        1
    },
    {
        sizeof(usb_cdc_acm_descriptor),
        CS_INTERFACE,
        USB_CDC_TYPE_ACM,
        0,
    },
    {
        sizeof(usb_cdc_union_descriptor),
        CS_INTERFACE,
        USB_CDC_TYPE_UNION,
        0,
        1,
    }
};

constexpr std::array<usb_interface_descriptor, 1> _comm_iface = {{
                                                                     {
                                                                         USB_DT_DEVICE_SIZE,
                                                                         USB_DT_INTERFACE,
                                                                         0,
                                                                         0,
                                                                         1,
                                                                         USB_CLASS_CDC,
                                                                         USB_CDC_SUBCLASS_ACM,
                                                                         USB_CDC_PROTOCOL_AT,
                                                                         0,
                                                                         _comm_endp.data(),
                                                                         &_cdcacm_functional_descriptors,
                                                                         sizeof(_cdcacm_functional_descriptors)
                                                                     }
                                                                 }};

constexpr std::array<usb_interface_descriptor, 1> _data_iface = {{
                                                                     {
                                                                         USB_DT_INTERFACE_SIZE,
                                                                         USB_DT_INTERFACE,
                                                                         1,
                                                                         0,
                                                                         2,
                                                                         USB_CLASS_DATA,
                                                                         0,
                                                                         0,
                                                                         0,
                                                                         _data_endp.data(),
                                                                         {},
                                                                         {}
                                                                     }
                                                                 }};

constexpr std::array<usb_interface, 2> _ifaces = {{
                                                      {
                                                          {},
                                                          _comm_iface.size(),
                                                          {},
                                                          _comm_iface.data()
                                                      },
                                                      {
                                                          {},
                                                          _data_iface.size(),
                                                          {},
                                                          _data_iface.data()
                                                      }
                                                  }};

constexpr usb_config_descriptor _config = {
    USB_DT_CONFIGURATION_SIZE,
    USB_DT_CONFIGURATION,
    0,
    2,
    1,
    0,
    USB_CONFIG_ATTR_DEFAULT,
    0x32,

    _ifaces.data(),
};

constexpr std::array<const char*, 3> _usb_strings = {{
                                                         "Black Sphere Technologies",
                                                         "CDC-ACM Demo",
                                                         "DEMO"
                                                     }};
}  // namespace
