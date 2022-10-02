#ifndef DUPLICATE_BASE_H
#define DUPLICATE_BASE_H

#include <stdio.h>
#include <stdlib.h>

#define IO_UNIT_SIZE 1048576L

typedef struct file_memory{
    void * file;
    long size;
} file_memory;

file_memory load_file(char * file_path);
void store_file(char * file_path, file_memory file_memory);

#endif