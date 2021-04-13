/*       
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>

       int stat(const char *pathname, struct stat *statbuf);
            -获取文件相关的信息
            -参数
                -pathname 文件路径
                -*statbuf 结构体变量，传出参数，保存获取到的文件信息

            -返回值
                成功 返回 0
                失败 返回 -1 设置errno


       int fstat(int fd, struct stat *statbuf);
       int lstat(const char *pathname, struct stat *statbuf);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main(){

    struct stat statbuf;
    
    int ret = stat("a.txt",&statbuf);
    if(ret == -1){
        perror("stat ");
        return -1;
    }

    printf("size : %ld\n",statbuf.st_size);




    return 0;
}