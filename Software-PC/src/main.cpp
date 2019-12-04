#include <array>
#include <string_view>
#include <thread>
#include <vector>

#include <fmt/format.h>
#include <libusb-1.0/libusb.h>

using namespace std::literals;

namespace {
constexpr std::uint16_t vid = 0x0483;
constexpr std::uint16_t pid = 0x5740;

void libusb_check_error(int rc, const char* func_name) {
  if (rc < 0) {
    std::string e_what = "Error initializing libusb: ";
    e_what += libusb_error_name(rc);
    fmt::print(stderr, "{}: {}", func_name, e_what);
    throw std::runtime_error(e_what);
  }
}

void libusb_cdc_claim_interface(libusb_device_handle* const device_handle) {
  for (int if_num = 0; if_num < 2; ++if_num) {
    if (libusb_kernel_driver_active(device_handle, if_num) != 0) {
      libusb_detach_kernel_driver(device_handle, if_num);
    }
    libusb_check_error(libusb_claim_interface(device_handle, if_num), __func__);
  }
}

void libusb_cdc_init_line_state(libusb_device_handle* const device_handle) {
  libusb_check_error(libusb_control_transfer(device_handle, 0x21, 0x22, 0x01 | 0x02, 0, nullptr, 0, 0), __func__);
}

void libusb_cdc_init_line_encoding(libusb_device_handle* const device_handle) {
  std::array<unsigned char, 8> encoding = {0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00};
  libusb_check_error(libusb_control_transfer(device_handle, 0x21, 0x20, 0, 0, encoding.data(), encoding.size(), 0), __func__);
}

void libusb_write(libusb_device_handle* const device_handle, std::vector<unsigned char>& data) {
  int actual_length;
  if (int rc = libusb_bulk_transfer(device_handle, 0x01, data.data(), data.size(), &actual_length, 0); rc < 0) {
    fmt::print(stderr, "Error while sending data\n");
  } else if (data.size() != actual_length) {
    fmt::print(stderr, "Incomplete packet sent!\n");
  }
}

void libusb_read(libusb_device_handle* const device_handle, std::vector<unsigned char>& buffer) {
  int actual_length;
  if (int rc = libusb_bulk_transfer(device_handle, 0x82, buffer.data(), buffer.size(), &actual_length, 1000); rc < 0) {
    if (rc == LIBUSB_ERROR_TIMEOUT) {
      fmt::print(stderr, "Request timed out with {} bytes sent\n", actual_length);
    } else {
      fmt::print("Error while waiting for data\n");
    }

    buffer.resize(0);
  } else {
    buffer.resize(actual_length);
  }
}

struct packet {
  std::uint8_t x;
  std::uint8_t y;
  bool state;
};

constexpr std::array<packet, 4> tuples = {{
                                              {0, 0, true},
                                              {127, 0, true},
                                              {127, 71, true},
                                              {0, 71, true}
                                          }};

}  // namespace

int main() {
  libusb_check_error(libusb_init(nullptr), __func__);

  libusb_set_option(nullptr, LIBUSB_OPTION_LOG_LEVEL, 3);

  libusb_device_handle* device_handle = nullptr;
  while (device_handle == nullptr) {
    if ((device_handle = libusb_open_device_with_vid_pid(nullptr, vid, pid)) != nullptr) {
      break;
    }

    fmt::print(stderr, "Error finding USB device! Retrying in 5 seconds...\n");
    std::this_thread::sleep_for(5s);
  }

  libusb_cdc_claim_interface(device_handle);
  libusb_cdc_init_line_state(device_handle);
  libusb_cdc_init_line_encoding(device_handle);

  std::vector<unsigned char> out_data(64);
  std::vector<unsigned char> in_data(1024);

  auto it = tuples.begin();
  while (true) {
    out_data.resize(64);
    in_data.resize(1024);

    out_data[0] = static_cast<unsigned char>(it->x);
    out_data[1] = static_cast<unsigned char>(it->y);
    out_data[2] = static_cast<unsigned char>(it->state);

    out_data.resize(4);

    libusb_write(device_handle, out_data);
    libusb_read(device_handle, in_data);

    if (in_data.front() != 0) {
      if (in_data.front() != 1) {
        fmt::print(stderr, "Expected 0, got {}\n", in_data.front());
      }
    } else {
      ++it;
      if (it == tuples.end()) {
        it = tuples.begin();
      }
    }
  }
}
