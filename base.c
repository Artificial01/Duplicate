#include "base.h"

file_memory load_file(char * file_path){
    FILE * file_pointer = fopen(file_path,"rb");
    file_memory file_memory = {NULL,0};
    fseek(file_pointer,0,SEEK_END);
    file_memory.size = ftell(file_pointer);
    file_memory.file = (void *)malloc(file_memory.size);
    rewind(file_pointer);
    fread(file_memory.file,IO_UNIT_SIZE,file_memory.size/IO_UNIT_SIZE,file_pointer);
    fread((void *)((char *)file_memory.file+file_memory.size/IO_UNIT_SIZE*IO_UNIT_SIZE),1,file_memory.size%IO_UNIT_SIZE,file_pointer);
    fclose(file_pointer);
    return file_memory;
}

void store_file(char * file_path, file_memory file_memory){
    FILE * file_pointer = fopen(file_path,"wb");
    fwrite(file_memory.file,IO_UNIT_SIZE,file_memory.size/IO_UNIT_SIZE,file_pointer);
    fwrite((void *)((char *)file_memory.file+file_memory.size/IO_UNIT_SIZE*IO_UNIT_SIZE),1,file_memory.size%IO_UNIT_SIZE,file_pointer);
    fclose(file_pointer);
}
