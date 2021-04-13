/*
写一个守护进程，每隔两秒获取一下系统时间，并将这个时间写入磁盘文件
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void myFun(int num){
    // 捕捉到信号，获取系统时间，写入磁盘
    printf("This is my func\n");
    time_t t1 = time(NULL);
    struct tm *loc = localtime(&t1);
    char buff[1024];
    // sprintf(buff,"%d-%d-%d %d:%d:%d\n",loc->tm_year,loc->tm_mon,loc->tm_mday,loc->tm_hour,loc->tm_min,loc->tm_sec);
    // printf("%s\n",buff);
    char * str = asctime(loc);
    int fd = open("/home/wen/Linux/log.txt",O_RDWR | O_CREAT | O_APPEND,0664);
    write(fd,str,strlen(str));

    close(fd);
}
int main(){
    // 1.创建子进程，杀死父进程
    pid_t pid = fork();
    if(pid > 0){
        exit(0);
    }

    // 2.将子进程重新创建一个会话
    setsid();

    // 3.设置掩码
    umask(022);

    // 4.更改工作目录
    chdir("/");

    // 5.关闭，重定向文件描述符
    int fd = open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    // 6.业务逻辑

    // 信号捕捉
    // signal(SIGALRM,myFun);
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myFun;

    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM,&act,NULL);
    
    // 创建定时器
    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL,&val,NULL);
    while (1)
    {
        sleep(10);
    }
    

    return 0;
}