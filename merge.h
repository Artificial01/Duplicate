#ifndef FILE_MERGE_H
#define FILE_MERGE_H

#include "base.h"

#define MERGE_PATH_SIZE 256
#define MERGE_STACK_SIZE 32

typedef struct merge_message{
    int type;
    char path[MERGE_PATH_SIZE];
    long size;
} merge_message;

typedef struct merge_node{
    merge_message merge_message;
    file_memory * file_memory_pointer;
    struct merge_node * next;
} merge_node;

merge_node * merge_current_node_pointer;

char merge_path[MERGE_PATH_SIZE];
int merge_stack[MERGE_STACK_SIZE];
int merge_stack_index;

void push_merge(char * file_name){
    merge_stack_index++;
    merge_stack[merge_stack_index] = merge_stack[merge_stack_index-1];
    if(merge_stack_index>1){
        merge_path[merge_stack[merge_stack_index]] = '/';
        merge_stack[merge_stack_index]++;
    }
    while(*file_name!='\0'){
        merge_path[merge_stack[merge_stack_index]] = *file_name;
        merge_stack[merge_stack_index]++;
        file_name++;
    }
    merge_path[merge_stack[merge_stack_index]] = '\0';
}

void pop_merge(){
    merge_stack_index--;
    merge_path[merge_stack[merge_stack_index]] = '\0';
}

int identify_merge(){
    struct stat buffer = {};
    stat(merge_path,&buffer);
    return (buffer.st_mode&(unsigned short)S_IFDIR)!=0;
}

void file_merge(){
    file_memory * file_memory_pointer = load_file(merge_path);
    merge_node * merge_node_pointer = (merge_node *)malloc(sizeof(merge_node));
    merge_node_pointer->merge_message.type = FILE_TYPE;
    for(int i=0; i<=merge_stack[merge_stack_index]; i++){
        merge_node_pointer->merge_message.path[i] = merge_path[i];
    }
    merge_node_pointer->merge_message.size = (long)sizeof(merge_message)+file_memory_pointer->size;
    merge_node_pointer->file_memory_pointer = file_memory_pointer;
    merge_node_pointer->next = NULL;
    merge_current_node_pointer->next = merge_node_pointer;
    merge_current_node_pointer = merge_node_pointer;
}

void folder_merge(){
    merge_node * merge_node_pointer = (merge_node *)malloc(sizeof(merge_node));
    merge_node_pointer->merge_message.type = FOLDER_TYPE;
    for(int i=0; i<=merge_stack[merge_stack_index]; i++){
        merge_node_pointer->merge_message.path[i] = merge_path[i];
    }
    merge_node_pointer->merge_message.size = sizeof(merge_message);
    merge_node_pointer->file_memory_pointer = NULL;
    merge_node_pointer->next = NULL;
    merge_current_node_pointer->next = merge_node_pointer;
    merge_current_node_pointer = merge_node_pointer;
    DIR * folder_pointer = opendir(merge_path);
    for(struct dirent * folder_entry_pointer=readdir(folder_pointer); folder_entry_pointer; folder_entry_pointer=readdir(folder_pointer)){
        if(strcmp(folder_entry_pointer->d_name,".")!=0 && strcmp(folder_entry_pointer->d_name,"..")!=0){
            push_merge(folder_entry_pointer->d_name);
            if(identify_merge()){
                folder_merge();
            }else{
                file_merge();
            }
            pop_merge();
        }
    }
}

void free_merge(merge_node * pointer){
    if(pointer==NULL){
        return;
    }
    free_merge(pointer->next);
    free_file_memory(pointer->file_memory_pointer);
    free(pointer);
}

file_memory * process_merge(char * file_path){
    merge_node merge_node = {{},NULL,NULL};
    merge_current_node_pointer = &merge_node;
    merge_path[0] = '\0';
    merge_stack[0] = 0;
    merge_stack_index = 0;
    push_merge(file_path);
    if(identify_merge()){
        folder_merge();
    }else{
        file_merge();
    }
    file_memory * file_memory_pointer = (file_memory *)malloc(sizeof(file_memory));
    file_memory_pointer->size = 0;
    for(merge_current_node_pointer=merge_node.next; merge_current_node_pointer; merge_current_node_pointer=merge_current_node_pointer->next){
        file_memory_pointer->size+=merge_current_node_pointer->merge_message.size;
    }
    file_memory_pointer->size+=sizeof(merge_message);
    file_memory_pointer->file = (void *)malloc(file_memory_pointer->size);
    byte * pointer = (byte *)(file_memory_pointer->file);
    for(merge_current_node_pointer=merge_node.next; merge_current_node_pointer; merge_current_node_pointer=merge_current_node_pointer->next){
        *((merge_message *)pointer) = merge_current_node_pointer->merge_message;
        pointer+=sizeof(merge_message);
        if(merge_current_node_pointer->file_memory_pointer){
            for(int i=0; i<merge_current_node_pointer->file_memory_pointer->size; i++){
                *pointer = ((byte *)(merge_current_node_pointer->file_memory_pointer->file))[i];
                pointer++;
            }
        }
    }
    free_merge(merge_node.next);
    merge_message merge_last_message = {NO_TYPE};
    *((merge_message *)pointer) = merge_last_message;
    return file_memory_pointer;
}

void process_reverse_merge(file_memory * file_memory_pointer){
    byte * pointer = (byte *)(file_memory_pointer->file);
    merge_message buffer;
    while(1){
        buffer = *((merge_message *)pointer);
        if(buffer.type==FOLDER_TYPE){
            mkdir(buffer.path);
        }else if(buffer.type==FILE_TYPE){
            file_memory temporary_file_memory = {pointer+sizeof(merge_message),buffer.size-(long)sizeof(merge_message)};
            store_file(buffer.path,&temporary_file_memory);
        }else{
            break;
        }
        pointer+=buffer.size;
    }
}

#endif