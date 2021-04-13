/*
       #include <unistd.h>

       int access(const char *pathname, int mode);
        获取文件的权限，检查文件是否存在
        参数
            -pathname 文件目录
            -mode   指定的是可以检查的权限
                R_OK:判断文件是否有读权限
                W_OK:判断文件是否有写权限
                X_OK:判断文件是否有执行权限
                F_OK:判断文件是否存在
        返回值
            成功返回0
            失败返回-1


*/
#include <unistd.h>
#include <stdio.h>


int main(){

    int re = access("a.txt",F_OK);
    if(re == -1){
        perror("access");
        printf("文件不存在\n");
        return -1;

    }
    printf("文件存在\n");

    return 0;
}