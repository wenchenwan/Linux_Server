/*

    实现 ps aux | grep xxx
        父子进程通信
    子进程：ps aux，子进程结束后，将数据发送给父进程
    父进程：获取数据，过滤

    pipe()
    execlp()
    子进程将标准输出重定向到管道的写端。

    缓存区只有4k,需要循环的去写数据
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main(){
    int fd[2];
    int ret = pipe(fd);
    // 创建匿名管道
    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    // 创建子进程
    pid_t pid = fork();
    if(pid == 0){
        // 子进程
        // 文件描述符重定向 stdout_fileno -> fd[1]
        // 关闭读端
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        // 执行 ps aux
        execlp("ps","ps","aux",NULL);

    }else if(pid > 0){
        // 父进程
        // 关闭写端
        close(fd[1]);
        char buf[1024]={0};
        int len = -1;
        while((len = read(fd[0],buf,sizeof(buf)-1)) > 0){
            printf("%s",buf);
            memset(buf,0,sizeof(buf));
        }
        wait(NULL);
        
        // 从管道读取数据，并过滤输出
    }else if(pid == -1){
        perror("fork");
        exit(0);
    }


    return 0;
}
