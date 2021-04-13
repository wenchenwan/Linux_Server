/*

*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int main(){
    // 创建共享内存
    int shmid = shmget(100,4096,IPC_CREAT|0664);
    printf("shmid : %d\n", shmid);
    // if(shmid == -1){
    //     perror("shmget");
    //     exit(0);
    // }
    // 和当前进程进行关联
    void * shmaddr = shmat(shmid,NULL,0);


    // 写数据
    char * str = "Hello World !";
    memcpy(shmaddr,str,strlen(str)+1);
    printf("按任意键继续\n");
    getchar();

    // 解除关联
    shmdt(shmaddr);

    // 删除共享内存
    // struct shmid_ds buf;
    shmctl(shmid,IPC_RMID,NULL);

    return 0;

}