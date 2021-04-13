/*
       #include <unistd.h>
       #include <sys/types.h>

       int truncate(const char *path, off_t length);
        作用：缩减或者增加文件的尺寸
        参数：
            -pathname
            -length 目标文件的长度

*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    int re = truncate("b.txt",20);

    if(re == -1){
        perror("truncate");
        return -1;
    }
    printf("更改成功");
    return 0;
}