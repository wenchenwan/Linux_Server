// tcp服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    // 1.创建socket(用于监听的套接字)
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }
    // 2.绑定IP端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;

    inet_pton(AF_INET,"192.168.190.143",&saddr.sin_addr.s_addr);
    // saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 3.监听
    re = listen(lfd,5);
    if(re == -1){
        perror("listen");
        exit(0);
    }

    // 4.接受客户端连接
    struct sockaddr_in Clientaddr;
    socklen_t len1 = sizeof(Clientaddr);
    int cfd = accept(lfd,(struct sockaddr *)&Clientaddr,&len1);
    if(cfd == -1){
        perror("accept");
        exit(0);
    }
    // 输出客户端的信息
    char clientIP[16] = {};
    inet_ntop(AF_INET,&Clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short clientport = ntohs(Clientaddr.sin_port);

    printf("IP : %s port : %d",clientIP,clientport);
    char recvBuf[1024] = {};
    // 获取客户端信息
    while(1){
        
        int len = read(cfd,recvBuf,sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(0);
        }else if(len > 0){
            printf("rev client data : %s\n",recvBuf);

        }else if(len == 0){
            printf("client closeed ....\n");
            break;
        }
        // 给客户端发送数据
        char * data = "hello I am server";
        write(cfd,data,strlen(data));
    }
    
    // 关闭文件描述符
    close(cfd);
    close(lfd);
    return 0;
}

