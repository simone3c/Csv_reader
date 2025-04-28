# csv_reader
Simple csv reader written in C

## Usage notes
- The CSV file format requires to have an header with the field's name
- CR and CRLF are both supported
- The application automatically allocates memory in function parameters- remember to free them before using them again or when they are not needed anymore
- Defining the preprocessor macro ```INTERNAL_GETLINE``` allows to use this parser inside ESP-IDF which provides ```__getline()``` instead of ```getline()```

## Usage
Running Cmake build process will produce a static library under ```build/src/libcsv_reader.a``` which you have to link when building your project.

```c
/*
    file.csv:

    f1,f2,f3
    1,2,3
*/

    #include "csv_reader.h"

    // Initialization
    csv_reader_t csv;
    csv_reader_init(&csv, "file.csv", ',');

    // Read the CSV header
    csv_reader_line_t* header;
    csv_reader_getheader(&csv, &header);
    printf("%s", header.line); // "f1,f2,f3"

    // Read each line and obtain a specific field
    csv_reader_line_t* line;
    char* field = NULL;
    while(!csv_reader_getline(&csv, &line)){
        csv_reader_getfield(&csv, &line, "f1", &field);
        printf("%s", field); // "1"
    }

    // Clean all the allocated memory
    csv_reader_line_free(line);
    csv_reader_line_free(header);
    csvreader_deinit(&csv);

```
## TODO
- better cmake
- error codes
- doc