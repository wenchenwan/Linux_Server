/*
以下信号集都是对我们自定义的信号集进行操作

int sigemptyset(sigset_t *set); 
    -功能：清空信号集中的数据，将信号集中的所有标志位置为0
    -参数：set 为一个传出参数，是我门需要操作的信号集
    -返回值：
        成功 0
        失败 -1
int sigfillset(sigset_t *set);
    -功能：将信号集中的所有标志位置为1
    -参数：set 为一个传出参数，是我门需要操作的信号集
    -返回值：
        成功 0
        失败 -1
int sigaddset(sigset_t *set, int signum);
    -功能：将信号集中的某一个标志位置为1，表示阻塞这个信号
    -参数：
        -set 为一个传出参数，是我门需要操作的信号集
        -signum 需要阻塞的那个信号
    -返回值：
        成功 0
        失败 -1
int sigdelset(sigset_t *set, int signum);
    -功能：将信号集中的某一个标志位置为0，表示不阻塞这个信号
    -参数：
        -set 为一个传出参数，是我门需要操作的信号集
        -signum 需要不阻塞的那个信号
    -返回值：
        成功 0
        失败 -1
int sigismember(const sigset_t *set, int signum);
    -功能：判断某个信号是否阻塞
    -参数：
        -set 是我门需要操作的信号集
        -signum 需要判断的那个信号
    -返回值：
        1 表示signum被阻塞
        0 表示signum不阻塞、
        -1 表示调用失败

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
*/
#include <stdio.h>
#include <signal.h>

int main(){
    // 创建一个信号集
    sigset_t set;

    // 清空信号集
    sigemptyset(&set);

    // 判断信号SIGINT是否在信号集中
    int flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);
    // 判断

    flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }

    flag = sigismember(&set,SIGQUIT);
    if(flag == 0){
        printf("SIGQUIT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGQUIT 阻塞\n");
    }
    // 删除信号
    sigdelset(&set,SIGQUIT);

    flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }

    flag = sigismember(&set,SIGQUIT);
    if(flag == 0){
        printf("SIGQUIT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGQUIT 阻塞\n");
    }

    return 0;
}