add_library(Csv_reader STATIC 
    csv_reader.c
)

target_include_directories(Csv_reader
    PRIVATE "${INCLUDE_DIR}"
)

target_compile_options(Csv_reader
    PUBLIC "${GXX_COMPILE_OPTIONS}"
)