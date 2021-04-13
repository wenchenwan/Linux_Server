#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>

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
    // 调用poll系统函数
    struct pollfd fds[1024];
    for(int i = 0;i < 1024;i++){
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;

    int nfds = 0;
    
    while(1){
        int ret = poll(fds,nfds + 1,-1);
        if(ret == -1){
            perror("poll");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 说明对应文件描述符的缓冲区发生了改变。
            if(fds[0].revents & POLLIN){
                // 表示有新的客户端连接进来
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                // 将新的文件描述符加入到集合中
                for(int i = 1 ;i < 1024; i++){
                    if(fds[i].fd == -1){
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                
                // 更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd;
            }
            for(int i = 1;i <= nfds; i++){
                if(fds[i].revents & POLLIN){
                    // 说明文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(fds[i].fd,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;
}