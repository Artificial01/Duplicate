#include "compress.h"

void create_compress_unit_compress(unsigned char * byte, long amount){
    for(int i=0; i<COMPRESS_UNIT_CATEGORY; i++){
        compress_unit[i] = 0;
    }
    for(int i=0; i<amount; i++){
        compress_unit[*byte]++;
        byte++;
    }
}

int minimum_compress_unit_compress(){
    int minimum_index = 0;
    long minimum_value = compress_unit[0];
    for(int i=1; i<COMPRESS_UNIT_CATEGORY; i++){
        if(compress_unit[i]<minimum_value){
            minimum_index = i;
            minimum_value = compress_unit[i];
        }
    }
    return minimum_index;
}

void create_compress_tree_compress(){
    compress_node compress_node = {-1,-1,-1,-1};
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        compress_tree[i] = compress_node;
    }
    for(int i=COMPRESS_UNIT_CATEGORY; i<COMPRESS_TREE_SIZE; i++){
        int child_0 = minimum_compress_unit_compress();
        long unit = compress_unit[child_0];
        compress_unit[child_0] = LONG_MAX;
        while(compress_tree[child_0].parent!=-1){
            child_0 = compress_tree[child_0].parent;
        }
        int child_1 = minimum_compress_unit_compress();
        unit+=compress_unit[child_1];
        compress_unit[child_1] = unit;
        while(compress_tree[child_1].parent!=-1){
            child_1 = compress_tree[child_1].parent;
        }
        compress_tree[child_0].parent = i;
        compress_tree[child_0].parent_sign = 0;
        compress_tree[child_1].parent = i;
        compress_tree[child_1].parent_sign = 1;
        compress_tree[i].child_0 = child_0;
        compress_tree[i].child_1 = child_1;
    }
}

void process_compress(file_memory file_memory){
    create_compress_unit_compress(file_memory.file,file_memory.size);
    create_compress_tree_compress();
}

void process_reverse_compress(){}
