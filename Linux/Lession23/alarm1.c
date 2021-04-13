/*
    一秒钟可以数多少个数字

    实际运行时间 = 内核时间 + 用户时间 + 消耗时间
    文件I/O比较耗费时间

    定时器和进程的状态无关，采用了自然定时法，无论进程处于什么状态alarm都会计时
    
*/

#include <unistd.h>
#include <stdio.h>

int main(){
    alarm(1);
    int i = 0;
    while (1)
    {
        printf("i = %d\n",i++);
    }
    
    return 0;

}