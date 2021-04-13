/*
    #include <sys/time.h>
    int setitimer(int which, const struct itimerval *new_value,
                        struct itimerval *old_value);
    
        - 功能：设置定时器（闹钟）。可以替代alarm函数。精度微妙us，可以实现周期性定时
        - 参数：
            - which : 定时器以什么时间计时
              ITIMER_REAL: 真实时间，时间到达，发送 SIGALRM   常用
              ITIMER_VIRTUAL: 用户时间，时间到达，发送 SIGVTALRM
              ITIMER_PROF: 以该进程在用户态和内核态下所消耗的时间来计算，时间到达，发送 SIGPROF

            - new_value: 设置定时器的属性
            
                struct itimerval {      // 定时器的结构体
                struct timeval it_interval;  // 每个阶段的时间，间隔时间
                struct timeval it_value;     // 延迟多长时间执行定时器
                };

                struct timeval {        // 时间的结构体
                    time_t      tv_sec;     //  秒数     
                    suseconds_t tv_usec;    //  微秒    
                };

            过10秒后，每个2秒定时一次
           
            - old_value ：记录上一次的定时的时间参数，一般不使用，指定NULL
        
        - 返回值：
            成功 0
            失败 -1 并设置错误号
*/
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


int main(){
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