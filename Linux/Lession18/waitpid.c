/*
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t waitpid(pid_t pid, int *wstatus, int options);
        -功能：回收指定进程号的的子进程，可以设置是否阻塞
        -参数：
            pid：
               pid > 0 表示回收的某个子进程的pid
               pid = 0 表示回收当前进程组的所有子进程
               pid = -1 表示回收所有得子进程
               pid < -1 表示回收某个进程组的组，id的绝对值
        -options
            0：阻塞
            WNOHANG：非阻塞

        -返回值：
            > 0 返回子进程id
            = 0 options=WNOHANGH，表示还有子进程或者错误
            = -1 错误，或者没有子进程了
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
            // int re = waitpid(-1,&state,0);
            // //阻塞,执行返回子进程的id

            int re = waitpid(-1,&state,WNOHANG);
            // 非阻塞

            if(re == -1){
                break;
            }else if(re == 0){
                //说明还有子进程存在 
                sleep(1);
                continue;
               
            }else if(re > 0){
                if(WIFEXITED(state)){
                    //正常退出
                    printf("退出的状态码 ：%d\n",WEXITSTATUS(state));
                }
                if(WIFSIGNALED(state)){
                    // 是否为异常终止
                    printf("被哪个信号干掉了 ：%d\n",WTERMSIG(state));
                }
                printf("child die, pid = %d\n",re);
            }

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