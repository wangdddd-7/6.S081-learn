#include "kernel/types.h"
#include "user/user.h"
int main(){
    //文件描述符
    int ptoc_fd[2];
    int ctop_fd[2];
    //两个管道，父进程与子进程相互传递消息
    pipe(ptoc_fd);
    pipe(ctop_fd);
    //缓冲区存放传递的信息
    char buffer[] = {"a"};
    if(fork()==0){
         //子进程
        close(ptoc_fd[1]);
        close(ctop_fd[0]);
        read(ptoc_fd[0],buffer,1);
        printf("%d: received pong\n",getpid());
        write(ctop_fd[1],buffer,1);
    }
    else{
        //父进程
        close(ptoc_fd[0]);
        close(ctop_fd[1]);
        write(ptoc_fd[1],buffer,1);
        read(ctop_fd[0],buffer,1);
        printf("%d: received ping\n",getpid());
    }
    exit(0);
}