#include "base.h"
#include "merge.h"
#include "abstract.h"
#include "compress.h"
#include "lock.h"

int main(){
    char path[32] = "002";
    int choice;
    while(1){
        puts("INPUT 1(compress) 2(reverse compress) 3(abstract) 4(reverse abstract) 5(merge) 6(reverse merge) 7(lock) 8(reverse lock) TO CHANGE FOLDER/FILE NAMED 002");
        scanf("%d",&choice);
        if(choice==1){
            process_compress(path);
        }else if(choice==2){
            process_reverse_compress(path);
        }else if(choice==3){
            process_abstract(path);
        }else if(choice==4){
            printf("%s\n",process_reverse_abstract(path));
        }else if(choice==5){
            process_merge(path);
        }else if(choice==6){
            process_reverse_merge(path);
        }else if(choice==7){
            process_lock(path,(byte *)"1234567812345678");
        }else if(choice==8){
            process_reverse_lock(path,(byte *)"1234567812345678");
        }else{
            break;
        }
    }
}
