/*
    标准c库

        #include <stdio.h>
        int fseek(FILE *stream, long offset, int whence);

    Linux库
       #include <sys/types.h>
       #include <unistd.h>

       off_t lseek(int fd, off_t offset, int whence);
            -fd  文件描述符
            -offset  偏移量
            -whence
                SEEK_SET
                The file offset is set to offset bytes.

                SEEK_CUR
                The file offset is set to its current location plus offset bytes.

                SEEK_END
                The file offset is set to the size of the file plus offset bytes.

            返回值:返回当前指针的位置
        作用：
            1.移动文件指针到头文件
            lseek(fd,0,SEEK_SET)

            2.获取当前文件指针的位置
            lseek(fd,0,SEEK_CUR)

            3.获取文件的长度
            lseek(fd,0.SEEK_END)

            4.拓展文件的长度
            lseek(fd,100,SEEK_END)
            //需要写一次文件数据

*/

#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(){

    int fd = open("hello.txt",O_WRONLY);

    if(fd == -1){
        perror("open file");
        return -1;
    }

    lseek(fd,100,SEEK_END);
    
    //写入一个空数据实现拓展
    write(fd," ",1);
    close(fd);

    return 0;
    

}