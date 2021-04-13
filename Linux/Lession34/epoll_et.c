#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

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
    int epfd = epoll_create(100);

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
            perror("epoll_wait");
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
                    // 设置cfd非阻塞
                    int flag = fcntl(cfd, F_GETFL);
                    flag = flag | O_NONBLOCK;
                    fcntl(cfd,F_SETFL,flag);

                    epev.events = EPOLLIN | EPOLLET;
                    epev.data.fd = cfd;

                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
                }else{
                    // 说明有数据到达
                    if(arrep[i].events & EPOLLOUT){
                        continue;
                    }
                    char buf[5] = {0};
                    // 循环读取所有的数据
                    int len = 0;
                    // 设置read为非阻塞的情形，通过文件描述符来设置
                    while((len = read(arrep[i].data.fd,buf,sizeof(buf)))>0){
                        // printf("rev data : %s\n",buf);
                        write(STDOUT_FILENO,buf,len);
                        write(arrep[i].data.fd,buf,len);
                    }    
                    if(len == 0){
                        printf("client closed ...\n");
                    }else if(len == -1){
                        if(errno == EAGAIN){
                            printf("data over ...\n");
                        }else{
                            perror("read");
                            exit(0);
                        }
                        
                    }
                    
                }
            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}