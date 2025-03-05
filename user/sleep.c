#include "kernel/types.h"
#include "user.h"

int main(int a,char *para[]){
    if(a!=2){
        fprintf(2,"damn,what can I say?");
    }

    int stall= atoi(para[1]);

    exit(sleep(stall));
}