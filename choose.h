#ifndef FILE_CHOOSE_H
#define FILE_CHOOSE_H

#include "base.h"
#include "merge.h"
#include "compress.h"
#include "lock.h"
#include "abstract.h"

#define MERGE_SIGN 0b10000000U
#define COMPRESS_SIGN 0b01000000U
#define LOCK_SIGN 0b00100000U

file_memory * process_choose(char * file_path, byte * key, int merge_sign, int compress_sign, int lock_sign){
    file_memory * file_memory_pointer;
    byte sign = 0;
    if(merge_sign){
        file_memory_pointer = process_merge(file_path);
        sign|=MERGE_SIGN;
    }else{
        file_memory_pointer = load_file(file_path);
    }
    if(compress_sign){
        process_compress(file_memory_pointer);
        sign|=COMPRESS_SIGN;
    }
    if(lock_sign){
        process_lock(file_memory_pointer,key);
        sign|=LOCK_SIGN;
    }
    void * file = (void *)malloc(file_memory_pointer->size+1);
    for(long i=0; i<file_memory_pointer->size; i++){
        ((byte *)file)[i] = ((byte *)file_memory_pointer->file)[i];
    }
    ((byte *)file)[file_memory_pointer->size] = sign;
    free(file_memory_pointer->file);
    file_memory_pointer->file = file;
    file_memory_pointer->size++;
    return file_memory_pointer;
}

void process_reverse_choose(char * file_path, byte * key){
    if(process_reverse_abstract(file_path)){
        puts("ABSTRACT ERROR");
        return;
    }
    file_memory * file_memory_pointer = load_file(file_path);
    file_memory_pointer->size-=1;
    byte sign = ((byte *)file_memory_pointer->file)[file_memory_pointer->size];
    if(sign&LOCK_SIGN){
        if(process_reverse_lock(file_memory_pointer,key)){
            puts("LOCK ERROR");
            free_file_memory(file_memory_pointer);
            return;
        }
    }
    if(sign&COMPRESS_SIGN){
        process_reverse_compress(file_memory_pointer);
    }
    if(sign&MERGE_SIGN){
        process_reverse_merge(file_memory_pointer);
    }else{
        store_file(file_path,file_memory_pointer);
    }
    free_file_memory(file_memory_pointer);
}

#endif