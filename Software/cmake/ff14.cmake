enable_language(C)
add_library(FF14
        middleware/ff14/source/ffunicode.c
        middleware/ff14/source/ff.h
        middleware/ff14/source/ffsystem.c
        middleware/ff14/source/diskio.h
        middleware/ff14/source/ffconf.h
        middleware/ff14/source/ff.c
        )
target_compile_options(FF14 PRIVATE
        ${TARGET_SPECIFIC_FLAGS}
        ${ADDITIONAL_CXX_FLAGS}
        ${CXX_WARN_FLAGS})
target_link_options(FF14 PRIVATE
        ${LINKER_FLAGS}
        ${ADDITIONAL_LINKER_FLAGS})
