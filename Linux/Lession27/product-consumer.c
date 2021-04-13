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
pthread_mutex_t mutex;

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
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
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
    pthread_exit(NULL);
    
    return 0;
}