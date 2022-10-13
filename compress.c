#include "compress.h"

void create_compress_unit_compress(const unsigned char * byte, long amount){
    for(int i=0; i<COMPRESS_UNIT_CATEGORY; i++){
        compress_unit[i] = 0;
    }
    for(int i=0; i<amount; i++){
        compress_unit[byte[i]]++;
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

void transform_compress(int compress_node_index){
    if(compress_tree[compress_node_index].parent==-1){
        return;
    }
    transform_compress(compress_tree[compress_node_index].parent);
    update_bit(&compress_file,compress_tree[compress_node_index].parent_sign,1);
    compress_bit_amount++;
}

void process_compress(file_memory * file_memory_pointer){
    create_compress_unit_compress((const unsigned char *)file_memory_pointer->file,file_memory_pointer->size);
    create_compress_tree_compress();
    void * file = (void *)malloc(sizeof(compress_tree)+sizeof(long)+file_memory_pointer->size);
    compress_file.character_pointer = ((unsigned char *)file)+sizeof(compress_tree)+sizeof(long);
    compress_file.bit_index = 0;
    for(int i=0; i<file_memory_pointer->size; i++){
        compress_file.character_pointer[i] = 0;
    }
    compress_bit_amount = 0;
    for(int i=0; i<file_memory_pointer->size; i++){
        transform_compress(((unsigned char *)(file_memory_pointer->file))[i]);
    }
    free(file_memory_pointer->file);
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        ((compress_node *)file)[i] = compress_tree[i];
    }
    *((long *)(((compress_node *)file)+COMPRESS_TREE_SIZE)) = file_memory_pointer->size;
    file_memory_pointer->file = file;
    file_memory_pointer->size = compress_bit_amount/8+(compress_bit_amount%8!=0);
}

void process_reverse_compress(file_memory * file_memory_pointer){
    for(int i=0; i<COMPRESS_TREE_SIZE; i++){
        compress_tree[i] = ((compress_node *)(file_memory_pointer->file))[i];
    }
    file_memory_pointer->size = *((long *)(((compress_node *)(file_memory_pointer->file))+COMPRESS_TREE_SIZE));
    compress_file.character_pointer = ((unsigned char *)(file_memory_pointer->file))+sizeof(compress_tree)+sizeof(long);
    compress_file.bit_index = 0;
    void * file = (void *)malloc(file_memory_pointer->size);
    for(int i=0; i<file_memory_pointer->size; i++){
        int compress_node_index = COMPRESS_TREE_SIZE-1;
        while(compress_tree[compress_node_index].child_0!=-1){
            if(fetch_bit(&compress_file)){
                compress_node_index = compress_tree[compress_node_index].child_1;
            }else{
                compress_node_index = compress_tree[compress_node_index].child_0;
            }
        }
        ((unsigned char *)file)[i] = compress_node_index;
    }
    free(file_memory_pointer->file);
    file_memory_pointer->file = file;
}
