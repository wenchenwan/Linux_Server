/*
    #include <unistd.h>
    int execl(const char *path, const char *arg, ...);
        -参数
            -path
                需要指定可执行文件的路径或者名称
                    a.out// /home/wen/Linux/Lession15/a.out
            -arg:可执行文件的列表
                第一个参数一般没有什么作用，一般为执行文件的名称，
                从第二参数往后，就是程序执行的参数列表
                参数最后需要以NUll结束（哨兵）
        -返回值
            只有当调用失败或者错误的时候才会有返回值，如果调用成功
            则没有返回值

    int execl(const char *path, const char *arg, ...)
        -会到环境变量中查找可执行文件，找到了就执行，找不到就报错
        -参数
            -file：可执行文件的文件名
                a.out
                ps
            -arg:可执行文件的列表
                第一个参数一般没有什么作用，一般为执行文件的名称，
                从第二参数往后，就是程序执行的参数列表
                参数最后需要以NUll结束（哨兵）
        -返回值
            只有当调用失败或者错误的时候才会有返回值，如果调用成功
            则没有返回值



        int execv(const char *path, char *const argv[]);
        argv是需要的参数的一个字符串数组
        char * argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);

        int execve(const char *filename, char *const argv[], char *const envp[]);
        char * envp[] = {"/home/nowcoder", "/home/bbb", "/home/aaa"};
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(){

    pid_t pid = fork();

    if(pid > 0){
        //父进程
        printf("I am parent process,pid: %d\n",getpid());
    }else if(pid == 0){
        //子进程
        
        // execl("/home/wen/Linux/Lession16/hello","hello",NULL);
        // execl("/bin/ps","ps","a","u","x",NULL);
        execlp("ps","ps","a","u","x",NULL);
        printf("I am child process,pid: %d\n",getpid());
    }
    for(int i = 0;i<5;i++){
        printf("i = %d , pid = %d \n",i,getpid());
    }

    return 0;

}