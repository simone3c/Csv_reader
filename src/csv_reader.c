#include "include/csv_reader.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void csv_reader_linecpy(csv_line_t* dst,const csv_line_t* src){
    dst->n_fields = src->n_fields;
    dst->fields = calloc(dst->n_fields, sizeof(char*));
    for(int i = 0; i < dst->n_fields; ++i){
        dst->fields[i] = malloc(strlen(src->fields[i]) + 1);
        strcpy(dst->fields[i], src->fields[i]);
    }
}

int csv_reader_init(csv_reader_t* r, const char* file_path, char delimiter){

    FILE* file = fopen(file_path, "r");
    if(file == NULL)
        return -1;

    csv_line_t hdr;

    r->f = file;
    r->delim = delimiter;
    
    if(csv_reader_getline(r, &hdr) < 0)
        return -1;
        
    memcpy(&r->hdr, &hdr, sizeof(hdr));

    return 0;    
}

int csv_reader_getline(csv_reader_t* r, csv_line_t* line_ptr){
    char* line = NULL;
    size_t len = 0, l;
    if((l = getline(&line, &len, r->f)) == -1){
        free(line);
        return -1;
    }

    if(line[l - 1] == '\n'){
        line[l - 1] = 0;
        if(line[l - 2] == '\r')
            line[l - 2] = 0;
    }

    size_t arr_sz = 1;
    char** arr = calloc(arr_sz, sizeof(char*));
    int tok_id = 0;
    char* token = strtok(line, &r->delim);
    while(token){
        if(tok_id == arr_sz){
            arr_sz *= 2;
            arr = reallocarray(arr, arr_sz, sizeof(char*));
        }
        arr[tok_id++] = token;
        token = strtok(NULL, &r->delim);
    }

    line_ptr->n_fields = tok_id;
    line_ptr->fields = calloc(tok_id, sizeof(char*));
    for(int i = 0; i < tok_id; ++i){
        line_ptr->fields[i] = malloc(strlen(arr[i]) + 1);
        strcpy(line_ptr->fields[i], arr[i]);
    }
    
    free(line);
    free(arr);

    return 0;    
}

void csv_reader_line_free(csv_line_t* line_ptr){
    for(int i = 0; i < line_ptr->n_fields; ++i)
        free(line_ptr->fields[i]);

    free(line_ptr->fields);
    memset(line_ptr, 0, sizeof(*line_ptr));
}

int csv_reader_getheader(const csv_reader_t* r, csv_line_t* hdr){

    csv_reader_linecpy(hdr, &r->hdr);

    return 0;
}

int csv_reader_getfield(const csv_reader_t* r, const csv_line_t* line, const char* field, char** field_out, size_t* len){
    assert(*field_out == NULL);
    for(int i = 0; i < r->hdr.n_fields; ++i){
        if(!strcmp(field, r->hdr.fields[i])){
            *len = strlen(line->fields[i]) + 1;
            *field_out = malloc(*len);
            strcpy(*field_out, line->fields[i]);
            return 0;
        }
    }

    return -1;
}


void csv_reader_deinit(csv_reader_t* r){
    csv_reader_line_free(&r->hdr);
    fclose(r->f);
}
