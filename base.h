#ifndef FILE_BASE_H
#define FILE_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

typedef unsigned char byte;
typedef unsigned int bit_32;
typedef unsigned long long bit_64;

#define PIPE_SIZE 1048576L
#define BIT_AMOUNT 8

enum file_type{NO_TYPE,FOLDER_TYPE,FILE_TYPE};
enum file_error{NO_ERROR,LOCK_ERROR,ABSTRACT_ERROR};

typedef struct file_memory{
    void * file;
    long size;
} file_memory;

typedef struct bit_pointer{
    byte * byte_pointer;
    int bit_index;
} bit_pointer;

void free_file_memory(file_memory * file_memory_pointer){
    if(file_memory_pointer){
        if(file_memory_pointer->file){
            free(file_memory_pointer->file);
        }
        free(file_memory_pointer);
    }
}

file_memory * load_file(char * file_path){
    FILE * file_pointer = fopen(file_path,"rb");
    file_memory * file_memory_pointer = (file_memory *)malloc(sizeof(file_memory));
    fseek(file_pointer,0,SEEK_END);
    file_memory_pointer->size = ftell(file_pointer);
    file_memory_pointer->file = (void *)malloc(file_memory_pointer->size);
    rewind(file_pointer);
    fread(file_memory_pointer->file,PIPE_SIZE,file_memory_pointer->size/PIPE_SIZE,file_pointer);
    fread((void *)((byte *)file_memory_pointer->file+file_memory_pointer->size/PIPE_SIZE*PIPE_SIZE),1,file_memory_pointer->size%PIPE_SIZE,file_pointer);
    fclose(file_pointer);
    return file_memory_pointer;
}

void store_file(char * file_path, file_memory * file_memory_pointer){
    if(file_memory_pointer->file==NULL){
        return;
    }
    FILE * file_pointer = fopen(file_path,"wb");
    fwrite(file_memory_pointer->file,PIPE_SIZE,file_memory_pointer->size/PIPE_SIZE,file_pointer);
    fwrite((void *)((byte *)file_memory_pointer->file+file_memory_pointer->size/PIPE_SIZE*PIPE_SIZE),1,file_memory_pointer->size%PIPE_SIZE,file_pointer);
    fclose(file_pointer);
}

void write_bit(bit_pointer * bit_pointer_pointer, int bit){
    *(bit_pointer_pointer->byte_pointer)|=(byte)((byte)bit<<(byte)(BIT_AMOUNT-bit_pointer_pointer->bit_index-1));
    bit_pointer_pointer->bit_index++;
    bit_pointer_pointer->byte_pointer+=(bit_pointer_pointer->bit_index/BIT_AMOUNT);
    bit_pointer_pointer->bit_index%=BIT_AMOUNT;
}

int read_bit(bit_pointer * bit_pointer_pointer){
    byte result = (*(bit_pointer_pointer->byte_pointer))&(byte)((byte)1<<(byte)(BIT_AMOUNT-bit_pointer_pointer->bit_index-1));
    bit_pointer_pointer->bit_index++;
    bit_pointer_pointer->byte_pointer+=(bit_pointer_pointer->bit_index/BIT_AMOUNT);
    bit_pointer_pointer->bit_index%=BIT_AMOUNT;
    return result!=0;
}

#endif