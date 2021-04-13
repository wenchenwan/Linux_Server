/*
       #include <sys/stat.h>
       #include <sys/types.h>


       int mkdir(const char *pathname, mode_t mode);
            作用：创建一个目录
            参数
            -pathname 创建的目录的路径
            -mode 文件权限（八进制 ）
            返回值
            成功 0
            失败 -1

*/
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(){

    int re = mkdir("Dir",0777);
    if(re == -1){
        perror("mkdir\n");
        return -1;
    }

    printf("创建成功\n");

    return 0;
}