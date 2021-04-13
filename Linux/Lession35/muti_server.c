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

    // 2.设置多播属性，设置外出接口
    struct in_addr op;
    // 初始化多播地址
    inet_pton(AF_INET,"239.0.0.10",&op.s_addr);
    setsockopt(lfd,IPPROTO_IP,IP_MULTICAST_IF,&op,sizeof(op)); 

    // 3.初始化客户端的信息
    struct sockaddr_in cliaddr;
    // cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET,"239.0.0.10",&cliaddr.sin_addr.s_addr);


    // 4.通信
    int nummber = 0;
    while(1){
        // 接受数据
        char sendBuf[128];
        sprintf(sendBuf,"hello client : %d \n",nummber++);

        sendto(lfd,sendBuf,strlen(sendBuf)+1,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        printf("多播的数据 ： %s\n",sendBuf);

        sleep(1);
    }
    close(lfd);
    
    return 0;
}