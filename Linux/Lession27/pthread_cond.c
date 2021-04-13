/*

    条件变量的类型 pthread_cond_t 
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);

    int pthread_cond_destroy(pthread_cond_t *cond);

    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	    //等待产生可以消费的数据
        阻塞函数，等待唤醒
    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
	    //等待多久的时间
        调用这个函数，线程会阻塞，知道等待时间结束
    int pthread_cond_signal(pthread_cond_t *cond);
	    //唤醒消费者一个或者多个
        唤醒一个或者多个等待的线程
    int pthread_cond_broadcast(pthread_cond_t *cond);
	    //唤醒所有的消费者
        唤醒所有的等待的线程

*/
/*
    生产者消费者模型
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct Node
{
    int num;
    struct Node * next;
    
};
struct Node * head = NULL; 
// 创建互斥变量
pthread_mutex_t mutex;

// 创建条件变量
pthread_cond_t cond; 

void * Consume(void * arg){
    // 保存头节点的指针
    while(1){
       pthread_mutex_lock(&mutex);
       struct Node * temp =  head;
       if(head != NULL){
            //有数据
            head = head->next;
            printf("del node, num : %d, tid : %ld\n",temp->num,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
       }else{
            // 没有数据需要等待
            pthread_cond_wait(&cond,&mutex);
            // 阻塞进入拿到锁以后，阻塞等待器件，就会解锁，其他线程就可以抢占
            // 如果阻塞结束被唤醒，向下执行就会加锁。
            pthread_mutex_unlock(&mutex);
       }
       
    }
    return NULL;
}

void * Product(void * arg){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand()%1000 + 1;
        printf("add Mode, num : %d, tid : %ld\n",newNode->num,pthread_self());
        
        // 只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    // 创建5个生产者线程和5个消费者线程
    pthread_t produ[5],consu[5];
    for(int i = 0;i < 5; i++){
        pthread_create(&produ[i],NULL,Product,NULL);
        pthread_create(&consu[i],NULL,Consume,NULL);

    }
    
    for(int i = 0;i < 5;i++){
        pthread_detach(produ[i]);
        pthread_detach(consu[i]);
    }

    while (1)
    {
        sleep(10);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    
    return 0;
}