#include "base.h"

file_memory * load_file(char * file_path){
    FILE * file_pointer = fopen(file_path,"rb");
    file_memory * file_memory_pointer = (file_memory *)malloc(sizeof(file_memory));
    fseek(file_pointer,0,SEEK_END);
    file_memory_pointer->size = ftell(file_pointer);
    file_memory_pointer->file = (void *)malloc(file_memory_pointer->size);
    rewind(file_pointer);
    fread(file_memory_pointer->file,IO_UNIT_SIZE,file_memory_pointer->size/IO_UNIT_SIZE,file_pointer);
    fread((void *)((char *)file_memory_pointer->file+file_memory_pointer->size/IO_UNIT_SIZE*IO_UNIT_SIZE),1,file_memory_pointer->size%IO_UNIT_SIZE,file_pointer);
    fclose(file_pointer);
    return file_memory_pointer;
}

void store_file(char * file_path, file_memory * file_memory_pointer){
    FILE * file_pointer = fopen(file_path,"wb");
    fwrite(file_memory_pointer->file,IO_UNIT_SIZE,file_memory_pointer->size/IO_UNIT_SIZE,file_pointer);
    fwrite((void *)((char *)file_memory_pointer->file+file_memory_pointer->size/IO_UNIT_SIZE*IO_UNIT_SIZE),1,file_memory_pointer->size%IO_UNIT_SIZE,file_pointer);
    fclose(file_pointer);
}

void update_bit(bit_pointer * bit_pointer_pointer, unsigned char storage, int amount){
    int offset = BIT_OF_BYTE_AMOUNT-bit_pointer_pointer->bit_index-amount;
    if(offset>0){
        *(bit_pointer_pointer->character_pointer)|=(unsigned char)(storage<<(unsigned char)offset);
        bit_pointer_pointer->bit_index+=amount;
    }else{
        *(bit_pointer_pointer->character_pointer)|=(unsigned char)(storage>>(unsigned char)(-offset));
        bit_pointer_pointer->character_pointer++;
        *(bit_pointer_pointer->character_pointer)|=(unsigned char)(storage<<(unsigned char)(BIT_OF_BYTE_AMOUNT+offset));
        bit_pointer_pointer->bit_index=-offset;
    }
}

int fetch_bit(bit_pointer * bit_pointer_pointer){
    int result;
    bit_pointer_pointer->bit_index++;
    if(bit_pointer_pointer->bit_index==BIT_OF_BYTE_AMOUNT){
        bit_pointer_pointer->character_pointer++;
        bit_pointer_pointer->bit_index = 0;
        result = (*(bit_pointer_pointer->character_pointer-1))&(unsigned char)1U;
    }else{
        result = (*(bit_pointer_pointer->character_pointer))&(unsigned char)(1U<<(unsigned char)(BIT_OF_BYTE_AMOUNT-bit_pointer_pointer->bit_index));
    }
    return result!=0;
}
