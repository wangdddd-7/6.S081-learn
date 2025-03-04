#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path,const char *filename){
    char buf[512],*p;
    int fd;
    // 声明与文件相关的结构体
    struct dirent de;
    struct stat st;
    //处理路径和文件报错
    if((fd = open(path,0)) < 0){
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }
    if(fstat(fd,&st)<0){
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return;
    }
    if(st.type != T_DIR){
        fprintf(2,"find: %s is not a directory\n",path);
        close(fd);
        return;
    }

    if(strlen(path) + 1 +DIRSIZ+1 > sizeof buf){
        fprintf(2,"find: directory too long\n");
        close(fd);
        return;
    }

    strcpy(buf,path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd,&de,sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0){
            continue;
        }
        if(!strcmp(de.name,".") || !strcmp(de.name,"..")){
            continue;
        }
        memmove(p,de.name,DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf,&st)<0){
            fprintf(2,"find: cannot stat %s\n",path);
            continue;
        }
        if(st.type == T_DIR){
            find(buf,filename);
        }
        else if(st.type == T_FILE && !strcmp(de.name,filename)){
            printf("%s\n",buf);
        }
    }
}
int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2,"usage: find dirname fileName\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}