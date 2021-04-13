/*
    #include <sys/mman.h>

    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    -功能
        映射一个文件或者设备到内存中
    -参数
        -void *addr：（起始地址）传递一个null，由内核指定
        -length ： 要映射的数据的长度，这个值不能为零。建议使用文件的长度。
            获取文件的长度：
                stat函数或者lseek函数，Linux分配内存大小的默认会取分页的整数倍
        -prot ：对我们申请的内存映射区的一个操作权限
            -PROT_EXEC  Pages may be executed.
            -PROT_READ  Pages may be read.
            -PROT_WRITE Pages may be written.
            -PROT_NONE  Pages may not be accessed.
            需要操作的内存必须要有读的权限
            一般写 PROT_READ 或者 PROT_READ | PROT_WRITE
        -flags：
            -MAP_SHARED ：映射区的数据会自动和磁盘文件进行同步，进程间通信必须要有这个选项
            -MAP_PRIVATE ：不同步，内存映射区的数据改变了，不会对原来的文件进行修改，会重新创建一个新的文件（copy on write）
        -fd： 需要映射的文件的文件描述符，由open()得到
            -文件的大小不能为0，open指定的权限不能和prot参数权限冲突
                prot ：PROT_READ    open：只读/读写
                prot ：PROT_READ | PROT_WRITE   open ：读写权限
        -offset ：偏移量
            -一般不适用，必须指定为4K的整数倍，0表示不偏移
    -返回值 ：
        失败返回：(void *) -1
        成功返回：创建内存的首地址

    int munmap(void *addr, size_t length);
    -功能：释放内存映射
    -参数：
        -addr 释放内存的首地址
        -length 要释放的内存大小，要和mmap内存中length参数值一样
*/
// 使用内存映射实现进程间通信
/**
 * 1.有关系的进程（父子进程）
 *      -没有子进程的时候：
 *          -通过唯一的父进程，先创建内存映射区
 *      -有了内存映射区以后，创建子进程
 *      -父子进程共享创建的内存映射区
 * 2.没有关系的进程间通信
 *      -准备一个大小不是0的磁盘文件
 *      -进程1：通过磁盘文件创建内存映射区
 *          -得到一个操作这块内存的指针
 *      -进程2：通过磁盘文件创建内存映射区
 *          -得到一个操作这块内存的指针
 *      -使用内存映射区通信
 *  内存映射区通信不会阻塞
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
    // 1.打开一个文件
    int fd = open("test.txt",O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }
    
    int file_size = lseek(fd,0,SEEK_END);//获取文件的大小
    void * ptr = mmap(NULL,file_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    // 3.创建进程
    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        strcpy((char * )ptr,"ni hao,son");

    }else if(pid == 0){
        // 子进程
        // 定义一个数组
        char buf[64] = {0};
        strcpy(buf,ptr);
        printf("I am son rev : %s\n",buf);

    }else if(pid == -1){
        perror("fork");
        exit(0);
    }


    munmap(ptr,file_size);

    
    return 0;
}