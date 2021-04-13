/*
    #include <pthread.h>
    int pthread_join(pthread_t thread, void **retval);
    -功能：
        和一个已经终止的线程进行连接，就是回收子线程的资源
            这个函数是阻塞函数
            这个函数调用一次只能回收一个子进程
            一般在子线程中使用
    -参数
        -thread :需要回收的子线程id
        -retval ：接受子线程结束时的返回值
    -返回值
        -0 表示成功
        -非零 设置错误号
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int value = 10;
void * MyFunc(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    // sleep(3);
    // int value = 0;
    // 线程的栈空间的数据会在线程结束以后清空
    pthread_exit((void *)(&value));
    // return NULL;
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

    // 主线程调用pthread_join()回收子线程的资源
    int * thread_retval = NULL;
    int ret = pthread_join(tid, (void **)&thread_retval);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error : %s\n",errstr);
    }
    printf("I receive value : %d\n",*thread_retval);
    printf("回收子线程资源成功\n");
    pthread_exit(NULL);
    // 主线程退出，对其他线程没有影响

    
    return 0;
}