#ifndef CSV_READER
#define CSV_READER
#include "stdio.h"

typedef struct{
    char** fields;
    size_t n_fields;
} csv_line_t;

typedef struct{
    FILE* f;
    csv_line_t hdr;
    char delim;
} csv_reader_t;


int csv_reader_init(csv_reader_t* r, const char* file, char delimiter, char new_line);
void csv_reader_deinit(csv_reader_t* r);
int csv_reader_getheader(const csv_reader_t* r, csv_line_t* hdr);
int csv_reader_getline(csv_reader_t* r, csv_line_t* line_ptr);
int csv_reader_getfield(const csv_reader_t* r, const csv_line_t* line, const char* field, char** field_out, size_t* len);

void csv_reader_line_free(csv_line_t* line_ptr);
void csv_reader_linecpy(csv_line_t* dst, const csv_line_t* src);

#endif
