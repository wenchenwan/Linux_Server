/*
    读写锁的类型 pthread_rwlock_t 
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int num = 10;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void * ThreadRead(void * arg){
    while(1){
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);
        printf("===read tid: %ld, num = %d\n",pthread_self(),num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}
void * ThreadWrite(void * arg){
    while (1)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("num++ tid: %ld, num = %d\n",pthread_self(),num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL; 
    
}
int main(){
    // pthread_mutex_init(&mutex,NULL);
    pthread_rwlock_init(&rwlock,NULL);
    pthread_t w_tid[3],r_tid[5];
    for(int i = 0;i< 5 ;i++){
        pthread_create(&r_tid[i],NULL,ThreadRead,NULL);
    }

    for(int i = 0;i< 3 ;i++){
        pthread_create(&w_tid[i],NULL,ThreadWrite,NULL);
    }

    for(int i = 0;i< 5 ;i++){
        pthread_detach(r_tid[i]);
    }

    for(int i = 0;i< 3 ;i++){
        pthread_detach(w_tid[i]);
    }
    
    pthread_exit(NULL);
    // pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}