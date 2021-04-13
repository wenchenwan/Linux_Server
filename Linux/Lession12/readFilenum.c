/*
        
       #include <sys/types.h>
       #include <dirent.h>
       DIR *opendir(const char *name);
        功能
            打开一个目录
        返回值
            返回一个目录流

       #include <dirent.h>
       struct dirent *readdir(DIR *dirp);
        功能：
            读取目录
        返回值
            返回一个结构体
            如果读到文件流的末尾或者读取失败返回NULL
       
       #include <sys/types.h>
       #include <dirent.h>
       int closedir(DIR *dirp);
        功能：关闭目录
*/
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int getFileNum(const char * path);
int main(int argc,char * argv[]){
 
    if(argc < 2){
        printf("%s path \n",argv[0]);
        return -1;
    }
    int FileSum = getFileNum(argv[1]);
    printf("普通文件的数目为：%d\n",FileSum);

    return 0;
}

//用于获取目录下普通文件个数
int getFileNum(const char * path){
    //打开目录
    DIR * dir = opendir(path);
    if(dir == NULL){
        perror("opendir");
        exit(0);
    }
    struct dirent * ptr;
    int num = 0;
    

    while((ptr= readdir(dir)) != NULL){
        //获取名称
        char * dname = ptr->d_name;

        //记录普通文件的数目
        

        //忽略掉./和../
        if(strcmp(dname,".")==0 || strcmp(dname,"..")==0){
            continue;
        }
        //判断为普通文件还是目录
        if(ptr->d_type == DT_DIR){
            char newPath[256];
            sprintf(newPath,"%s/%s",path,dname);
            num += getFileNum(newPath);
        }
        if(ptr->d_type == DT_REG){
            num++;
        }
        
    }

    closedir(dir);
    return num;
    
}