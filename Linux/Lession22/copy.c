// 使用内存映射实现文件拷贝功能

/*
    思路：
        1.对文件进行内存映射
        2.创建一个新文件（拓展该文件）
        3.把新文件的数据映射到内存中
        4.通过内存拷贝第一个文件的内存数据拷贝到新的文件内存中
        5.释放资源
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>

int main(){
    // 1.对原始文件进行映射
    int fd = open("english.txt",O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }


    // 计算原始文件的长度
    struct stat statbuf;
    int re = stat("english.txt",&statbuf);
    // int len = lseek(fd,0,SEEK_END);

    int fd_new = open("cpFile.txt",O_CREAT|O_RDWR,0664);
    if(fd_new == -1){
        perror("open");
        exit(0);
    }
    truncate("cpFile.txt",statbuf.st_size);
    // 2.分别做内存映射
    void * ptr1 = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr1 == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    void * ptr2 = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd_new,0);
    if(ptr2 ==MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    // 内存拷贝
    memcpy(ptr2,ptr1,statbuf.st_size);

    munmap(ptr1,statbuf.st_size);
    munmap(ptr2,statbuf.st_size);
    
    return 0;
}