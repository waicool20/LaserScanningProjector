#include <array>
#include <numeric>
#include <string_view>
#include <thread>
#include <vector>

#include <fmt/format.h>
#include <libusb-1.0/libusb.h>
#include <opencv2/opencv.hpp>

using namespace std::literals;

namespace {
constexpr std::uint16_t vid = 0x0483;
constexpr std::uint16_t pid = 0x5740;

constexpr auto w = 320;
constexpr auto h = 240;

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

void resize_keep_ratio(cv::Mat& src, cv::Mat& dest, const cv::Size& target) {
  const auto y_scale = double(target.height) / src.rows;
  const auto x_scale = double(target.width) / src.cols;

  const auto scale = std::min(x_scale, y_scale);
  cv::resize(src, dest, cv::Size(src.cols * scale, src.rows * scale), scale, scale);

  // Padding-related stuff
  /*const auto x_padding = (target.width - src.cols) / 2;
  const auto y_padding = (target.height - src.rows) / 2;

  cv::copyMakeBorder(dest, dest, y_padding, y_padding, x_padding, x_padding, cv::BORDER_CONSTANT, cv::Scalar(255));

  cv::Mat padded;
  padded.create(src.rows + y_padding * 2, src.cols + x_padding * 2, src.type());
  padded.setTo(cv::Scalar::all(0));
  src.copyTo(padded(cv::Rect(x_padding, y_padding, src.cols, src.rows)));
  padded.copyTo(src);*/
}

int main(int argc, char* argv[]) {
  std::vector tuples(::tuples.begin(), ::tuples.end());
  if (argc == 2) {
    tuples.clear();
    cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (src.empty()) {
      fmt::print("Could not open or find the image!\n");
      fmt::print("Usage: {} <Input image>\n", argv[0]);
      return -1;
    }

    resize_keep_ratio(src, src, cv::Size(w, h));

    cv::Mat src_gray;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    constexpr auto dist_thresh = 200;
    constexpr auto lines_thresh = 200;
    constexpr auto lowThreshold = 0;
    constexpr auto max_lowThreshold = 100;

    cv::Mat detected_edges;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::pair<std::size_t, std::size_t> bounds = {lowThreshold, max_lowThreshold};
    double accumulator = 0.0;
    while (true) {
      const auto threshold = double(bounds.first + bounds.second) / 2;
      contours.clear();
      hierarchy.clear();

      blur(src_gray, detected_edges, cv::Size(3, 3));
      Canny(detected_edges, detected_edges, threshold, threshold * 3, 3);
      findContours(detected_edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

      if (std::fabs(bounds.second - bounds.first) <= 1.0f) {
        break;
      } else if (threshold == lowThreshold || threshold + 1 == max_lowThreshold) {
        break;
      }

      if (contours.size() > lines_thresh) {
        bounds.first = (bounds.first + bounds.second) / 2;
      }

      std::vector<double> distances(contours.size());
      std::transform(contours.begin(), contours.end(), distances.begin(), [](const std::vector<cv::Point>& vp) {
        const double x = vp.back().x - vp.front().x;
        const double y = vp.back().y - vp.back().y;
        return std::sqrt(x * x + y * y);
      });
      accumulator = std::accumulate(distances.begin(), distances.end(), 0.0);
      if (accumulator < dist_thresh) {
        bounds.second = double(bounds.first + bounds.second) / 2;
      } else if (accumulator > dist_thresh) {
        bounds.first = double(bounds.first + bounds.second) / 2;
      } else {
        break;
      }

      // NumContour-based Limit
      /*if (contours.size() == lines_thresh) {
        break;
      } else if (std::fabs(bounds.second - bounds.first) <= 1.0f) {
        break;
      } else if (bounds.second == lowThreshold || bounds.first == max_lowThreshold) {
        break;
      }

      if (contours.size() < lines_thresh) {
        bounds.second = (bounds.first + bounds.second) / 2;
      } else if (contours.size() > lines_thresh) {
        bounds.first = (bounds.first + bounds.second) / 2;
      }*/
    }

    for (const auto& line : contours) {
      const auto last = --line.end();

      for (auto it = line.begin(); it != line.end(); ++it) {
        const bool state = it != last;
        const packet p = {
            std::uint8_t(it->x),
            std::uint8_t(it->y),
            state
        };
        tuples.emplace_back(p);
      }
    }

    fmt::print("Displaying Edges: {}, Distance: {}\n", contours.size(), accumulator);
    cv::Mat drawing = cv::Mat::zeros(detected_edges.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++) {
      drawContours(drawing, contours, int(i), cv::Scalar(255, 255, 255), 1, cv::LINE_8, hierarchy, 0);
    }
  }

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

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}
