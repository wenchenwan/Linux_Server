/*
    互斥量的类型 pthread_mutex_t 
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
        -初始化互斥量
        -参数：
            -mutex：需要初始化的互斥量变量、
            -attr： 互斥量相关的属性，null
        -restrict：C语言的修饰符，被修饰的指针，不能由另外的指针进行操作。
            pthread_mutex_t *restrict mutex = xxx;
            pthread_mutex_t *restrict mutex1 =  mutex;
            不能使用mutex1来操作mutex

    int pthread_mutex_destroy(pthread_mutex_t *mutex); 
        -释放互斥锁资源

    int pthread_mutex_lock(pthread_mutex_t *mutex); 
        -加锁，阻塞的，如果有一个线程加锁，其他线程只能阻塞等待

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        -尝试上锁，如果枷锁失败，不会阻塞，直接返回

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        -解锁
*/  
/*
    使用多线程实现买票的案例
    三个窗口，一共一百张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticketd = 1000;

// 创建线程锁
pthread_mutex_t mutex;
void * Myfunc(void * arg){
    // 临界区
    while(1){
        // 加锁
        pthread_mutex_lock(&mutex);
        // pthread_mutex_trylock(&mutex);
        // trylock尝试上锁，上锁失败就不阻塞

        // pthread_mutex_lock(&mutex);
        // pthread_mutex_lock(&mutex);
        // 重复加锁


        if(ticketd > 0){
            printf("%ld 正在卖第 %d 张票\n",pthread_self(),ticketd);
            usleep(6000);
            ticketd--;
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    // 解锁
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_mutex_init(&mutex,NULL);

    // 创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid2,NULL,Myfunc,NULL);
    pthread_create(&tid3,NULL,Myfunc,NULL);

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

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);
    
    return 0;
}