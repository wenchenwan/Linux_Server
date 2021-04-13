#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    // 1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(0);
    }
    // 2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.190.143",&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);

    int re = connect(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(re == -1){
       perror("connect");
       exit(0); 
    }

    // 3. 通信
    char * data = "hello,i am client";
    char recvBuf[1024] = {0};
    while(1) {
        // 给客户端发送数据
        write(fd, data , strlen(data));

        sleep(1);
        
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            exit(-1);
        } else if(len > 0) {
            printf("recv server data : %s\n", recvBuf);
        } else if(len == 0) {
            // 表示服务器端断开连接
            printf("server closed...");
            break;
        }

    }
    // 关闭连接
    close(fd);
    return 0;
}