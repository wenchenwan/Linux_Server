/*
    匿名映射,不需要文件实体进程一个内存映射

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
#include <wait.h>
int main(){

    int len = 4096;
    void * ptr = mmap(NULL, len, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANON,-1,0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        strcpy((char *)ptr,"hello , world !");
        wait(NULL);

    }else if(pid == 0){
        // 子进程
        sleep(1);
        printf("%s\n",(char *)ptr);
    }else if(pid == -1){
        perror("fork");
        exit(0);
    }

    int re = munmap(ptr,len);
    if(re == -1){
        perror("munmap");
        exit(0);
    }
    return 0;
}