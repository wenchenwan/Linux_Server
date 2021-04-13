#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
int main() {

    /*
    设置非阻塞
    int flag = fcntl(fd[0],F_GETFL) //获取原来文件的标记、
    flag |= O_NOBLOCK;
    fcntl(fd[0],F_SETFL,flag);
    */
    // 在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1) {
        perror("pipe");
        exit(0);
    }

    // 创建子进程
    pid_t pid = fork();
    if(pid > 0) {
        // 父进程
        printf("i am parent process, pid : %d\n", getpid());

        // 关闭写端
        close(pipefd[1]);
        int flag = fcntl(pipefd[0],F_GETFL); //获取原来文件的标记、
        flag |= O_NONBLOCK;
        fcntl(pipefd[0],F_SETFL,flag);
        // 从管道的读取端读取数据
        char buf[1024] = {0};
        while(1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len = %d\n",len);
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            memset(buf,0,sizeof(buf));
            sleep(1);
        }

    } else if(pid == 0){
        // 子进程
        printf("i am child process, pid : %d\n", getpid());
        // 关闭读端
        close(pipefd[0]);
        char buf[1024] = {0};
        while(1) {
            // 向管道中写入数据
            char * str = "hello,i am child";
            write(pipefd[1], str, strlen(str));
            sleep(2);
        }
        
    }
    return 0;
}


