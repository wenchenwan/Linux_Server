/*
    SIGCHILD信号产生的三个条件：
        1.子进程结束
        2.子进程暂停了
        3.子进程继续运行
        都会给父进程发送该信号，父进程默认忽略该信号
    使用SIGCHILD解决僵尸进程问题
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void MyFun(int num){
    printf("捕捉到的信号 ： %d\n",num);
    while (1)
    {
        //wait(NULL); //阻塞

        // 提前设置好阻塞信号集，阻塞SIGCHLD，因为子进程可能很快结束，信号没有被捕捉

        int re = waitpid(-1,NULL,WNOHANG);
        if(re > 0){
            printf("child die , pid = %d\n",re);
        }else if(re == 0){
            break;
        }else if(re == -1){
            perror("waitpid");
            exit(0);
        }
    }
}
int main(){

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    pid_t pid;
    // // 捕捉子进程死亡时发送的SIGCHILD信号
    // struct sigaction sig;
    // sig.sa_flags = 0;
    // sig.sa_handler = MyFun;
    // sigemptyset(&sig.sa_mask);
    // sigaction(SIGCHLD,&sig,NULL);

    for(int i = 0;i<20;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }
    if(pid > 0){
        // // 父进程
        // // 捕捉子进程死亡时发送的SIGCHILD信号
        struct sigaction sig;
        sig.sa_flags = 0;
        sig.sa_handler = MyFun;
        sigemptyset(&sig.sa_mask);
        sigaction(SIGCHLD,&sig,NULL);
        // 可能出现子进程已经结束，然而信号未捕捉到
        // 注册完信号捕捉以后
        sigprocmask(SIG_UNBLOCK,&set,NULL);

        while (1)
        {
            printf("parent process pid : %d\n",getpid());
            sleep(1);
        }
    }else if(pid == 0){
        // 子进程
        printf("child process pid : %d\n",getpid());
    }
    return 0;
}