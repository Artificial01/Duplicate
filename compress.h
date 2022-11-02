#ifndef FILE_COMPRESS_H
#define FILE_COMPRESS_H

#include "base.h"

#define COMPRESS_UNIT_SIZE 256
#define COMPRESS_TREE_SIZE 511

typedef struct compress_node{
    int sign;
    int parent;
    int child[2];
} compress_node;

long compress_unit[COMPRESS_UNIT_SIZE];
compress_node compress_tree[COMPRESS_TREE_SIZE];

bit_pointer compress_file;
long compress_bit_amount;

int search_compress(){
    int index = 0;
    long value = compress_unit[0];
    for(int i=1; i<COMPRESS_UNIT_SIZE; i++){
        if(compress_unit[i]<value){
            index = i;
            value = compress_unit[i];
        }
    }
    return index;
}

void prepare_compress(const byte * byte, long amount){
    for(int i=0; i<COMPRESS_UNIT_SIZE; i++){
        compress_unit[i] = 0;
    }
    for(int i=0; i<amount; i++){
        compress_unit[byte[i]]++;
    }
    compress_node compress_initial_node = {-1,-1,-1,-1};
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        compress_tree[i] = compress_initial_node;
    }
    for(int i=COMPRESS_UNIT_SIZE; i<COMPRESS_TREE_SIZE; i++){
        int child_0 = search_compress();
        long unit = compress_unit[child_0];
        compress_unit[child_0] = LONG_MAX;
        while(compress_tree[child_0].parent!=-1){
            child_0 = compress_tree[child_0].parent;
        }
        int child_1 = search_compress();
        unit+=compress_unit[child_1];
        compress_unit[child_1] = unit;
        while(compress_tree[child_1].parent!=-1){
            child_1 = compress_tree[child_1].parent;
        }
        compress_tree[child_0].sign = 0;
        compress_tree[child_0].parent = i;
        compress_tree[child_1].sign = 1;
        compress_tree[child_1].parent = i;
        compress_tree[i].child[0] = child_0;
        compress_tree[i].child[1] = child_1;
    }
}

void transform_compress(int compress_node_index){
    if(compress_tree[compress_node_index].parent==-1){
        return;
    }
    transform_compress(compress_tree[compress_node_index].parent);
    write_bit(&compress_file,compress_tree[compress_node_index].sign);
    compress_bit_amount++;
}

void process_compress(file_memory * file_memory_pointer){
    prepare_compress((const byte *)file_memory_pointer->file,file_memory_pointer->size);
    void * file = (void *)malloc(sizeof(compress_tree)+sizeof(long)+file_memory_pointer->size);
    compress_file.byte_pointer = ((byte *)file)+sizeof(compress_tree)+sizeof(long);
    compress_file.bit_index = 0;
    for(int i=0; i<file_memory_pointer->size; i++){
        compress_file.byte_pointer[i] = 0;
    }
    compress_bit_amount = 0;
    for(int i=0; i<file_memory_pointer->size; i++){
        transform_compress(((byte *)(file_memory_pointer->file))[i]);
    }
    free(file_memory_pointer->file);
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        ((compress_node *)file)[i] = compress_tree[i];
    }
    *((long *)(((compress_node *)file)+COMPRESS_TREE_SIZE)) = file_memory_pointer->size;
    file_memory_pointer->file = file;
    file_memory_pointer->size = (long)sizeof(compress_tree)+(long)sizeof(long)+compress_bit_amount/BIT_AMOUNT+(compress_bit_amount%BIT_AMOUNT!=0);
}

void process_reverse_compress(file_memory * file_memory_pointer){
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        compress_tree[i] = ((compress_node *)(file_memory_pointer->file))[i];
    }
    file_memory_pointer->size = *((long *)(((compress_node *)(file_memory_pointer->file))+COMPRESS_TREE_SIZE));
    compress_file.byte_pointer = ((byte *)(file_memory_pointer->file))+sizeof(compress_tree)+sizeof(long);
    compress_file.bit_index = 0;
    void * file = (void *)malloc(file_memory_pointer->size);
    for(int i=0; i<file_memory_pointer->size; i++){
        int compress_node_index = COMPRESS_TREE_SIZE-1;
        while(compress_tree[compress_node_index].child[0]!=-1){
            compress_node_index = compress_tree[compress_node_index].child[read_bit(&compress_file)];
        }
        ((byte *)file)[i] = compress_node_index;
    }
    free(file_memory_pointer->file);
    file_memory_pointer->file = file;
}

#endif