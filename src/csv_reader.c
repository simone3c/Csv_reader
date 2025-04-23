#include "include/csv_reader.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULT_FIELDS_NUM 4

int csv_reader_linecpy(csv_line_t* dst, const csv_line_t* src){
    size_t i = 0;

    dst->line = (char*)malloc(strlen(src->line) + 1);
    dst->fields = (char**)calloc(src->n_fields, sizeof(char*));
    
    if(!dst->line || !dst->fields)
        goto MALLOC_ERROR;
    
    strcpy(dst->line, src->line);
    
    while(i < src->n_fields){
        dst->fields[i] = (char*)malloc(strlen(src->fields[i]) + 1);
        if(!dst->fields[i])
            goto MALLOC_ERROR_WITH_LOOP;
        ++i;
    }

    dst->n_fields = src->n_fields;

    i = 0;
    while(i < dst->n_fields){
        strcpy(dst->fields[i], src->fields[i]);
        ++i;
    }

    return 0;

MALLOC_ERROR_WITH_LOOP:
    while(i)
        free(dst->fields[--i]);

MALLOC_ERROR:
    free(dst->line);
    free(dst->fields);
    memset(dst, 0, sizeof(*dst));
    return -1;
}

int csv_reader_init(csv_reader_t* r, const char* file_path, char delimiter){
    
    csv_line_t hdr;

    FILE* file = fopen(file_path, "r");
    if(file == NULL)
        return -1;

    r->f = file;
    r->delim = delimiter;
    
    if(csv_reader_getline(r, &hdr) < 0){
        fclose(r->f);
        return -1;
    }
        
    memcpy(&r->hdr, &hdr, sizeof(hdr));

    return 0;    
}

int csv_reader_getline(csv_reader_t* r, csv_line_t* line_ptr){
    size_t i;
    char** arr = NULL;
    char* token;
    size_t tok_id;
    line_ptr->fields = NULL;
    line_ptr->line = NULL;

    char* line = NULL;
    size_t len = 0;
    ssize_t l = getline(&line, &len, r->f);
    size_t arr_sz = DEFAULT_FIELDS_NUM;
    
    if(l < 1)
        goto MALLOC_ERROR;
    
    if(line[l - 1] == '\n'){
        line[l - 1] = 0;
        if(l > 1 && line[l - 2] == '\r')
            line[l - 2] = 0;
    }

    line_ptr->line = (char*)malloc(strlen(line) + 1);
    if(!line_ptr->line)
        goto MALLOC_ERROR;
    strcpy(line_ptr->line, line);

    arr = (char**)calloc(arr_sz, sizeof(char*));
    token = strtok(line, &r->delim);
    tok_id = 0;
    while(token){
        if(tok_id == arr_sz){
            arr_sz *= 2;
            arr = (char**)reallocarray(arr, arr_sz, sizeof(char*));
            if(!arr)
                goto MALLOC_ERROR;
        }
        arr[tok_id++] = token;        
        token = strtok(NULL, &r->delim);
    }

    line_ptr->fields = (char**)calloc(tok_id, sizeof(char*));
    if(!line_ptr->fields)
        goto MALLOC_ERROR;

    i = 0;
    while(i < tok_id){
        line_ptr->fields[i] = (char*)malloc(strlen(arr[i]) + 1);
        if(!line_ptr->fields[i])
            goto MALLOC_ERROR_WITH_LOOP;

        strcpy(line_ptr->fields[i], arr[i]);
        ++i;
    }
    line_ptr->n_fields = tok_id;

    free(line);
    free(arr);

    return 0;   
    
MALLOC_ERROR_WITH_LOOP:
    while(i)
        free(line_ptr->fields[--i]);
    
MALLOC_ERROR:
    free(line_ptr->line);
    free(line_ptr->fields);
    memset(line_ptr, 0, sizeof(*line_ptr));
    free(arr);
    free(line);

    return -1;
}

void csv_reader_line_free(csv_line_t* line_ptr){
    for(size_t i = 0; i < line_ptr->n_fields; ++i)
        free(line_ptr->fields[i]);

    free(line_ptr->fields);
    free(line_ptr->line);
    memset(line_ptr, 0, sizeof(*line_ptr));
}

int csv_reader_getheader(const csv_reader_t* r, csv_line_t* hdr){

    return csv_reader_linecpy(hdr, &r->hdr);
}

int csv_reader_getfield(const csv_reader_t* r, const csv_line_t* line, const char* field, char** field_out){
    for(size_t i = 0; i < r->hdr.n_fields; ++i){
        if(!strcmp(field, r->hdr.fields[i])){
            size_t l = strlen(line->fields[i]) + 1;
            *field_out = (char*)malloc(l);

            if(!*field_out)
                return -1;

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
