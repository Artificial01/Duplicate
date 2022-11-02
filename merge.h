#ifndef FILE_MERGE_H
#define FILE_MERGE_H

#include "base.h"

typedef struct merge_message{
    int type;
    char path[PATH_SIZE];
    long size;
} merge_message;

typedef struct merge_node{
    merge_message merge_message;
    file_memory * file_memory_pointer;
    struct merge_node * next;
} merge_node;

merge_node * merge_current_node_pointer;

void free_merge(merge_node * pointer){
    if(pointer==NULL){
        return;
    }
    free_merge(pointer->next);
    free_file_memory(pointer->file_memory_pointer);
    free(pointer);
}

void file_merge(){
    file_memory * file_memory_pointer = load_file(folder_path);
    merge_node * merge_node_pointer = (merge_node *)malloc(sizeof(merge_node));
    merge_node_pointer->merge_message.type = FILE_TYPE;
    for(int i=0; i<=folder_stack[folder_stack_index]; i++){
        merge_node_pointer->merge_message.path[i] = folder_path[i];
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
    for(int i=0; i<=folder_stack[folder_stack_index]; i++){
        merge_node_pointer->merge_message.path[i] = folder_path[i];
    }
    merge_node_pointer->merge_message.size = sizeof(merge_message);
    merge_node_pointer->file_memory_pointer = NULL;
    merge_node_pointer->next = NULL;
    merge_current_node_pointer->next = merge_node_pointer;
    merge_current_node_pointer = merge_node_pointer;
    DIR * folder_pointer = opendir(folder_path);
    for(struct dirent * folder_entry_pointer=readdir(folder_pointer); folder_entry_pointer; folder_entry_pointer=readdir(folder_pointer)){
        if(strcmp(folder_entry_pointer->d_name,".")!=0 && strcmp(folder_entry_pointer->d_name,"..")!=0){
            enter_folder(folder_entry_pointer->d_name);
            if(identify_folder(folder_path)){
                folder_merge();
            }else{
                file_merge();
            }
            exit_folder();
        }
    }
}

void process_merge(char * path){
    merge_node merge_node = {{},NULL,NULL};
    merge_current_node_pointer = &merge_node;
    init_folder(path);
    folder_merge();
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
    delete_path(path);
    store_file(path,file_memory_pointer);
    free_file_memory(file_memory_pointer);
}

void process_reverse_merge(char * path){
    file_memory * file_memory_pointer = load_file(path);
    byte * pointer = (byte *)(file_memory_pointer->file);
    merge_message buffer;
    delete_file(path);
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
    free_file_memory(file_memory_pointer);
}

#endif