/*
    main线程所在的线程成为主线程（main线程），其余创建的线程叫做子线程
    程序中默认只有一个进程，调用fork()函数，子进程出现
    程序中默认只有一个线程，调用pthread()函数，子线程出现

    #include <pthread.h>
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine) (void *), void *arg);

        -功能：创建一个子线程（main函数为主线程）
        -参数：
            -*thread    传出参数，线程创建成功，子线程的线程ID会保存在这一个指针中
            -*attr      需要设置的线程的属性，一般使用默认的属性，传递null
            -start_routine 函数指针，子线程需要处理的逻辑代码
            -arg        给第三个参数使用，向函数传递参数
        -返回值：
            成功 返回0
            失败 返回错误号。这个错误号与之前的errno不一样
                不能使用perror()进行错误的输出
                获取错误号信息： char * strerror(int errnum);
*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * MyFunc(void * arg){
    printf("child pthread now ...\n");
    printf("arg val : %d\n",*(int *)arg);

    return NULL;
}
int main(){
    pthread_t tid;
    int num = 10;
    int re = pthread_create(&tid,NULL,MyFunc,(void *)&num);
    if(re != 0){
        char * str = strerror(re);
        printf("errorno : %s\n",str);
        exit(0);
    }
    for(int i = 0;i < 5;i++){
        printf("i = %d \n",i);
    }

    sleep(1);

    return 0;
}