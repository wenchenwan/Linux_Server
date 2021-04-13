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
    // inet_pton(AF_INET,"192.168.190.144",&addr.sin_addr.s_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;// 绑定本机的IP地址

    int re = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(-1);
    }
    // 3.通信
    while(1){
        // 接受数据
        char buf[128];
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