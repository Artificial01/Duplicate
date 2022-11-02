#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include "base.h"

#define LOCK_KEY_SIZE 6

byte lock_key[LOCK_KEY_SIZE];
int lock_key_index;

byte lock_anchor[LOCK_KEY_SIZE] = {0b01010101,0b01010101,0b01010101,0b01010101,0b01010101,0b01010101};

void key_lock(const byte * key){
    for(int i=0; i<LOCK_KEY_SIZE; i++){
        lock_key[i] = key[i];
    }
    lock_key_index = 0;
}

byte transform_lock(byte source){
    byte result = source^lock_key[lock_key_index];
    lock_key_index = (lock_key_index+1)%LOCK_KEY_SIZE;
    return result;
}

void process_lock(file_memory * file_memory_pointer, byte * key){
    key_lock(key);
    void * file = (void *)malloc(LOCK_KEY_SIZE+file_memory_pointer->size);
    for(int i=0; i<LOCK_KEY_SIZE; i++){
        ((byte *)file)[i] = transform_lock(lock_anchor[i]);
    }
    for(int i=0; i<file_memory_pointer->size; i++){
        ((byte *)file)[LOCK_KEY_SIZE+i] = transform_lock(((byte *)(file_memory_pointer->file))[i]);
    }
    free(file_memory_pointer->file);
    file_memory_pointer->file = file;
    file_memory_pointer->size+=LOCK_KEY_SIZE;
}

int process_reverse_lock(file_memory * file_memory_pointer, byte * key){
    key_lock(key);
    for(int i=0; i<LOCK_KEY_SIZE; i++){
        if(transform_lock(((byte *)(file_memory_pointer->file))[i])!=lock_anchor[i]){
            free(file_memory_pointer->file);
            file_memory_pointer->file = NULL;
            file_memory_pointer->size = 0;
            return LOCK_ERROR;
        }
    }
    file_memory_pointer->size-=LOCK_KEY_SIZE;
    void * file = (void *)malloc(file_memory_pointer->size);
    for(int i=0; i<file_memory_pointer->size; i++){
        ((byte *)file)[i] = transform_lock(((byte *)(file_memory_pointer->file))[LOCK_KEY_SIZE+i]);
    }
    free(file_memory_pointer->file);
    file_memory_pointer->file = file;
    return NO_ERROR;
}

#endif