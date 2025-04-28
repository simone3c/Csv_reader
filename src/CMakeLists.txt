add_library(Csv_reader STATIC 
    csv_reader.c  
)

target_include_directories(Csv_reader
    PUBLIC "${INCLUDE_DIR}"
)

target_compile_options(Csv_reader
    PUBLIC "${GXX_COMPILE_OPTIONS}"
)

set_target_properties(Csv_reader PROPERTIES
    PUBLIC_HEADER "${INCLUDE_DIR}/Csv_reader.h"
)