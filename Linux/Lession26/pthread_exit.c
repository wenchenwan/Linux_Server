/*
    #include <pthread.h>
    void pthread_exit(void *retval);
    -功能：终止一个线程，在哪一个线程中调用，终止哪个线程
    -参数：
        retval:需要传递一个指针，作为一个返回值，可以在pthread_join()中可以
    pthread_t pthread_self(void)
        -功能：
            获取当前线程的线程id
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void * MyFunc(void * arg){
    printf("child thread id : %ld\n",pthread_self());

    return NULL;
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,MyFunc,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("%s\n",errstr);
        exit(0);
    }
    for(int i = 0;i < 100;i++){
        printf("i = %d\n",i);
    }

    printf("tid : %ld, main thread id : %ld\n",tid ,pthread_self());

    pthread_exit(NULL);
    // 主线程退出，对其他线程没有影响

    printf("pthread exit \n");
    return 0;
}