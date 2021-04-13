#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // 判断有名管道是否存在
    int flag1 = access("fifo1",F_OK);
    if(flag1 == -1){
        int re = mkfifo("fifo1",0664);
        if(re == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    int flag2 = access("fifo2",F_OK);
    if(flag2 == -1){
        int re = mkfifo("fifo2",0664);
        if(re == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    int fdw = open("fifo1",O_WRONLY);
    if(fdw == -1){
        perror("open");
        exit(0);
    }
    printf("管道fifo1打开成功\n");

    int fdr = open("fifo2",O_RDONLY);
    if(fdr == -1){
        perror("open");
        exit(0);
    }
    printf("管道fifo2打开成功\n");
    char buf[128] = {0};

    while (1)
    {   
        memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);

        int re = write(fdw,buf,strlen(buf));
        if(re == -1){
            perror("write");
            exit(0);
            // break;
        }

        // 读管道数据
        memset(buf,0,sizeof(buf));
        int ret = read(fdr,buf,sizeof(buf));
        if(ret <= 0){
            perror("read");
            break;
        }
        printf("I reve: %s\n",buf);

        
    }
    close(fdw);
    close(fdr);
    return 0;
}