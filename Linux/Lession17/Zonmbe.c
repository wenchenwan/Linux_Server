/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){


    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //返回创建的子进程的进程号
        while(1){
            printf("I am parent process,pid :%d , ppid : %d\n",getpid(),getppid());
            sleep(1);
        }
        
    }
    if(pid == 0){
        // 当前是子进程
        sleep(1);
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
        // 构造出孤儿进程
    }
    for(int i = 0;i<6;i++){
        printf("i ：%d  pid: %d\n",i,getpid());
      
    }

    return 0;
}