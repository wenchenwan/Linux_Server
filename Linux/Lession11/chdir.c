/*
    #include <unistd.h>
    int chdir(const char *path);
    作用：
        修改进程的工作目录
    参数：
        path 需要修改的工作目录

    #include <unistd.h>
    char *getcwd(char *buf, size_t size);
    作用：
        获取当前的工作目录
    参数
        -buf 存储的路径指向一个数组
        -size 数组的大小

    返回值
        返回指向的一块内存，这个数据就是第一个参数

 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

    char buf[128];
    getcwd(buf,sizeof(buf));

    printf("当前的工作目录：%s\n",buf);

    int re = chdir("/home/wen/Linux/Lession10");
    if(re == -1){
        perror("chdir\n");
        return -1;
    }

    //创建一个新的文件
    int fd = open("chdir.txt",O_RDWR | O_CREAT,0777);
    if(fd == -1){
        perror("open");
        return -1;
    }

    close(fd);

    getcwd(buf,sizeof(buf));
    printf("当前的工作目录：%s\n",buf);

    return 0;
}
