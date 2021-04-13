/*
       #include <sys/stat.h>

       int chmod(const char *pathname, mode_t mode);
       参数：
        -pathname文件目录
        -mode：需要修改的权限值
       返回值：
        成功 0
        失败 -2



*/
#include <sys/stat.h>
#include <stdio.h>

int main(){
    int re = chmod("a.txt",0775);
    if(re == -1){
        perror("chmod");
    }
    printf("修改成功");

    return 0;
}