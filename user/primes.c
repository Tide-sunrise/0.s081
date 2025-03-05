#include "kernel/types.h"
#include "user.h"

void kid(int listen){
    int forks=0;
    int own_num=0;
    int num=0;
    int pi[2];
    while(1){
        int tmp= read(listen,&num,4);

        if(tmp==0){
            close(listen);
            if(forks) {
                close(pi[1]);
                int child_pid;
                wait(&child_pid);
            }
            exit(0);
        }
        if(own_num==0){
            own_num=num;
            printf("prime %d\n",own_num);
        }
        if(num%own_num!=0){
            if(!forks){
                pipe(pi);
                forks=1;
                int ret =fork();
                if(ret==0){
                    close(listen);
                    close(pi[1]);
                    kid(pi[0]);
                }else{
                    close(pi[0]);
                }
            }
            write(pi[1],&num,4);
        }
    }
}

int main(){
    int p[2];
    pipe(p);
    for(int i=2;i<=35;i++){
        write(p[1],&i,4);
    }
    close(p[1]);
    kid(p[0]);
    exit(0);
}