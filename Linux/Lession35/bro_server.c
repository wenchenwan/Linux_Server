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

    // 2.设置广播属性
    int op = 1;
    setsockopt(lfd,SOL_SOCKET,SO_BROADCAST,&op,sizeof(op)); 

    // 3.创建一个广播的地址
    struct sockaddr_in cliaddr;
    // cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999);
    inet_pton(AF_INET,"192.168.190.144",&cliaddr.sin_addr.s_addr);


    // 4.通信
    int nummber = 0;
    while(1){
        // 接受数据
        char sendBuf[128];
        sprintf(sendBuf,"hello client : %d \n",nummber++);

        sendto(lfd,sendBuf,strlen(sendBuf)+1,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        printf("广播的数据 ： %s\n",sendBuf);

        sleep(1);
    }
    close(lfd);
    
    return 0;
}