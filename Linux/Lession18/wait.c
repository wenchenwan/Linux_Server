/*
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *wstatus);
        -功能：
            等待任意一个子进程，如果任意子进程结束了，次函数会回收子进程的资源
        -参数：
            int *wstatus
            进程退出的状态信息，传入一个int类型的地址，传出参数
        -返回值：
            -成功 返回被回收的子进程ID
            -失败 返回-1
    调用wait函数，进程会被挂起（阻塞），知道它的一个子进程收到一个不能被忽略的信号才被唤醒
    （相当于继续向下执行）
    如果没有子进程，函数立即返回，返回-1，如果子进程已经结束了，也会立即返回-1，并且回收子进程的资源

*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    // 一个父进程，创建五个子进程
    pid_t pid;
    // 创建五个子进程
    for(int i=0;i<5;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    } 

    if(pid > 0){
        //父进程
        while(1){
            printf("parent, pid = %d\n",getpid());
            // int re = wait(NULL);
            int state;
            
            int re = wait(&state);
            //阻塞,执行返回子进程的id
            if(re == -1){
                break;
            }
            if(WIFEXITED(state)){
                //正常退出
                printf("退出的状态码 ：%d\n",WEXITSTATUS(state));
            }
            if(WIFSIGNALED(state)){
                // 是否为异常终止
                printf("被哪个信号干掉了 ：%d\n",WTERMSIG(state));
            }
            printf("child die, pid = %d\n",re);
            sleep(1);
        }    
    }else if(pid == 0){
        // 子进程
        while(1){
            printf("child, pid = %d\n",getpid());
            sleep(1);
        }
        //exit(0);
    }
    return 0;
}