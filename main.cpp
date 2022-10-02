#include "merge.h"
#include "compress.h"
#include "lock.h"

/* *** */
#include "base.c"

int main(){

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

    return 0;
}
