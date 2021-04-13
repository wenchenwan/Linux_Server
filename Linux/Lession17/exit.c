/*
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);
    -status参数：是进程退出时候的一个状态信息，父进程回收子进程的资源时候可以获得

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    printf("hello\n");
    printf("world!");
    //exit(0);
    // 执行成功，return 0就不会再执行
    _exit(0);

    return 0;
}