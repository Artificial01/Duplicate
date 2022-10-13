#include "base.c"
#include "compress.c"
#include "lock.c"
#include "merge.c"

void debug_1(){
    char * load_path = (char *)malloc(10);
    load_path[0]='L';
    load_path[1]='O';
    load_path[2]='A';
    load_path[3]='D';
    load_path[4]='.';
    load_path[5]='e';
    load_path[6]='x';
    load_path[7]='e';
    load_path[8]='\0';
    load_path[9]='\0';

    char * store_path = (char *)malloc(10);
    store_path[0]='S';
    store_path[1]='T';
    store_path[2]='O';
    store_path[3]='R';
    store_path[4]='E';
    store_path[5]='.';
    store_path[6]='e';
    store_path[7]='x';
    store_path[8]='e';
    store_path[9]='\0';

    store_file(store_path,load_file(load_path));
}

void debug_2(){
    auto * file = (unsigned char *)malloc(10);
    for(int i=0; i<10; i++){
        file[i] = 0;
    }

    bit_pointer bit_pointer = {file,0};
    update_bit(&bit_pointer,0b11,2);
    update_bit(&bit_pointer,0b101,3);
    update_bit(&bit_pointer,0b1001,4);
    update_bit(&bit_pointer,0b11,2);
    update_bit(&bit_pointer,0b101,3);
    update_bit(&bit_pointer,0b1001,4);
    update_bit(&bit_pointer,0b11,2);
    update_bit(&bit_pointer,0b101,3);
    update_bit(&bit_pointer,0b1001,4);

    bit_pointer.character_pointer = file;
    bit_pointer.bit_index = 0;
    for(int i=0; i<80; i++){
        printf("%d ",fetch_bit(&bit_pointer));
    }
}

void debug_3(){
    char * load_path = (char *)malloc(10);
    load_path[0]='L';
    load_path[1]='O';
    load_path[2]='A';
    load_path[3]='D';
    load_path[4]='.';
    load_path[5]='e';
    load_path[6]='x';
    load_path[7]='e';
    load_path[8]='\0';
    load_path[9]='\0';

    char * store_path = (char *)malloc(10);
    store_path[0]='S';
    store_path[1]='T';
    store_path[2]='O';
    store_path[3]='R';
    store_path[4]='E';
    store_path[5]='.';
    store_path[6]='e';
    store_path[7]='x';
    store_path[8]='e';
    store_path[9]='\0';

    file_memory * file_memory_pointer = load_file(load_path);
    process_compress(file_memory_pointer);
    process_reverse_compress(file_memory_pointer);
    store_file(store_path,file_memory_pointer);
}

int main(){

    debug_3();

    return 0;
}
