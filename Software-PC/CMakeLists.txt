cmake_minimum_required(VERSION 3.10)
project(LaserScanningProject-PC CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt REQUIRED)
find_package(OpenCV REQUIRED)

find_path(LIBUSB_INCLUDE_DIR NAMES libusb.h PATHS /usr/include/libusb-1.0/)
find_library(LIBUSB_LIBRARY NAMES usb-1.0)

add_executable(${PROJECT_NAME}
        src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE ${CXX_WARN_FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${FMT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LIBUSB_LIBRARY}
        fmt::fmt
        ${OpenCV_LIBS}
        opencv_imgcodecs)
