/*
    #include <unistd.h>
    unsigned int alarm(unsigned int seconds);
        -功能 ：设置定时器。函数调用开始倒计时，当倒计时为零时
            函数会给当前进程发送一个信号：SIGALARM
        -参数 ：
            second：倒计时的时长，单位：秒。如果参数为零，定时器无效（你进行倒计时，不发送信号）
                取消一个计时器，调用alarm，传递一个零alarm(0)
        -返回值：

    -SIGALARM：默认终止当前进程，每一个进程都有一个唯一的定时器。
    alarm(10);      -> 返回0
    过了一秒；
    alarm(5);       -> 返回9
    将数值进行重新设置 

    alarm(100) -> 该函数不阻塞
*/
#include <stdio.h>
#include <unistd.h>

int main(){
    int second = alarm(5);
    printf("second : %d\n",second);
    sleep(2);
    second = alarm(10);
    printf("second : %d\n",second);
    while (1)
    {
        /* code */
    }
    
    return 0;
}