/*

#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
    -功能 ：给任何的某个进程或者进程组发送信号，发送某个信号
    -参数 ：
        -pid ：需要发送给进程的pid
            pid > 0     将信号发送到指定的进程
            pid = 0     将信号发给当前的进程组
            pid = -1    将信号发给每一个有权限接受该信号的进程
            pid < -1    将信号发送给某个进程组（pid为进程组的pid的绝对值）
        -sig ：需要发送信号的编号或者宏值

    kill(getpid(),sig);

int raise(int sig);
    -功能 ：给当前进程发送信号
    -参数 ：要发送的信号或者信号的宏值
    -返回值 ：如果成功返回0，如果失败返回-1

void abort(void);
    -功能 ：发送SIGABRT信号给当前的进程杀死当前进程
    kill(getpid(),SIGABRT);
unsigned int alarm(unsigned int seconds);
int setitimer(int which, const struct itimerval *new_val,
                struct itimerval *old_value);
*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    pid_t pid = fork();
    if(pid == 0){
        // 子进程
        for(int i = 0;i<5;i++){
            printf("child process\n");
            sleep(1);
        }
    }else if(pid > 0){
        // 父进程
        printf("parent process\n");
        sleep(2);
        printf("Kill child process\n");
        kill(pid,SIGABRT);

    }else{
        perror("fork");
        exit(0);
    }

    return 0;
}