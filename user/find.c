#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user.h"


char *basename(char *pathname) {//获取到/后的（包含/）字符串
    char *prev = 0;
    char *curr = strchr(pathname, '/');
    while (curr != 0) {
        prev = curr;
        curr = strchr(curr + 1, '/');
    }
    return prev;
}

void find(char a[],char b[]){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(a, O_RDONLY)) < 0){
        fprintf(2, "ls: cannot open %s\n", a);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", a);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            char *f_name = basename(a);
            int match = 1;
            if(f_name == 0 || strcmp(f_name + 1, b) != 0){//排除/导致不一致的情况
                match=0;
            }
            if(match){
                printf("%s\n",a);
            }
            close(fd);
            break;

        case T_DIR:
            memset(buf, 0, sizeof(buf));
            uint a_len = strlen(a);
            memcpy(buf, a, a_len);
            buf[a_len] = '/';
            p = buf + a_len + 1;
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
                    continue;
                memcpy(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, b); // recurse
            }
            close(fd);
            break;
    }
}

int main(int argc, char *argv[]){
    if(argc!=3){
        fprintf(2, "usages: find [source] [target]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}