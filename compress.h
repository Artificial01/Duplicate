#ifndef DUPLICATE_COMPRESS_H
#define DUPLICATE_COMPRESS_H

#include "base.h"

#define COMPRESS_UNIT_CATEGORY 256
#define COMPRESS_TREE_SIZE 511

typedef struct compress_node{
    int parent;
    int parent_sign;
    int child_0;
    int child_1;
} compress_node;

long compress_unit[COMPRESS_UNIT_CATEGORY];
compress_node compress_tree[COMPRESS_TREE_SIZE];

bit_pointer compress_file;
long compress_bit_amount;

void process_compress(file_memory * file_memory_pointer);
void process_reverse_compress(file_memory * file_memory_pointer);

#endif