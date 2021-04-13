/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){



    int num = 0;
    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //返回创建的子进程的进程号
        printf("pid : %d\n",pid);
        printf("I am parent process,pid :%d , ppid : %d\n",getpid(),getppid());

        printf("parent process num = %d\n",num);
        num = num +10;
        printf("parent process num = %d\n",num);

    }
    if(pid == 0){
        //当前是子进程
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
        printf("parent process num = %d\n",num);
    }
    for(int i = 0;i<6;i++){
        printf("i ：%d  pid: %d\n",i,getpid());
        sleep(1);
    }

    return 0;
}