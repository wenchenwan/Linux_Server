#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // 1.创建socket
    int lfd = socket(PF_INET,SOCK_DGRAM,0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }
    // 2.绑定
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 3.通信
    while(1){
        // 接受数据
        char buf[128];
        char ipbuf[16];
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);
        int num = recvfrom(lfd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&len);
        if(num == -1){
            perror("recvfrom");
            exit(0);
        }

        printf("client IP  :  %s , Port  ： %d \n",
        inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ipbuf,sizeof(ipbuf)),
        ntohs(clientaddr.sin_port));

        printf("client say : %s\n",buf);

        sendto(lfd,buf,strlen(buf)+1,0,(struct sockaddr *)&clientaddr,sizeof(clientaddr));

    }
    close(lfd);
    
    return 0;
}