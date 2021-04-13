/*
    int pthread_attr_init(pthread_attr_t *attr); 
        -初始化线程属性变量

    int pthread_attr_destroy(pthread_attr_t *attr); 
        -释放线程属性的资源

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
        -获取线程分离的状态属性

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        -设置线程分离模型的状态属性

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void * callback(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    return NULL;
}
int main(){
    // 创建一个线程属性变量
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    // 获取线程的栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr,&size);
    

    pthread_t tid;

    int re = pthread_create(&tid,&attr,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }
    printf("stack size : %ld\n",size);

    pthread_attr_destroy(&attr);

    printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());

    // 设置子线程分离
    //pthread_detach(tid);


    pthread_exit(NULL);
    return 0;
}