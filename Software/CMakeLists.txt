cmake_minimum_required(VERSION 3.10)

project(LaserScanningProject CXX)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=rtlib/cmake/arm-toolchain.cmake or use\
    your own toolchain file")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(LOG_VERBOSE true)
set(DEVICE STM32F303CCT6)

include(cmake/libopencm3.cmake)
include(cmake/compile_flags.cmake)

# Additional Flags
set(ADDITIONAL_COMMON_FLAGS
        -fmessage-length=0
        -ffunction-sections
        -fdata-sections
        -fsigned-char)
set(ADDITIONAL_CXX_FLAGS
        ${ADDITIONAL_COMMON_FLAGS}
        -fno-exceptions
        -fno-rtti)
set(ADDITIONAL_LINKER_FLAGS
        -nostartfiles
        --specs=rdimon.specs
        -Wl,-Map,${CMAKE_PROJECT_NAME}.map,--cref
        -Wl,--gc-sections)

# Additional Libraries
include(cmake/lvgl.cmake)

message("----------------------------------------")

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/drivers/st7735s.cpp
        src/drivers/stepper_motor.cpp
        src/drivers/laser.cpp
        src/drivers/laser_canvas.cpp
        src/drivers/nav5.cpp
        src/lib/gpio.cpp
        src/lib/rcc.cpp
        src/lib/systick.cpp
        src/lib/ui.cpp)
add_dependencies(${PROJECT_NAME}.elf libopencm3-genlink)
target_compile_options(${PROJECT_NAME}.elf PRIVATE
        ${TARGET_SPECIFIC_FLAGS}
        ${ADDITIONAL_CXX_FLAGS}
        ${CXX_WARN_FLAGS})
target_include_directories(${PROJECT_NAME}.elf PRIVATE src)
target_link_options(${PROJECT_NAME}.elf PRIVATE
        ${LINKER_FLAGS}
        ${ADDITIONAL_LINKER_FLAGS})
target_link_libraries(${PROJECT_NAME}.elf PRIVATE
        ${LDLIBS}
        c
        rdimon
        LVGL)

# Add a secondary target to generate a bin file from the elf file.
add_custom_target(
        ${PROJECT_NAME}.bin ALL
        COMMAND ${ARM_OBJCOPY} -Obinary "${PROJECT_NAME}.elf" "${PROJECT_NAME}.bin"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS ${PROJECT_NAME}.elf
)

# Output elf file size
add_custom_command(
        TARGET ${PROJECT_NAME}.elf
        POST_BUILD
        COMMAND ${ARM_SIZE} "${PROJECT_NAME}.elf"
)

add_custom_target(
        FlashElf
        COMMAND arm-none-eabi-gdb -q -ex "set confirm off" -ex "target extended-remote :3333" -ex "load" -ex "kill" -ex "quit" ${PROJECT_NAME}.elf
        DEPENDS ${PROJECT_NAME}.elf
)
