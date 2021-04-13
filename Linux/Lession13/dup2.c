/*
    #include <unistd.h>
    int dup(int oldfd);
        作用：重定向文件描述符
        oldfd 指向 a.txt,newfd 指向 b.txt
        调用函数成功后：newfd 和 b.txt做close，newfd指向a.txt

        oldfd必须为一个有效的文件描述符
        oldfd和newfd值相同，相当于什么都不做
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

    int fd = open("1.txt",O_RDWR | O_CREAT,0664);
    if(fd == -1){
        perror("open");
        return -1;
    }

    int fd1 = open("2.txt",O_RDWR|O_CREAT,0664);
    if(fd1 == -1){
        perror("open");
        return -1;
    }

    printf("fd %d , fd1 %d\n",fd, fd1);

    int fd2 = dup2(fd,fd1);
    if(fd2 == -1){
        perror("dup2");
        return -1;
    }
    char * str = "hello ,dup2";
    // 通过fd1去写数据，实际操作的是1.txt，而不是2.txt
    int len = write(fd1,str,strlen(str));
    if(len == -1){
        perror("write");
        return -1;
    }

    printf("fd : %d, fd1 : %d fd2 : %d\n",fd,fd1,fd2);
    //fd2和fd1相同
    close(fd);
    close(fd1);

    return 0;
}