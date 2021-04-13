/*

       #include <unistd.h>

       ssize_t read(int fd, void *buf, size_t count);
        -fd 文件描述符
        -buff 文件缓冲区，数组的地址
        -count 指定的数组大小


        return value
        成功：
        = 0 读完文件
        > 0 返回实际读到的字节数
        失败：
        =-1，并设置errno



       #include <unistd.h>
       ssize_t write(int fd, const void *buf, size_t count);
       同上


*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd1 = open("english.txt",O_RDONLY);
    char buff[1024] = {};

    if(fd1 == -1){
        perror("open file");
    }

    int fd2 = open("copy.txt",O_WRONLY|O_CREAT,0664);
    //0664为赋予文件的权限
    int byte_sum = 0;
    while ((byte_sum = read(fd1,buff,sizeof(buff)))>0)
    {
        write(fd2,buff,byte_sum);
        /* code */
    }
    
    

    close(fd1);
    close(fd2);

    return 0;

}

