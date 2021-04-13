/*
    #include <unistd.h>
    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用来进程间通信。
        参数：int pipefd[2] 这个数组是一个传出参数。
            pipefd[0] 对应的是管道的读端
            pipefd[1] 对应的是管道的写端
        返回值：
            成功 0
            失败 -1

    管道默认是阻塞的：如果管道中没有数据，read阻塞，如果管道满了，write阻塞

    注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
*/

// 子进程发送数据给父进程，父进程读取到数据输出
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int pipnum[2]={0};
    int pip = pipe(pipnum);
    if(pip != 0){
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        printf("I am parent process pid: %d\n",getpid());
        close(pipnum[0]);
        char buff[1024]={0};
        while(1){
            // printf("I am parent process pid: %d\n",getpid());
            char * str = "I am chinese !!!";
            write(pipnum[1],str,strlen(str));
            //sleep(1);
            
            // read(pipnum[0],buff,sizeof(buff));
            // printf("parent : I read buff: %s pid : %d\n",buff,getpid());
            
            // bzero(buff, 1024);
        }
    }else if(pid == 0){
        // 子进程
        printf("I am child process pid: %d\n",getpid());
        close(pipnum[1]);
        char buf[1024]={0};
        while (1)
        {
            
            
            read(pipnum[0],buf,sizeof(buf));
            printf("child : I read buf : %s pid : %d\n",buf,getpid());
            bzero(buf, 1024);
            //sleep(1);

            // char * str1 = "hello world!!!";
            // write(pipnum[1],str1,strlen(str1));
            
            
        }
    }
    return 0;
}