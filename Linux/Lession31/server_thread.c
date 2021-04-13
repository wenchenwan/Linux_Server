#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct sockInfo{
    int fd;
    struct sockaddr_in addr;
    pthread_t tid;
};

struct sockInfo sockinfos[128];

void * MyFun(void * arg){
    // 进行子线程和客户端通信  cfd  客户端信息  线程号
    char cliIP[16] = {};
    struct sockInfo * pinfo = (struct sockInfo *)arg;
    inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr,cliIP,(socklen_t)sizeof(cliIP));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("recv client IP : %s, PORT : %d \n",cliIP,cliPort);

    // 接受客户端发来的数据
    char buf[1024] = {};
    while(1){
        int num = read(pinfo->fd,buf,sizeof(buf));
        if(num == -1){
            perror("read");
            exit(0);
        }else if(num > 0){
            printf("rev client : %s \n",buf);
        }else if(num == 0){
            printf("client closed ...\n");
            break;
        }

        write(pinfo->fd,buf,strlen(buf)+1);
    }
    close(pinfo->fd);
    pthread_exit(NULL);
    return NULL;
}

int main(){
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }
    // 绑定IP和端口
    struct sockaddr_in  clientaddr;
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.190.143",&clientaddr.sin_addr.s_addr);
    clientaddr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 监听
    re = listen(lfd,5);
    if(re == -1){
        perror("listen");
        exit(0);
    }
    // 初始化数据
    int max = sizeof(sockinfos)/sizeof(sockinfos[0]);
    for(int i = 0; i < max; i ++){
        bzero(&sockinfos[i],sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        // 设置文件描述符不可用

    }
    //循环的等待客户端的连接，一但有子进程连接进来就创建子线程
    while(1){
        // 接受连接
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);

        struct sockInfo *pinfo;
        for(int i = 0; i < max; i++){
            // 从数组中找到一个可用的sockInfo元素
            if(sockinfos[i].fd == -1){
                pinfo = &sockinfos[i];
                break;
            }
            if(i == max-1){
                sleep(1);
                i--;
            }
        }

        pinfo->fd = cfd;

        // memcpy(&pinfo->addr,&cliaddr,sizeof(cliaddr));
        pinfo->addr.sin_addr.s_addr = cliaddr.sin_addr.s_addr;
        pinfo->addr.sin_family = cliaddr.sin_family;
        pinfo->addr.sin_port = cliaddr.sin_port;
        

        // 创建子线程
        pthread_create(&pinfo->tid,NULL,MyFun,pinfo);

        // pthread_join(NULL);
        // 阻塞的

        pthread_detach(pinfo->tid);
    }
    
    return 0;

}