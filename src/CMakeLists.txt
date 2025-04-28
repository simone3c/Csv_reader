add_library(csv_reader STATIC 
    csv_reader.c  
)

target_include_directories(csv_reader
    PUBLIC "${CSV_READER_INCLUDE_DIR}"
)

target_compile_options(csv_reader
    PUBLIC "${GXX_COMPILE_OPTIONS}"
)