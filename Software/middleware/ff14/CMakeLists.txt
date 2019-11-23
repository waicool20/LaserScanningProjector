project(FF14 C)

set(FF14_HEADERS
        source/ff.h
        source/diskio.h
        source/ffconf.h)
set(FF14_SOURCES
        source/ffunicode.c
        source/ffsystem.c
        source/ff.c)

add_library(FF14
        ${FF14_HEADERS}
        ${FF14_SOURCES})
target_compile_options(FF14 PRIVATE
        ${TARGET_SPECIFIC_FLAGS}
        ${ADDITIONAL_C_FLAGS})
target_include_directories(FF14 SYSTEM PUBLIC source)
target_link_options(FF14 PRIVATE
        ${LINKER_FLAGS}
        ${ADDITIONAL_LINKER_FLAGS})