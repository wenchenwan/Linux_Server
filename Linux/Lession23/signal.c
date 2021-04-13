/*
    #include <signal.h>
    sighandler_t signal(int signum, sighandler_t handler);
        -功能 ：设置某个信号的捕捉行为
        -参数 :
            -signum 要捕捉的信号
            -handler 捕捉到信号要如何处理
                -SIG_IGN ：忽略信号
                -SIG_DEF ：使用默认信号的行为
                -回调函数 ：这个函数是通过内核调用，程序员只负责进行写
                回调函数：
                    - 需要程序员实现，提前准备好的，函数的类型根据实际需求，看函数指针的定义
                    - 不是程序员调用，而是当信号产生，由内核调用
                    - 函数指针是实现回调的手段，函数实现之后，将函数名放到函数指针的位置就可以了。                    
        -返回值 ：
            成功：返回上一次注册的处理函数的地址，第一次调用返回NULL
            失败：返回SIG_ERR,设置错误信号
            
    SIGKILL SIGSTOP 不能捕捉，不能被忽略
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
    signal(SIGALRM,myProcess);
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
    // while(1){
        getchar();
    // }
    return 0;
}
void myProcess(int num){
    printf("回调函数被执行\n");
    printf("捕捉到的信号为：%d\n",num);

}