#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(){
    unlink("server.sock");
    // 1.创建套接字
    int lfd = socket(AF_LOCAL,SOCK_STREAM,0);
    
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,"server.sock");
    int re = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(-1);
    }

    // 3.监听
    re = listen(lfd,8);
    if(re == -1){
        perror("listen");
        exit(-1);
    }

    // 4.等待客户端连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
    if( cfd == -1){
        perror("accept");
        exit(-1);
    }

    printf("client sockt filename : %s",cliaddr.sun_path);

    // 5.通信
    while(1){
        char buf[128];
        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("client closed ...\n");
            break;
        }else if(len > 0){
            printf("client says : %s \n",buf);
            send(cfd,buf,len,0);
        }

    }
    close(cfd);
    close(lfd);
    return 0;
}