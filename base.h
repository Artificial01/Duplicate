#ifndef DUPLICATE_BASE_H
#define DUPLICATE_BASE_H

#include <stdio.h>
#include <stdlib.h>

#define IO_UNIT_SIZE 1048576L
#define BIT_OF_BYTE_AMOUNT 8

typedef struct file_memory{
    void * file;
    long size;
} file_memory;

typedef struct bit_pointer{
    unsigned char * character_pointer;
    int bit_index;
} bit_pointer;

file_memory * load_file(char * file_path);
void store_file(char * file_path, file_memory * file_memory_pointer);

void update_bit(bit_pointer * bit_pointer_pointer, unsigned char storage, int amount);
int fetch_bit(bit_pointer * bit_pointer_pointer);

#endif