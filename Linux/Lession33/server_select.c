#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    // 创建一个fd_set的一个集合，用来存放需要检测的文件描述符
    fd_set rdset,temp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd = lfd;

    while(1){
        temp = rdset;
        int ret = select(maxfd+1,&temp,NULL,NULL,NULL);
        if(ret == -1){
            perror("select");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 说明对应文件描述符的缓冲区发生了改变。
            if(FD_ISSET(lfd,&temp)){
                // 表示有新的客户端连接进来
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                // 将新的文件描述符加入到集合中
                FD_SET(cfd,&rdset);
                
                // 更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            for(int i = lfd + 1;i <= maxfd; i++){
                if(FD_ISSET(i,&temp)){
                    // 说明文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        close(i);
                        FD_CLR(i,&rdset);
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(i,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;
}