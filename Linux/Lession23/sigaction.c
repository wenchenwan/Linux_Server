/*
    #include <signal.h>
    int sigaction(int signum, const struct sigaction *act,
                            struct sigaction *oldact);

        - 功能：检查或者改变信号的处理。信号捕捉 
        - 参数：
            - signum : 需要捕捉的信号的编号或者宏值（信号的名称）
            - act ：捕捉到信号之后的处理动作
            - oldact : 上一次对信号捕捉相关的设置，一般不使用，传递NULL
        - 返回值：
            成功 0
            失败 -1

     struct sigaction {
        // 函数指针，指向的函数就是信号捕捉到之后的处理函数
        void     (*sa_handler)(int);
        // 不常用,第一个参数，int信号捕捉到的编号，siginfo保存信号相关的信息
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        // 临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号。
        sigset_t   sa_mask;
        // 使用哪一个信号处理对捕捉到的信号进行处理
        // 这个值可以是0，表示使用sa_handler,也可以是SA_SIGINFO表示使用sa_sigaction
        int        sa_flags;
        // 被废弃掉了
        void     (*sa_restorer)(void);
    };

*/
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
void myProcess(int num);

int main(){

    // 注册信号捕捉
    // signal(SIGALRM,SIG_IGN);
    // signal(SIGALRM,SIG_DFL);
    // void ( * sighandler) (int); 函数指针,int为信号的编号
    // signal(SIGALRM,myProcess);
    struct sigaction set;
    set.sa_flags = 0;
    set.sa_handler = myProcess;
    sigemptyset(&set.sa_mask);
    // 清空临时阻塞信号集

    sigaction(SIGALRM,&set,NULL);
    struct itimerval newvalue;
    // 设置间隔时间
    newvalue.it_interval.tv_sec = 2;
    newvalue.it_interval.tv_usec = 0;
    // 设置延迟时间
    newvalue.it_value.tv_sec = 3;
    newvalue.it_value.tv_usec = 0;

    int re = setitimer(ITIMER_REAL,&newvalue,NULL);
    printf("定时器开始了！\n");
    if(re == -1){
        perror("setitimer");
        exit(0);
    }
    while(1){
        getchar();
    }
    return 0;
}
void myProcess(int num){
    printf("回调函数被执行\n");
    printf("捕捉到的信号为：%d\n",num);

}