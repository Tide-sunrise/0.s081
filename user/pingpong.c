#include "kernel/types.h"
#include "user.h"

int main(int paranum,char* para){
    int pid;
    int p1[2],p2[2];
    char buf[]={'a'};
    pipe(p1);
    pipe(p2);

    int ret = fork();
    if(ret==0){
        pid=getpid();
        close(p1[0]);
        close(p2[0]);
        read(p1[1],buf,1);
        printf("%d: received ping\n",pid);
        write(p2[1],buf,1);
    }else{
        pid=getpid();
        close(p1[1]);
        close(p2[1]);
        write(p2[0],buf,1);
        read(p1[0],buf,1);
        printf("%d: received pong\n",pid);
    }
    exit(0);
}