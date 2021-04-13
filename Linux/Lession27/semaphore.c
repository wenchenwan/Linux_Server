/*
    信号量的类型 sem_t 
    int sem_init(sem_t *sem, int pshared, unsigned int value); 
        -初始化信号量
        -参数
            -sem ：信号变量的地址
            -pshared ：
                0 用在线程间
                非零 用在进程间
            -value ：
                信号量中的值

    int sem_destroy(sem_t *sem); 
        -释放资源
    int sem_wait(sem_t *sem); 
        -对信号量加锁，调用一次，对信号量的值减一，如果为0，就阻塞
        原来在信号量里边的值存在，每当调用一次就减一，直到信号量为零就阻塞
    int sem_trywait(sem_t *sem); 
        - 
    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
        -等待多久的时间
    int sem_post(sem_t *sem);、
        -对信号量解锁，调用一次，对信号量的值加一
    int sem_getvalue(sem_t *sem, int *sval);

    sem_t psem;
    sem_t csem;
    init(psem, 0, 8);
    init(csem, 0, 0);

    producer() {
        sem_wait(&psem);
        sem_post(&csem)
    }

    customer() {
        sem_wait(&csem);
        sem_post(&psem)
    }
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
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

// 创建信号量
sem_t sempro,semcon;

void * Consume(void * arg){
    // 保存头节点的指针
    while(1){

        sem_wait(&semcon);
        pthread_mutex_lock(&mutex);
        struct Node * temp =  head;
        //有数据
        head = head->next;
        printf("del node, num : %d, tid : %ld\n",temp->num,pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        sem_post(&sempro);
        usleep(100);

       
    }
    return NULL;
}

void * Product(void * arg){
    while(1){
        sem_wait(&sempro);
        // 调用一次减小1，当为零的时候就阻塞
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand()%1000 + 1;
        printf("add Mode, num : %d, tid : %ld\n",newNode->num,pthread_self());
        
        // 只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        sem_post(&semcon);
        // 表示消费者可以消费多少个
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    sem_init(&sempro,0,10);
    sem_init(&semcon,0,0);
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
    sem_destroy(&sempro);
    sem_destroy(&semcon);
    pthread_exit(NULL);
    
    return 0;
}