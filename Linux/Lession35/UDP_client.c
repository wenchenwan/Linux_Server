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
    struct sockaddr_in saddr;
    inet_pton(AF_INET,"127.0.0.1",&saddr.sin_addr.s_addr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);


    // 3.通信
    int num1 = 0;
    while(1){
        // 接受数据
        char buf[128];
        int len = sizeof(buf);

        sprintf(buf,"hello I am client %d\n",num1++);
        sendto(lfd,buf,strlen(buf)+1,0,(struct sockaddr *)&saddr,len);

        int num = recvfrom(lfd,buf,sizeof(buf),0,NULL,NULL);
        if(num == -1){
            perror("recvfrom");
            exit(0);
        }

        printf("server say : %s\n",buf);

        sleep(1);
    }
    close(lfd);
    return 0;
}