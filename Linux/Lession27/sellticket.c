/*
    使用多线程实现买票的案例
    三个窗口，一共一百张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticketd = 100;
void * Myfunc(void * arg){
    
    while(ticketd > 0){
        printf("%ld 正在卖第 %d 张票\n",pthread_self(),ticketd);
        usleep(6000);
        ticketd--;
    }
    return NULL;
}
int main(){
    // 创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid1,NULL,Myfunc,NULL);

    // 回收子线程的资源，阻塞
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    // 设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);
    
    return 0;
}