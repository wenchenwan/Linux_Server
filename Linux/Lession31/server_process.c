#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recyleChild(int num){
    while(1){
        int re = waitpid(-1,NULL,WNOHANG);
        if(re == -1){
            // 所有子进程都回收结束
            break;
        }else if(re == 0){
            break;
            // 没有需要回收的子进程
        }else if(re > 0){
            // 被回收了
            printf("子进程 %d 被回收了\n",re);
        }
    }

}
int main(){
    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;
    sigaction(SIGCHLD,&act,NULL);

    // 创建socket
    // AF表示地址族
    // PF表示协议组
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }
    // 绑定IP和端口
    struct sockaddr_in  clientaddr;
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.190.143",&clientaddr.sin_addr.s_addr);
    clientaddr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 监听
    re = listen(lfd,5);
    if(re == -1){
        perror("listen");
        exit(0);
    }

    // 不断循环等待客户端连接
    while (1)
    {
        // 接受连接
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
        // 如果调用出错就会产生一个errno,然后结束这个进程运行,所以后边的客户端就不能再加入
        if(cfd == -1){
            if(errno == EINTR){
                continue;
            }
            perror("accept");
            exit(0);
        }

        // 每一个连接进来创建子进程和客户端进行通信
        pid_t pid = fork();
        if(pid == 0){
            // 子进程
            // 获取客户端信息
            char cliIP[16] = {};
            inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,cliIP,(socklen_t)sizeof(cliIP));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            printf("recv client IP : %s, PORT : %d \n",cliIP,cliPort);

            // 接受客户端发来的数据
            char buf[1024] = {};
            while(1){
                int num = read(cfd,buf,sizeof(buf));
                if(num == -1){
                    perror("read");
                    exit(0);
                }else if(num > 0){
                    printf("rev client : %s \n",buf);
                }else if(num == 0){
                    printf("client closed ...\n");
                    break;
                }

                write(cfd,buf,strlen(buf)+1);
            }
            close(cfd);
            pthread_exit(NULL);

        }else if(pid > 0){
            // 父进程

        }else if(pid == -1){
            perror("fork");
            exit(0);
        }

    }

    close(lfd);
    return 0;
}