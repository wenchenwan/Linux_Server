#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(){
    unlink("client.sock");
    // 1.创建套接字
    int cfd = socket(AF_LOCAL,SOCK_STREAM,0);
    
    if(cfd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,"client.sock");
    int re = bind(cfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(-1);
    }



    // 3.连接服务器
    struct sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    strcpy(saddr.sun_path,"server.sock");
    re = connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(re == -1){
        perror("connect");
        exit(-1);
    }

    // 5.通信
    int num = 0;
    while(1){
        char buf[128];
        sprintf(buf,"hello I am client %d \n",num++);
        send(cfd,buf,strlen(buf)+1,0);

        printf("client says : %s \n",buf);

        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("server closed ...\n");
            break;
        }else if(len > 0){
            printf("server says : %s \n",buf);    
        }
        sleep(1);
    }
    close(cfd);
    return 0;
}