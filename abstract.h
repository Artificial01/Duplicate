#ifndef FILE_ABSTRACT_H
#define FILE_ABSTRACT_H

#include "base.h"

#define ABSTRACT_ERROR_PATH_SIZE 4096

#define LEFT_ROTATE_32_ABSTRACT(number_1,number_2) ((number_1<<number_2)|(number_1>>(32-number_2)))

#define F_ABSTRACT(number_1,number_2,number_3) ((number_1&number_2)|((~number_1)&number_3))
#define G_ABSTRACT(number_1,number_2,number_3) ((number_1&number_3)|(number_2&(~number_3)))
#define H_ABSTRACT(number_1,number_2,number_3) (number_1^number_2^number_3)
#define I_ABSTRACT(number_1,number_2,number_3) (number_2^(number_1|(~number_3)))

bit_32 abstract_digest[4];

bit_32 abstract_a,abstract_b,abstract_c,abstract_d;

bit_32 abstract_data[16];
bit_32 abstract_data_index[64] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                                  1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
                                  5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
                                  0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9};

bit_32 abstract_s[64] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
                         5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
                         4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
                         6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};

bit_32 abstract_t[64] = {0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
                         0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
                         0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
                         0x6b901122,0xfd987193,0xa679438e,0x49b40821,
                         0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
                         0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
                         0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
                         0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
                         0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
                         0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
                         0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
                         0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
                         0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
                         0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
                         0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
                         0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};

char abstract_error_path[ABSTRACT_ERROR_PATH_SIZE];
int abstract_error_path_index;

void F_abstract(bit_32 * a_pointer, bit_32 b, bit_32 c, bit_32 d, bit_32 data, bit_32 s, bit_32 t){
    *a_pointer = b+LEFT_ROTATE_32_ABSTRACT((*a_pointer+F_ABSTRACT(b,c,d)+data+t),s);
}
void G_abstract(bit_32 * a_pointer, bit_32 b, bit_32 c, bit_32 d, bit_32 data, bit_32 s, bit_32 t){
    *a_pointer = b+LEFT_ROTATE_32_ABSTRACT((*a_pointer+G_ABSTRACT(b,c,d)+data+t),s);
}
void H_abstract(bit_32 * a_pointer, bit_32 b, bit_32 c, bit_32 d, bit_32 data, bit_32 s, bit_32 t){
    *a_pointer = b+LEFT_ROTATE_32_ABSTRACT((*a_pointer+H_ABSTRACT(b,c,d)+data+t),s);
}
void I_abstract(bit_32 * a_pointer, bit_32 b, bit_32 c, bit_32 d, bit_32 data, bit_32 s, bit_32 t){
    *a_pointer = b+LEFT_ROTATE_32_ABSTRACT((*a_pointer+I_ABSTRACT(b,c,d)+data+t),s);
}

void core_abstract(){
    abstract_a = abstract_digest[0];
    abstract_b = abstract_digest[1];
    abstract_c = abstract_digest[2];
    abstract_d = abstract_digest[3];
    for(int index=0; index<64; index++){
        void (* pointer)(bit_32 *,bit_32,bit_32,bit_32,bit_32,bit_32,bit_32);
        switch(index/16){
            case 0:
                pointer = F_abstract;
                break;
            case 1:
                pointer = G_abstract;
                break;
            case 2:
                pointer = H_abstract;
                break;
            case 3:
                pointer = I_abstract;
                break;
            default:
                pointer = NULL;
                break;
        }
        switch(index%4){
            case 0:
                (*pointer)(&abstract_a,abstract_b,abstract_c,abstract_d,abstract_data[abstract_data_index[index]],abstract_s[index],abstract_t[index]);
                break;
            case 1:
                (*pointer)(&abstract_d,abstract_a,abstract_b,abstract_c,abstract_data[abstract_data_index[index]],abstract_s[index],abstract_t[index]);
                break;
            case 2:
                (*pointer)(&abstract_c,abstract_d,abstract_a,abstract_b,abstract_data[abstract_data_index[index]],abstract_s[index],abstract_t[index]);
                break;
            case 3:
                (*pointer)(&abstract_b,abstract_c,abstract_d,abstract_a,abstract_data[abstract_data_index[index]],abstract_s[index],abstract_t[index]);
                break;
            default:
                break;
        }
    }
    abstract_digest[0]+=abstract_a;
    abstract_digest[1]+=abstract_b;
    abstract_digest[2]+=abstract_c;
    abstract_digest[3]+=abstract_d;
}

