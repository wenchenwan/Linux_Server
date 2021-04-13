/*
    #include <pthread.h>
    int pthread_cancel(pthread_t thread);

    -功能：取消线程（让线程终止）
        取消某个线程，可以终止某个线程的运行，而是当某个线程执行到某个取消点，线程才会被终止。
        取消点：系统规定好的一些系统调用，我们可以粗略的理解为系统从用户区到内核区的系统调用
*/

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void * callback(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    for(int i = 0;i < 5 ; i++){
        printf("child i = %d\n",i);
    }
    return NULL;
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }

    // printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());
    pthread_cancel(tid);

    for(int i = 0;i < 5 ; i++){
        printf("i = %d\n",i);
    }


    pthread_exit(NULL);
    return 0;
}