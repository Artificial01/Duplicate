#ifndef FILE_BASE_H
#define FILE_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

typedef unsigned char byte;
typedef unsigned int bit_32;
typedef unsigned long long bit_64;

#define BIT_AMOUNT 8
#define PIPE_SIZE 1048576L
#define PATH_SIZE 256
#define FOLDER_DEPTH 32

enum file_type{NO_TYPE,FOLDER_TYPE,FILE_TYPE};

typedef struct file_memory{
    void * file;
    long size;
} file_memory;

typedef struct bit_pointer{
    byte * byte_pointer;
    int bit_index;
} bit_pointer;

char folder_path[PATH_SIZE];
int folder_stack[FOLDER_DEPTH];
int folder_stack_index;

void free_file_memory(file_memory * file_memory_pointer){
    if(file_memory_pointer){
        if(file_memory_pointer->file){
            free(file_memory_pointer->file);
        }
        free(file_memory_pointer);
    }
}

file_memory * load_file(char * path){
    FILE * file_pointer = fopen(path,"rb");
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

void store_file(char * path, file_memory * file_memory_pointer){
    if(file_memory_pointer->file==NULL){
        return;
    }
    FILE * file_pointer = fopen(path,"wb");
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

int identify_folder(char * path){
    struct stat buffer;
    stat(path,&buffer);
    return (buffer.st_mode&(unsigned short)S_IFDIR)!=0;
}

void enter_folder(char * name){
    folder_stack_index++;
    folder_stack[folder_stack_index] = folder_stack[folder_stack_index-1];
    folder_path[folder_stack[folder_stack_index]] = '/';
    folder_stack[folder_stack_index]++;
    while(*name!='\0'){
        folder_path[folder_stack[folder_stack_index]] = *name;
        folder_stack[folder_stack_index]++;
        name++;
    }
    folder_path[folder_stack[folder_stack_index]] = '\0';
}

void exit_folder(){
    folder_stack_index--;
    folder_path[folder_stack[folder_stack_index]] = '\0';
}

void init_folder(char * path){
    folder_stack[0] = 0;
    folder_stack_index = 0;
    while(*path!='\0'){
        folder_path[folder_stack[0]] = *path;
        folder_stack[0]++;
        path++;
    }
    folder_path[folder_stack[0]] = '\0';
}

void delete_file(){
    remove(folder_path);
}

void delete_folder(){
    DIR * folder_pointer = opendir(folder_path);
    for(struct dirent * folder_entry_pointer=readdir(folder_pointer); folder_entry_pointer; folder_entry_pointer=readdir(folder_pointer)){
        if(strcmp(folder_entry_pointer->d_name,".")!=0 && strcmp(folder_entry_pointer->d_name,"..")!=0){
            enter_folder(folder_entry_pointer->d_name);
            if(identify_folder(folder_path)){
                delete_folder();
                rmdir(folder_path);
            }else{
                delete_file();
            }
            exit_folder();
        }
    }
}

void delete_path(char * path){
    init_folder(path);
    if(identify_folder(path)){
        delete_folder();
        rmdir(folder_path);
    }else{
        delete_file();
    }
}

#endif