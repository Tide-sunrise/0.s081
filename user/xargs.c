#include "kernel/param.h"
#include "kernel/types.h"
#include "user.h"

#define buf_size 512

int main(int argc, char *argv[]){
    char buf[buf_size+1]={0};//this is buffer,memory data
    uint occupy=0;//this is the number of bytes that are occupied
    char *xargv[MAXARG]={0};
    int stdin_end=0;//whether the stdin is terminated

    for(int i=1;i<argc;i++){
        xargv[i-1]=argv[i];
    }

    while (!(stdin_end && occupy==0)){
        if(!stdin_end){
            int remain_size=buf_size-occupy;
            int read_bytes = read(0,buf+occupy,remain_size);
            if (read_bytes<0){
                // 读取失败，输出错误信息
                fprintf(2, "xargs: read returns -1 error\n");
            }
            if(read_bytes==0){
                close(0);
                stdin_end=1;
            }
            occupy+=read_bytes;
        }
        char *line_end = strchr(buf,'\n');
        while(line_end){
            char xbuf[buf_size+1]={0};
            memcpy(xbuf,buf,line_end-buf);
            xargv[argc-1]=xbuf;

            int ret = fork();
            if(ret==0){
                if(!stdin_end){
                    close(0);
                }
                if(exec(xargv[0],xargv)<0){
                    fprintf(2, "xargs: exec fails with -1\n");
                    exit(1);
                }
            }else{
                memmove(buf, line_end + 1, occupy - (line_end - buf + 1));
                // 更新缓冲区中已占用的字节数
                occupy -= line_end - buf + 1;
                // 将缓冲区剩余部分清零
                memset(buf + occupy, 0, buf_size - occupy);
                // 等待子进程结束，回收僵尸进程
                int pid;
                wait(&pid);

                // 继续查找缓冲区中是否还有换行符
                line_end = strchr(buf, '\n');
            }
        }
    }
    exit(0);
}