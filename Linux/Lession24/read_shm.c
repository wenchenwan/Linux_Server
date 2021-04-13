/*

*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // 获取共享内存
    int shmid = shmget(100,4096,IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
        exit(0);
    }
    // 和当前进程进行关联
    void * shmaddr = shmat(shmid,NULL,0);


    // 读数据
    printf("读到的数据为：%s\n",(char *)shmaddr);
    printf("按任意键继续\n");
    getchar();

    // 解除关联
    shmdt(shmaddr);

    // 删除共享内存
    // struct shmid_ds buf;
    shmctl(shmid,IPC_RMID,NULL);

    return 0;

}