/*
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

       int open(const char *pathname, int flags, mode_t mode);
        -pathname  要打开的文件路径
        -flags：   对文件的操作权限
            O_RDONLY, O_WRONLY, or O_RDWR.
            O_CREAT
            If pathname does not exist, create it as a regular file.
        -mode
            八进制的数，表示创建出的新的文件的操作权限，比如0775
            (mode & ~umask)->抹去一些权限
            umask  0002
            设置umask的值-> umask 0003    只在当前终端有效

            0777    ->  111111111
        &    0775    ->  111111101
            -> 111111101
        drw （当前用户权限）xrw （当前组的权限）xr-x（其他权限） 2 wen wen 4096 Mar 16 18:52 openFile

        flags int类型占有4个字节，32位
        flags 32 位都是标志位

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    int fd = open("craeat.txt",O_RDWR|O_CREAT,0777);
    if(fd==-1){
        perror("open");        
    }

    close(fd);

    return 0;

}