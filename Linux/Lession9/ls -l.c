/*
实现ls -l 的功能
-rw-rw-r-- 1 wen wen   18 Mar 17 19:07  a.txt

*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc,char * argv[]){

    if(argc < 2){
        printf("%s filename \n",argv[0]);
        return -1;
    }
    //通过stat 获取用户传进来的文件信息
    struct  stat st;
    int re = stat(argv[1],&st);
    if(re == -1){
        perror("stat");
        return -1;
    }
    //获取文件类型和文件权限
    char perm[11] = {0};
    switch (st.st_mode & S_IFMT)
    {
    case S_IFLNK:
        perm[0] = 'l';
        break;
    case S_IFDIR:
        perm[0] = 'd';
        break;
    case S_IFREG:
        perm[0] = '-';
        break;
    case S_IFBLK:
        perm[0] = 'b';
        break;
    case S_IFCHR:
        perm[0] = 'c';
        break;
    case S_IFSOCK:
        perm[0] = 's';
        break;
    case S_IFIFO:
        perm[0] = 'p';
        break;
    default:
        perm[0] = '?';
        break;
    }
 
    perm[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perm[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perm[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';

    perm[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perm[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perm[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';

    perm[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perm[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perm[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    //获取硬链接数
    int link_num = st.st_nlink;

    //文件所有者
    char * fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char * fileGro = getgrgid(st.st_gid)->gr_name;

    //文件的大小
    long int size = st.st_size;
      
    //获取修改的时间
    char * time = ctime(&st.st_mtime);

    char mtime[512]={};
    strncpy(mtime,time,strlen(time)-1);

    char buff[1024]={};
    sprintf(buff,"%s %d %s %s %ld %s %s",perm,link_num,fileUser,fileGro,size,mtime,argv[1]);
    
    printf("%s\n",buff);

    return 0;

}