void MD5_abstract(void * file, long size){
    abstract_digest[0] = 0x67452301;
    abstract_digest[1] = 0xEFCDAB89;
    abstract_digest[2] = 0x98BADCFE;
    abstract_digest[3] = 0x10325476;
    for(int i=0; i<size/64; i++){
        for(int j=0; j<16; j++){
            abstract_data[j] = ((bit_32 *)file)[16*i+j];
        }
        core_abstract();
    }
    for(int i=0; i<size%64; i++){
        ((byte *)abstract_data)[i] = *(((byte *)file)+size/64*64+i);
    }
    if(size%64>56){
        for(int i=size%64; i<64; i++){
            ((byte *)abstract_data)[i] = 0;
        }
        core_abstract();
        for(int i=0; i<56; i++){
            ((byte *)abstract_data)[i] = 0;
        }
        *((bit_64 *)(((byte *)abstract_data)+56)) = size*BIT_AMOUNT;
        core_abstract();
    }else{
        for(int i=size%64; i<56; i++){
            ((byte *)abstract_data)[i] = 0;
        }
        *((bit_64 *)(((byte *)abstract_data)+56)) = size*BIT_AMOUNT;
        core_abstract();
    }
}

void concatenate_abstract(char * path){
    abstract_error_path[abstract_error_path_index] = '|';
    abstract_error_path_index++;
    while(*path!='\0'){
        abstract_error_path[abstract_error_path_index] = *path;
        abstract_error_path_index++;
        path++;
    }
    abstract_error_path[abstract_error_path_index] = '\0';
}

void file_abstract(){
    file_memory * file_memory_pointer = load_file(folder_path);
    MD5_abstract(file_memory_pointer->file,file_memory_pointer->size);
    free_file_memory(file_memory_pointer);
    FILE * file_pointer = fopen(folder_path,"ab");
    fwrite(abstract_digest,4,4,file_pointer);
    fclose(file_pointer);
}

void folder_abstract(){
    DIR * folder_pointer = opendir(folder_path);
    for(struct dirent * folder_entry_pointer=readdir(folder_pointer); folder_entry_pointer; folder_entry_pointer=readdir(folder_pointer)){
        if(strcmp(folder_entry_pointer->d_name,".")!=0 && strcmp(folder_entry_pointer->d_name,"..")!=0){
            enter_folder(folder_entry_pointer->d_name);
            if(identify_folder(folder_path)){
                folder_abstract();
            }else{
                file_abstract();
            }
            exit_folder();
        }
    }
}

void process_abstract(char * path){
    init_folder(path);
    if(identify_folder(path)){
        folder_abstract();
    }else{
        file_abstract();
    }
}

void file_reverse_abstract(){
    file_memory * file_memory_pointer = load_file(folder_path);
    file_memory_pointer->size-=16;
    MD5_abstract(file_memory_pointer->file,file_memory_pointer->size);
    for(int i=0; i<4; i++){
        if(((bit_32 *)(((byte *)file_memory_pointer->file)+file_memory_pointer->size))[i]!=abstract_digest[i]){
            concatenate_abstract(folder_path);
            free_file_memory(file_memory_pointer);
            return;
        }
    }
    store_file(folder_path,file_memory_pointer);
    free_file_memory(file_memory_pointer);
}

void folder_reverse_abstract(){
    DIR * folder_pointer = opendir(folder_path);
    for(struct dirent * folder_entry_pointer=readdir(folder_pointer); folder_entry_pointer; folder_entry_pointer=readdir(folder_pointer)){
        if(strcmp(folder_entry_pointer->d_name,".")!=0 && strcmp(folder_entry_pointer->d_name,"..")!=0){
            enter_folder(folder_entry_pointer->d_name);
            if(identify_folder(folder_path)){
                folder_reverse_abstract();
            }else{
                file_reverse_abstract();
            }
            exit_folder();
        }
    }
}

char * process_reverse_abstract(char * path){
    init_folder(path);
    abstract_error_path[0] = '\0';
    abstract_error_path_index = 0;
    if(identify_folder(path)){
        folder_reverse_abstract();
    }else{
        file_reverse_abstract();
    }
    return abstract_error_path;
}

#endif