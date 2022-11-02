#include "base.h"
#include "merge.h"
#include "abstract.h"
#include "compress.h"
#include "lock.h"

int main(){

    char path[32] = "002";

    int choice;
    while(1){
        printf("Input: ");
        scanf("%d",&choice);
        if(choice==1){
            process_compress(path);
        }else if(choice==2){
            process_reverse_compress(path);
        }else if(choice==3){
            process_abstract(path);
        }else if(choice==4){
            printf("%s",process_reverse_abstract(path));
        }else if(choice==5){
            process_merge(path);
        }else if(choice==6){
            process_reverse_merge(path);
        }else{
            break;
        }
    }

    return 0;
}
