/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);
    -功能：分离线程，被分离的线程在终止的时候，会自动释放资源返回给系统
        1.不能多次分离，会产生不可预期的错误
        2.不能去链接一个已经分离的线程,会报错
    -参数：
        需要分离的线程的ID
    -返回值：
        成功： 返回0
        失败： 返回错误号

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
    pthread_t tid;
    int re = pthread_create(&tid,NULL,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }

    printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());

    // 设置子线程分离
    pthread_detach(tid);

    // 对分离的子线程进行连接
    // int err = pthread_join(tid,NULL);
    // if(err != 0){
    //     char * str = strerror(err);
    //     printf("join err : %s\n",str);
    //     exit(0);
    // }
    pthread_exit(NULL);
    return 0;
}