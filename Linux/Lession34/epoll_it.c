#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(){
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(10);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    // 创建数组，用来保存内核返回的数据
    struct epoll_event arrep[1024];
    while(1){
        int ret = epoll_wait(epfd,arrep,1024,-1);
        if(ret == -1){
            perror("epoll");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            printf("ret = %d\n",ret);
            for(int i = 0;i < ret;i++){
                if(arrep[i].data.fd == lfd){
                    // 监听的文件描述符号，有客户端连接
                    struct sockaddr_in clientaddr;
                    int len = sizeof(clientaddr);
                    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                    epev.events = EPOLLIN;
                    epev.data.fd = cfd;

                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
                }else{
                    // 说明有数据到达
                    char buf[5] = {0};
                    int len = read(arrep[i].data.fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        epoll_ctl(epfd,EPOLL_CTL_DEL,arrep[i].data.fd,NULL);
                        close(arrep[i].data.fd);
                        
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(arrep[i].data.fd,buf,strlen(buf)+1);
                    }

                }
            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}