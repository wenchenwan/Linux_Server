## 1.Linux编程开发基础

### 1.1静态库的制作和使用

```shell
gcc -c add.c sub.c div.c mult.c
ar rcs libcal.a add.o sub.o mult.o div.o
#生成.a的lib文件
cp ../calc/libcal.a lib/
#将库文件导入lib文件夹下

#首先要生成.o文件
gcc main.c -o app

gcc main.c -o app -I ./include

gcc main.c -o app -I ./include -l cal -L ./lib
#-I 指定include 包含文件的搜索目录
#-L 指定搜索库的路径
#-l 指定库的名称
```

![image-20210316110232199](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316110232199.png)

### 1.2动态库的制作

![image-20210316112939597](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316112939597.png)

```
静态库： GCC 进行链接时，会把静态库中代码打包到可执行程序中
动态库： GCC 进行链接时，动态库的代码不会被打包到可执行程序中
程序启动之后，动态库会被动态加载到内存中，通过 ldd (list dynamicdependencies)命令检查动态库依赖关系
如何定位共享库文件呢？
当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径。此时就需要系统的动态载入器来获取该绝对路径。对于 elf 格式的可执行程序，是由 ld-linux.so 来完成的，它先后搜索 elf 文件的 DT_RPATH 段 ————> 环境变量LD_LIBRARY_PATH ————> /etc/ld.so.cache 文件列表 ————> / lib/，/usr/lib目录找到库文件后将其载入内存。
```

**env 查看环境变量**

```shell
#动态库创建和使用
gcc -c -FPIC add.c sub.c div.c mult.c
gcc -shared *.o -o libcalc.so

#直接将编译出来的动态库进行使用，会导致如下错误
./main: error while loading shared libraries: libcalc.so: cannot open shared object file: No such file or directory

ldd main
#ldd可以列出一个程序所需要得动态链接库（so）
	linux-vdso.so.1 (0x00007fffa6bd1000)
	libcalc.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb461e2c000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fb46241f000)
	
#使用动态库需要将动态库导入到环境变量中
1.利用export在终端导入
export LD_LIBRARY_PATH =$LD_LIBRARY_PATH:~/Linux/Lession3/library/lib
2.将环境变量导入bashrc脚本中
export LD_LIBRARY_PATH =$LD_LIBRARY_PATH:~/Linux/Lession3/library/lib
3.将环境变量添加到/etc/profile
export LD_LIBRARY_PATH =$LD_LIBRARY_PATH:~/Linux/Lession3/library/lib
4.将环境变量添加到/etc/ld.so.conf
/home/wen/Linux/Lession3/library/lib

ldd main
	linux-vdso.so.1 (0x00007ffce8f77000)
	libcalc.so => /home/wen/Linux/Lession3/library/lib/libcalc.so (0x00007f7e7eec7000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f7e7ead6000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f7e7f2cb000)
```

### 1.3动静态库的优缺点

PS：一般比较小的库使用静态库，比较大的库使用动态库

**静态库**

> **优点：**
>
> 静态库加载到程序中加载速度快
>
> 发布程序不需要提供静态库，移植方便

> **缺点：**
>
> 消耗系统资源、浪费内存
>
> 更新部署发布麻烦

![image-20210316162727444](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316162727444.png)

**动态库**

> **优点**
>
> 可以实现进程间资源共享(共享库)
>
> 更新、部署、发布简单
>
> 可以控制何时加载动态库

> **缺点**
>
> 加载速度比静态库慢
>
> 发布程序需要提供一来的静态库

![image-20210316163651073](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316163651073.png)

### 1.4Makefile

- 文件命名

  Makefile或者makefile

- makefile规则

  一个makefile中有一个或者多个规则

  ![image-20210316165401508](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316165401508.png)

  目标：最终要生成的文件

  依赖：生成目标所需要的文件或者是目标

  命令：通过执行命令对依赖操作生成目标（命令前必须有tap缩进）

- makefile中的所有命令都是为第一条命令服务的

- 命令执行之前会检查依赖是否存在

  - 如果依赖存在，执行命令
  - 如果不存在，向下检查，看在后边的规则中是否有可以用来生成这一个依赖的，如果找到则执行

- 检查更新，在执行的过程中，会比较目标和依赖文件之间的时间

  - 如果依赖的时间比目标的时间晚，则重新生成
  - 如果执行依赖的时间比目标的时间早，则不需要重新更新，对应规则的命令不需要执行

- **变量**

  - 自定义变量
    - 变量名 = 变量值  	var = hello
  - 预定义变量
    - AR	归档维护程序的名称，默认值为ar
    - CC     C的编译器的名称，默认值为cc
    - CXX    c++编译器的名称，默认为g++
    - $@    目标完整的名称
    - $<      第一个依赖文件的名称
    - $^       所有的依赖文件
  - 获取变量的值
    - $(变量名)

![image-20210316195224136](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316195224136.png)

![image-20210316195144963](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316195144963.png)

- **函数**

  - $(wildcard PATTERN)

    - 功能：获取指定目录下的指定类型的文件列表

    - 参数: PATTERN 指的是某个或多个目录下的对应的某种类型的文件，如果有多个目录，一般使用空格间隔

    - 返回: 得到的若干个文件的文件列表，文件名之间使用空格间隔

    - 示例

      ![image-20210316201213932](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316201213932.png)

  - ![image-20210316203555012](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316203555012.png)

```makefile
#定义变量
#src=sub.o add.o div.o mult.o main.o
src = $(wildcard ./*.c)
objs = $(patsubst %.c, %.o, $(src))

target=app
$(target):$(objs)
        $(CC) $(objs) -o $(target) 

%.o:%.c
        $(CC) -c $^ -o $@
        
.PHONY:clean
#将clean声明为一个伪目标，就不会在文件内检测clean文件的创建时间
clean:
        rm $(objs) -f

```

### 1.5GDB

- 一般来说，GDB主要帮助你完成下面四个方面的功能:
  1.启动程序，可以按照自定义的要求随心所欲的运行程序
  2.可让被调试的程序在所指定的调置的断点处停住(断点可以是条件表达式)
  3.当程序被停住时，可以检查此时程序中所发生的事
  4.可以改变程序，将一个BUG产生的影响修正从而测试其他BUG.

![image-20210316202744807](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210316202744807.png)

- **准备工作**
  - ■通常，在为调试而编译时，我们会() 关掉编译器的优化选项(-o)，并打开调试选项(-g)。另外，-Wall在尽量不影响程序行为的情况下选项打开所有warning，也可以发现许多问题，避免一些不必要的BUG。
    gcc -g -Wall program.c -o program
    ■ -g 选项的作用是在可执行文件中加入源代码的信息，比如可执行文件中第几条机器指令对应源代码的第几行，但并不是把整个源文件嵌入到可执行文件中，所以在调试时必须保证gdb能找到源文件。，
- **GDB命令-启动、退出、查看代码**

```shell
#启动和退出
gdb 可执行程序
quit

#给程序设置参数获取参数
set args 10 20
show args

#GDB帮助
help

#查看当前文件代码
list/l 从默认位置显示
list/l 行号 从指定行显示
list/l 函数名称 从指定函数显示

#查看非当前文件代码
list/l 文件名:行号
list/l 文件名：函数名

#设置显示的函数
show list/listsize
set list/listsize 行数

#设置断点
b/break行号
b/break函数名
b/break文件名:行号
b/break文件名:函数
#查看断点
i/info b/break

#删除断点
d/del/delete断点编号

#设置断点无效
dis/disable断点编号
#设置断点生效
ena/enable断点编号
#设置条件断点(一般用在循环的位置)
b/break 10 if i==5

```

调试步骤

```shell
#生成可以调试的代码
gcc test.c -o test -g 

#驱动GDB
gdb test

(gdb) set args 10 20
#gdb设置参数
(gdb) show args
#获取gdb参数
Argument list to give program being debugged when it is started is "10 20".

quit
#退出

```

```shell
#gdb命令-调试命令
#运行GDB程序
start 	(程序停在第-行)
run 	(遇到断点才停)
#继续运行，到下一个断点停
c/continue
#向下执行一行代码(不会进入函数体)
n/next
#变量操作
p/print	变量名(打印变量值)
ptype	变量名(打印变量类型)

#向下单步调试(遇到函数进入函数体)
s/step
finish 	(跳出函数体)函数体内部不能有断点

#自动变量操作
display num 	(自动打印指定变量的值)
i/info display
undisplay		编号

#其它操作
set var 		变量名=变量值
until(跳出循环) #循环内没有断点
```

### 1.6I/O

#### 1.6.1标准c库I/O函数

![image-20210317092438159](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210317092438159.png)

#### 1.6.2c语言标准I/O和Linux系统I/O关系

![image-20210317092945202](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210317092945202.png)

#### 1.6.3 虚拟地址空间

为了实现更好的对内存进行管理

![image-20210317093150486](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210317093150486.png)

> 堆空间要比栈空间大
>
> 堆空间的地址从小到大，栈空间的地址从大到小
>
> 内核空间用户不能直接访问，但是可以调用系统函数来进行访问



#### 1.6.4 文件描述符

![image-20210317093628646](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210317093628646.png)

> 默认文件描述符为1，2，3的标号已经被占用，同时与系统当前的终端相连接
>
> （Linux中一切皆为文件）

#### 1.6.5 Linux系统I/O函数

PS：使用man 2 open 来查看Linux的系统I/O函数，man 3 open来查看标准c库的I/O函数

```c
int open (const char *pathname, int flags);
int open (const char *pathname, int flags, mode_ t mode);
int close (int fd);
ssize_t read(int fd， void *buf， size t count);
ssize_t write (int fd， const void *buf， size t count);
off_t lseek(int fd， off t offset， int whence);
int stat (const char *pathname， struct stat *statbuf);
int lstat (const char *pathname， struct stat *statbuf);
```

 示例

```c
/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    // 打开一个已经存在的文件
    int open(const char *pathname, int flags);
        参数：
            - pathname：要打开的文件路径
            - flags：对文件的操作权限设置还有其他的设置
              O_RDONLY,  O_WRONLY,  O_RDWR  这三个设置是互斥的
        返回值：返回一个新的文件描述符，如果调用失败，返回-1

    errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号。
  
    #include <stdio.h>
    void perror(const char *s);作用：打印errno对应的错误描述
        s参数：用户描述，比如hello,最终输出的内容是  hello:xxx(实际的错误描述)
    

    // 创建一个新的文件
    int open(const char *pathname, int flags, mode_t mode);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    // 打开一个文件
    int fd = open("a.txt", O_RDONLY);

    if(fd == -1) {
        perror("open");
    }
    // 读写操作

    // 关闭
    close(fd);

    return 0;
}

```

```c
/*
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

       int open(const char *pathname, int flags, mode_t mode);
        -pathname  要打开的文件路径
        -flags：   对文件的操作权限
            O_RDONLY, O_WRONLY, or O_RDWR.
            O_CREAT
            If pathname does not exist, create it as a regular file.
        -mode
            八进制的数，表示创建出的新的文件的操作权限，比如0775
            (mode & ~umask)->抹去一些权限
            umask  0002
            设置umask的值-> umask 0003    只在当前终端有效

            0777    ->  111111111
        &    0775    ->  111111101
            -> 111111101
        drw （当前用户权限）xrw （当前组的权限）xr-x（其他权限） 2 wen wen 4096 Mar 16 18:52 openFile

        flags int类型占有4个字节，32位
        flags 32 位都是标志位

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    int fd = open("craeat.txt",O_RDWR|O_CREAT,0777);
    if(fd==-1){
        perror("open");        
    }

    close(fd);

    return 0;

}
```



**read和write函数**

```c
/*

       #include <unistd.h>

       ssize_t read(int fd, void *buf, size_t count);
        -fd 文件描述符
        -buff 文件缓冲区，数组的地址
        -count 指定的数组大小


        return value
        成功：
        = 0 读完文件
        > 0 返回实际读到的字节数
        失败：
        =-1，并设置errno



       #include <unistd.h>
       ssize_t write(int fd, const void *buf, size_t count);
       同上


*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd1 = open("english.txt",O_RDONLY);
    char buff[1024] = {};

    if(fd1 == -1){
        perror("open file");
    }

    int fd2 = open("copy.txt",O_WRONLY|O_CREAT,0664);
    //0664为赋予文件的权限
    int byte_sum = 0;
    while ((byte_sum = read(fd1,buff,sizeof(buff)))>0)
    {
        write(fd2,buff,byte_sum);
        /* code */
    }
    
    

    close(fd1);
    close(fd2);

    return 0;

}
```



**stat file 查看文件信息**

stat结构体

![image-20210318094903977](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318094903977.png)

![image-20210318100208801](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318100208801.png)

```c
/*       
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>

       int stat(const char *pathname, struct stat *statbuf);
            -获取文件相关的信息
            -参数
                -pathname 文件路径
                -*statbuf 结构体变量，传出参数，保存获取到的文件信息

            -返回值
                成功 返回 0
                失败 返回 -1 设置errno


       int fstat(int fd, struct stat *statbuf);
       int lstat(const char *pathname, struct stat *statbuf);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main(){

    struct stat statbuf;
    
    int ret = stat("a.txt",&statbuf);
    if(ret == -1){
        perror("stat ");
        return -1;
    }

    printf("size : %ld\n",statbuf.st_size);

    return 0;
}
```

**ln -s a.txt b.txt** 	创建软连接

lstat和stat的区别

> stat获取文件的信息，也就是软连接指向的文件的信息
>
> lstat获取文件信息，并不是软连接指向的文件信息

使用stat函数完成ls -l的功能

```c
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
    char perm[11] = {};
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
```

**文件属性的操作**

文件属性的查看

```c
/*
       #include <unistd.h>

       int access(const char *pathname, int mode);
        获取文件的权限，检查文件是否存在
        参数
            -pathname 文件目录
            -mode   指定的是可以检查的权限
                R_OK:判断文件是否有读权限
                W_OK:判断文件是否有写权限
                X_OK:判断文件是否有执行权限
                F_OK:判断文件是否存在
        返回值
            成功返回0
            失败返回-1


*/
#include <unistd.h>
#include <stdio.h>


int main(){

    int re = access("a.txt",F_OK);
    if(re == -1){
        perror("access");
        printf("文件不存在\n");
        return -1;

    }c
    printf("文件存在\n");

    return 0;
}
```

文件权限的修改

```c
/*
       #include <sys/stat.h>

       int chmod(const char *pathname, mode_t mode);
       参数：
        -pathname文件目录
        -mode：需要修改的权限值
       返回值：
        成功 0
        失败 -2

*/
#include <sys/stat.h>
#include <stdio.h>

int main(){
    int re = chmod("a.txt",0775);
    if(re == -1){
        perror("chmod");
    }
    printf("修改成功");

    return 0;
}
```

查看用户和组ID

vim  /etc/password

![image-20210318152955429](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318152955429.png)

所有者的ID和所在组的ID

查看组也可以通过

vim  /etc/group



创建用户

**sudo useradd gaogao**

```C
/*
    #include <unistd.h>
    int chown(const char *pathname, uid_t owner, gid_t group);
    
    -pathname 文件路径
    -owner 所有者的ID
    -group 组用户ID

*/
```

**修改文件大小的方法**

```c
/*
       #include <unistd.h>
       #include <sys/types.h>

       int truncate(const char *path, off_t length);
        作用：缩减或者增加文件的尺寸
        参数：
            -pathname
            -length 目标文件的长度

*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    int re = truncate("b.txt",20);

    if(re == -1){
        perror("truncate");
        return -1;
    }
    printf("更改成功");
    return 0;
}
```

#### 1.6.6 文件目录进行操作

![image-20210318154456400](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318154456400.png)

**mkdir函数**

```c
/*
       #include <sys/stat.h>
       #include <sys/types.h>


       int mkdir(const char *pathname, mode_t mode);
            作用：创建一个目录
            参数
            -pathname 创建的目录的路径
            -mode 文件权限（八进制 ）
            返回值
            成功 0
            失败 -1

*/
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(){

    int re = mkdir("Dir",0777);
    if(re == -1){
        perror("mkdir\n");
        return -1;
    }

    printf("创建成功\n");

    return 0;
}
```

**rename函数**

```C
/*
       #include <stdio.h>

       int rename(const char *oldpath, const char *newpath);

*/

#include <stdio.h>
int main(){
    int re = rename("Dir","dir");

    if(re == -1){
        perror("rename\n");
        return -1;
    }
    printf("修改成功\n");

    return 0;
}
```

chdir和getcwd函数

```c
/*
    #include <unistd.h>
    int chdir(const char *path);
    作用：
        修改进程的工作目录
    参数：
        path 需要修改的工作目录

    #include <unistd.h>
    char *getcwd(char *buf, size_t size);
    作用：
        获取当前的工作目录
    参数
        -buf 存储的路径指向一个数组
        -size 数组的大小

    返回值
        返回指向的一块内存，这个数据就是第一个参数

 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

    char buf[128];
    getcwd(buf,sizeof(buf));

    printf("当前的工作目录：%s\n",buf);

    int re = chdir("/home/wen/Linux/Lession10");
    if(re == -1){
        perror("chdir\n");
        return -1;
    }

    //创建一个新的文件
    int fd = open("chdir.txt",O_RDWR | O_CREAT,0777);
    if(fd == -1){
        perror("open");
        return -1;
    }

    close(fd);

    getcwd(buf,sizeof(buf));
    printf("当前的工作目录：%s\n",buf);

    return 0;
}

```

**目录遍历**

![image-20210318165006939](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318165006939.png)



![image-20210318165408175](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318165408175.png)

**统计目录下包括的一般文件的数目**

```C
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
```

**操作文件描述符**

![image-20210318192829013](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318192829013.png)

dup函数使用

```c
/*
        #include <unistd.h>
        int dup(int oldfd);
            作用：复制一个新的文件描述符（多个文件描述符可以指向同一个文件）
            fd = 3,int fd1 = dup(fd),
            fd指向a.txt，fd1也指向a.txt
            从空闲的文件描述表中找到一个最小的，作为拷贝文件描述符
*/
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(){
    int fd = open("a.txt",O_WRONLY | O_CREAT,0664);

    int fd1 = dup(fd);
    if(fd1 == -1){
        perror("dup");
        return -1;
    }

    printf("fd : %d,fd1 : %d",fd,fd1);
    close(fd);

    char * str = "hello world";
    int re = write(fd1,str,strlen(str));
    if(re == -1){
        perror("write");
        return -1;
    }
    close(fd1);
    return 0;
}
```

**dup2函数**

```c
/*
    #include <unistd.h>
    int dup(int oldfd);
        作用：重定向文件描述符
        oldfd 指向 a.txt,newfd 指向 b.txt
        调用函数成功后：newfd 和 b.txt做close，newfd指向a.txt

        oldfd必须为一个有效的文件描述符
        oldfd和newfd值相同，相当于什么都不做
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

    int fd = open("1.txt",O_RDWR | O_CREAT,0664);
    if(fd == -1){
        perror("open");
        return -1;
    }

    int fd1 = open("2.txt",O_RDWR|O_CREAT,0664);
    if(fd1 == -1){
        perror("open");
        return -1;
    }

    printf("fd %d , fd1 %d\n",fd, fd1);

    int fd2 = dup2(fd,fd1);
    if(fd2 == -1){
        perror("dup2");
        return -1;
    }
    char * str = "hello ,dup2";
    // 通过fd1去写数据，实际操作的是1.txt，而不是2.txt
    int len = write(fd1,str,strlen(str));
    if(len == -1){
        perror("write");
        return -1;
    }

    printf("fd : %d, fd1 : %d fd2 : %d\n",fd,fd1,fd2);
    //fd2和fd1相同
    close(fd);
    close(fd1);

    return 0;
}
```

**fcntl函数**

![image-20210318215607001](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210318215607001.png)

```c
/*

    #include <unistd.h>
    #include <fcntl.h>

    int fcntl(int fd, int cmd, ...);
    参数：
        fd : 表示需要操作的文件描述符
        cmd: 表示对文件描述符进行如何操作
            - F_DUPFD : 复制文件描述符,复制的是第一个参数fd，得到一个新的文件描述符（返回值）
                int ret = fcntl(fd, F_DUPFD);

            - F_GETFL : 获取指定的文件描述符文件状态flag
              获取的flag和我们通过open函数传递的flag是一个东西。

            - F_SETFL : 设置文件描述符文件状态flag
              必选项：O_RDONLY, O_WRONLY, O_RDWR 不可以被修改
              可选性：O_APPEND, O)NONBLOCK
                O_APPEND 表示追加数据
                NONBLOK 设置成非阻塞
        
        阻塞和非阻塞：描述的是函数调用的行为。
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {

    // 1.复制文件描述符
    // int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);

    // 2.修改或者获取文件状态flag
    int fd = open("1.txt", O_RDWR);
    if(fd == -1) {
        perror("open");
        return -1;
    }

    // 获取文件描述符状态flag
    int flag = fcntl(fd, F_GETFL);
    if(flag == -1) {
        perror("fcntl");
        return -1;
    }
    flag |= O_APPEND;   // flag = flag | O_APPEND

    // 修改文件描述符状态的flag，给flag加入O_APPEND这个标记
    int ret = fcntl(fd, F_SETFL, flag);
    if(ret == -1) {
        perror("fcntl");
        return -1;
    }

    char * str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}
```

## 2. Linux多进程开发

### 2.1 进程概述

**程序和进程**

**程序是包含一系列信息的文件，这些信息描述了如何在运行时创建一个进程：**

- 二进制格式标识：每个程序文件都包含用于描述可执行文件格式的元信息。内核利用此信息来解释文件中的其他信息。（ELF可执行连接格式）
- 机器语言指令：对程序算法进行编码。
- 程序入口地址：标识程序开始执行时的起始指令位置。
- 数据：程序文件包含的变量初始值和程序使用的字面量值（比如字符串）。
- 符号表及重定位表：描述程序中函数和变量的位置及名称。这些表格有多重用途，其中包括调试和运行时的符号解析（动态链接）。
- 共享库和动态链接信息：程序文件所包含的一些字段，列出了程序运行时需要使用的共享库，以及加载共享库的动态连接器的路径名。
- 其他信息：程序文件还包含许多其他信息，用以描述如何创建进程。

> **进程是正在运行的程序的实例。是一个具有一定独立功能的程序关于某个数据集合的一次运行活动。它是操作系统动态执行的基本单元，在传统的操作系统中，进程既是基本的分配单元，也是基本的执行单元。**

> **可以用一个程序来创建多个进程，进程是由内核定义的抽象实体，并为该实体分配用以执行程序的各项系统资源。从内核的角度看，进程由用户内存空间和一系列内核数据结构组成，其中用户内存空间包含了程序代码及代码所使用的变量，而内核数据结构则用于维护进程状态信息。记录在内核数据结构中的信息包括许多与进程相关的标识号（IDs）、虚拟内存表、打开文件的描述符表、信号传递及处理的有关信息、进程资源使用及限制、当前工作目录和大量的其他信息**。

**单道、多道程序设计**

```c
/*
1.单道程序，即在计算机内存中只允许一个的程序运行。
2.多道程序设计技术是在计算机内存中同时存放几道相互独立的程序，使它们在管理程序控制下，相互穿插运行，两个或两个以上程序在计算机系统中同处于开始到结束之间的状态, 这些程序共享计算机系统资源。引入多道程序设计技术的根本目的是为了提高 CPU 的利用率。
3.对于一个单 CPU 系统来说，程序同时处于运行状态只是一种宏观上的概念，他们虽然都已经开始运行，但就微观而言，任意时刻，CPU 上运行的程序只有一个。
4.在多道程序设计模型中，多个进程轮流使用 CPU。而当下常见 CPU 为纳秒级，1秒可以执行大约 10 亿条指令。由于人眼的反应速度是毫秒级，所以看似同时在运行。
*/
```

**时间片**

```c
/*
1.时间片（timeslice）又称为“量子（quantum）”或“处理器片（processor slice）”是操作系统分配给每个正在运行的进程微观上的一段 CPU 时间。事实上，虽然一台计算机通常可能有多个 CPU，但是同一个 CPU 永远不可能真正地同时运行多个任务。在只考虑一个 CPU 的情况下，这些进程“看起来像”同时运行的，实则是轮番穿插地运行，由于时间片通常很短（在 Linux 上为 5ms－800ms），用户不会感觉到
2.时间片由操作系统内核的调度程序分配给每个进程。首先，内核会给每个进程分配相等的初始时间片，然后每个进程轮番地执行相应的时间，当所有进程都处于时间片耗尽的状态时，内核会重新为每个进程计算并分配时间片，如此往复。

*/
```

**并行和并发**

```c
/*
1.并行(parallel)：指在同一时刻，有多条指令在多个处理器上同时执行。
2.并发(concurrency)：指在同一时刻只能有一条指令执行，但多个进程指令被快速的轮换执行，使得在宏观上具有多个进程同时执行的效果，但在微观上并不是同时执行的，只是把时间分成若干段，使多个进程快速交替的执行。
*/
```

![image-20210319111857785](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319111857785.png)

**进程控制块（PCB**）

```c
/*
1.为了管理进程，内核必须对每个进程所做的事情进行清楚的描述。内核为每个进程分配一个 PCB(Processing Control Block)进程控制块，维护进程相关的信息，Linux 内核的进程控制块是 task_struct 结构体。
2.在 /usr/src/linux-headers-xxx/include/linux/sched.h 文件中可以查看 struct task_struct 结构体定义。其内部成员有很多，我们只需要掌握以下部分即可：
	进程id：系统中每个进程有唯一的 id，用 pid_t 类型表示，其实就是一个非负整数
	进程的状态：有就绪、运行、挂起、停止等状态
	进程切换时需要保存和恢复的一些CPU寄存器
	描述虚拟地址空间的信息
	描述控制终端的信息
	当前工作目录（Current Working Directory）
	umask 掩码
	文件描述符表，包含很多指向 file 结构体的指针
	和信号相关的信息
	用户 id 和组 id
	会话（Session）和进程组
	进程可以使用的资源上限（Resource Limit）
*/
```

### 2.2  进程的状态

> 进程状态反映进程执行过程的变化。这些状态随着进程的执行和外界条件的变化而转换。在三态模型中，进程状态分为三个基本状态，即就绪态，运行态，阻塞态。在五态模型中，进程分为新建态、就绪态，运行态，阻塞态，终止态。

```c
/*
运行态：进程占有处理器正在运行

就绪态：进程具备运行条件，等待系统分配处理器以便运行。当进程已分配到除 CPU 以外的所有必要资源后，只要再获得 CPU ，便可立即执行。在一个系统中处于就绪状态的进程可能有多个，通常将它们排成一个队列，称为就绪队列

阻塞态：又称为等待 ( 态或睡眠 ( 态，指进程不具备运行条件，正在等待某个事件的完成
*/
```

![image-20210319112827031](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319112827031.png)

![image-20210319112857339](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319112857339.png)

> **新建态：进程刚被创建时的状态，尚未进入就绪队列**
>
> **终止态：进程完成任务到达正常结束点，或出现无法克服的错误而异常终止，或被操作系统及有终止权的进程所终止时所处的状态。进入终止态的进程以后不再执行，但依然保留在操作系统中等待善后。一旦其他进程完成了对终止态进程的信息抽取之后，操作系统将删除该进程。**

**进程相关的命令**

```shell
ps aux / ajx
（相当于拍照）
a：显示终端上的所有进程，包括其他用户的进程
u：显示进程的详细信息
x：显示没有控制终端的进程
j：列出与作业控制相关的信息
```

```C
STAT 参数意义：
D	不可中断 Uninterruptible usually IO
R	正在运行，或在队列中的进程
S	(大写）处于休眠状态
T	停止或被追踪
Z	僵尸进程
W	进入内存交换（从内核 2.6 开始无效）
X	死掉的进程
<	高优先级
N	低优先级
s	包含子进程
+	位于前台的进程组
```

![image-20210319124308563](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319124308563.png)

![image-20210319124512842](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319124512842.png)

![image-20210319125007698](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319125007698.png)

### 2.3 进程创建

系统允许一个进程创建新进程，新进程即为子进程，子进程可以创建新的子进程，从而形成进程树的结构模型。



```c++
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);
	返回值：
		成功，返回0，父进程返回子进程的ID
		失败，返回-1
		失败的主要原因
			1.当前系统的进程数目已经达到了系统规定的上限。这是errno的值被设置为EAGAIN
			2.当系统内存不足时候，这是errno被设置为ENOMEN
	fork()的返回值会返回两次，一次是在父进程中一次是在子进程中
		在父进程中，返回创建的子进程的ID，在子进程中返回0;
		如何区分父进程和子进程：通过fork的返回值。
		在父进程中返回-1，表示创建子进程失败，并设置errno；
```

**fork()函数使用**

```c
/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){

    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //返回创建的子进程的进程号
        printf("pid : %d\n",pid);
        printf("I am parent process,pid :%d , ppid : %d\n",getpid(),getppid());

    }
    if(pid == 0){
        //当前是子进程
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
    }
    for(int i = 0;i<6;i++){
        printf("i ：%d  pid: %d\n",i,getpid());
        sleep(1);
    }

    return 0;c
}
```

![image-20210319163308092](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319163308092.png)

![image-20210319170432673](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319170432673.png)

```C
/*
实际上，更准确来说，Linux 的 fork() 使用是通过写时拷贝 (copy- on-write) 实现。
写时拷贝是一种可以推迟甚至避免拷贝数据的技术。
内核此时并不复制整个进程的地址空间，而是让父子进程共享同一个地址空间。
只用在需要写入的时候才会复制地址空间，从而使各个进行拥有各自的地址空间。
也就是说，资源的复制是在需要写入的时候才会进行，在此之前，只有以只读方式共享。
注意：fork之后父子进程共享文件，
fork产生的子进程与父进程相同的文件文件描述符指向相同的文件表，引用计数增加，共享文件偏移指针。
*/
```

```C
/*
        父子进程之间的关系：
        区别：
            1.fork()函数的返回值不同
                父进程中: >0 返回的子进程的ID
                子进程中: =0
            2.pcb中的一些数据
                当前的进程的id pid
                当前的进程的父进程的id ppid
                信号集

        共同点：
            某些状态下：子进程刚被创建出来，还没有执行任何的写数据的操作
                - 用户区的数据
                - 文件描述符表
        
        父子进程对变量是不是共享的？
            - 刚开始的时候，是一样的，共享的。如果修改了数据，不共享了。
            - 读时共享（子进程被创建，两个进程没有做任何的写的操作），写时拷贝。
*/
```

### 2.4 GDB多进程调试

使用GDB 调试的时候， GDB 默认只能跟踪一个进程，可以在 fork 函数调用之前，通过指令设置 GDB 调试工具跟踪父进程或者是跟踪子进程，默认跟踪父进程。

```shell
设置调试父进程或者子进程：
	set follow_fork_mode [parent （默认 ）| child]
	show follow_fork_mode 查看模式
设置调试模式：
	set detach_on_fork [on | off]
默认为on ，表示调试当前进程的时候，其它的进程继续运行，如果为 off ，调试当前进程的时候，其它进程被 GDB 挂起。

查看调试的进程：
	info inferiors
切换当前调试的进程：
	inferior id
使进程脱离GDB 调试： 
	detach inferiors id
```

### 2.5 exec函数族

- **exec函数族就是根据指定的文件名找到可执行的文件，并用它来取代调用进程的内容，换句话说就是在调用进行中执行一个可执行文件。**
- **exec函数族执行成功不会返回，因为调用进程的实体，包括代码段、数据段和堆栈段都已经被新的内容取代exec 函数族的函数执行成功后不会返回，因为调用进程的实体，包括代码段，数据段和堆栈等都已经被新的内容取代，只留下进程 ID 等一些表面上的信息仍保持原样，颇有些神似 三十六计 中的 金蝉脱壳 。看上去还是旧的躯壳，却已经注入了新的灵魂。只有调用失败了，它们才会返回 1 ，从原程序的调用点接着往下执行。**

```c
int execl(const char *path, const char *arg, .../* (char *) NULL */);

int execlp(const char *file, const char *arg, ... /* (char *) NULL */);

int execle(const char *path, const char *arg, .../*, (char *) NULL, char *const envp[] */);

int execv(const char *path, char *const argv[]);

int execvp(const char *file, char *const argv[]);

int execvpe(const char *file, char *const argv[], char *const envp[]);

int execve(const char *filename, char *const argv[], char *const envp[]);

l(list) 参数地址列表，以空指针结尾
v(vector) 存有各参数地址的指针数组的地址
p(path) 按 PATH 环境变量指定的目录搜索可执行文件
e(environment) 存有环境变量字符串地址的指针数组的地址
```

```c
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
```

### 2.6 过程控制

#### **2.6.1 进程退出**

```C
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);
```

![image-20210319221907017](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319221907017.png)

**exit函数**

```C
/*
#include <stdlib.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);
    -status参数：是进程退出时候的一个状态信息，父进程回收子进程的资源时候可以获得

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    printf("hello\n");
    printf("world!");
    //exit(0);
    // 执行成功，return 0就不会再执行
    _exit(0);
    // 不会刷新I/O缓冲区，printf第二句不会执行

    return 0;
}
```

#### 2.6.2 孤儿进程

- 父进程运行结束，但子进程还在运行（未运行结束），这样的子进程就称为孤儿进程Orphan Process ）。

- 每当出现一个孤儿进程的时候，内核就把孤儿进程的父进程设置为 init ，而 init进程会循环地 wait() 它的已经退出的子进程。这样，当一个孤儿进程凄凉地结束了其生命周期的时候， init 进程就会代表党和政府出面处理它的一切善后工作。

- 因此孤儿进程并不会有什么危害。

```c
/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){


    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //返回创建的子进程的进程号
        printf("I am parent process,pid :%d , ppid : %d\n",getpid(),getppid());

    }
    if(pid == 0){
        // 当前是子进程
        sleep(1);
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
        // 构造出孤儿进程
        

    }
    for(int i = 0;i<6;i++){
        printf("i ：%d  pid: %d\n",i,getpid());
      
    }

    return 0;
}
```

![image-20210319223748660](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319223748660.png)

出现以上输出的原因：

​	当系统的终端作为可执行文件的父进程，调用可执行文件之后转到后台，当可执行文件有输出就通过终端输出，但是终端当子进程结束时候，它自身的子进程仍然在运行，这个时候就变成了一个孤儿进程。孤儿进程的输出就会通过输入输出流，然后将信息输出终端设备上，因此就会出现这种状况。

#### 2.6.3 僵尸进程

- 每个进程结束之后 , 都会释放自己地址空间中的用户区数据，内核区的 PCB 没有办法自己释放掉，需要父进程去释放。
- 进程终止时，父进程尚未回收，子进程残留资源（ PCB ）存放于内核中，变成僵尸Zombie ）进程。
- 僵尸进程不能被 kill 9 杀死，这样就会导致一个问题，如果父进程不调用 wait()或 waitpid() 的话，那么保留的那段信息就不会释放，其进程号就会一直被占用，但是系统所能使用的进程号是有限的，如果大量的产生僵尸进程，将因为没有可用的进程号而导致系统不能产生新的进程，此即为僵尸进程的危害，应当避免。

```c
/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){


    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //返回创建的子进程的进程号
        while(1){
            printf("I am parent process,pid :%d , ppid : %d\n",getpid(),getppid());
            sleep(1);
        }
    }
    if(pid == 0){
        // 当前是子进程
        sleep(1);
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
        // 构造出孤儿进程
    }
    for(int i = 0;i<6;i++){
        printf("i ：%d  pid: %d\n",i,getpid());
      
    }

    return 0;
}
```

![image-20210319225656554](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210319225656554.png)

#### 2.6.4**进程的回收**

- 每个进程在退出的时候，内核会释放进程的所有资源、其中包括打开的文件、占用的内存等。但是仍然会为其保留一定的信息，这些信息主要是指进程控制块PCB的信息（包括进程号、退出状态、运行时间等）
- 父进程可以通过调用wait或者waitpid函数得到它的退出状态同时彻底清除这个进程。
- wait（）和waitpid（）函数的功能一样，区别仅仅在于wait（）函数会设置阻塞，waitpid（）函数可以设置不阻塞，waitpid（）还可以指定等待哪个子进程
- 注意：一次wait（）或者waitpid（）调用一个只能清理一个子进程，清理多个子进程应该用循环



**退出信息相关的宏函数**

- **WIFEXITED(status) 非 0 ，进程正常退出**
- **WEXITSTATUS(status) 如果上宏为真，获取进程退出的状态（ exit 的参数）**
- **WIFSIGNALED(status) 非 0 ，进程异常终止**

- **WTERMSIG(status) 如果上宏为真，获取使进程终止的信号编号**

- **WIFSTOPPED(status) 非 0 ，进程处于暂停状态**

- **WSTOPSIG(status) 如果上宏为真，获取使进程暂停的信号的编号**

- **WIFCONTINUED(status) 非 0 ，进程暂停后已经继续运行**



**wait函数代码：**

```c
/*
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *wstatus);
        -功能：
            等待任意一个子进程，如果任意子进程结束了，次函数会回收子进程的资源
        -参数：
            int *wstatus
            进程退出的状态信息，传入一个int类型的地址，传出参数
        -返回值：
            -成功 返回被回收的子进程ID
            -失败 返回-1
    调用wait函数，进程会被挂起（阻塞），知道它的一个子进程收到一个不能被忽略的信号才被唤醒
    （相当于继续向下执行）
    如果没有子进程，函数立即返回，返回-1，如果子进程已经结束了，也会立即返回-1，并且回收子进程的资源
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    // 一个父进程，创建五个子进程
    pid_t pid;
    // 创建五个子进程
    for(int i=0;i<5;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    } 

    if(pid > 0){
        //父进程
        while(1){
            printf("parent, pid = %d\n",getpid());
            // int re = wait(NULL);
            int state;
            
            int re = wait(&state);
            //阻塞,执行返回子进程的id
            if(re == -1){
                break;
            }
            if(WIFEXITED(state)){
                //正常退出
                printf("退出的状态码 ：%d\n",WEXITSTATUS(state));
            }
            if(WIFSIGNALED(state)){
                // 是否为异常终止
                printf("被哪个信号干掉了 ：%d\n",WTERMSIG(state));
            }
            printf("child die, pid = %d\n",re);
            sleep(1);
        }    
    }else if(pid == 0){
        // 子进程
        while(1){
            printf("child, pid = %d\n",getpid());
            sleep(1);
        }
        //exit(0);
    }
    return 0;
}
```

**waitpid代码**

```C
/*
       #include <sys/types.h>
       #include <sys/wait.h>

       pid_t waitpid(pid_t pid, int *wstatus, int options);
        -功能：回收指定进程号的的子进程，可以设置是否阻塞
        -参数：
            pid：
               pid > 0 表示回收的某个子进程的pid
               pid = 0 表示回收当前进程组的所有子进程
               pid = -1 表示回收所有得子进程
               pid < -1 表示回收某个进程组的组，id的绝对值
        -options
            0：阻塞
            WNOHANG：非阻塞

        -返回值：
            > 0 返回子进程id
            = 0 options=WNOHANGH，表示还有子进程或者错误
            = -1 错误，或者没有子进程了
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    // 一个父进程，创建五个子进程
    pid_t pid;
    // 创建五个子进程
    for(int i=0;i<5;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    } 

    if(pid > 0){
        //父进程
        while(1){
            printf("parent, pid = %d\n",getpid());
            // int re = wait(NULL);
            int state;
            // int re = waitpid(-1,&state,0);
            // //阻塞,执行返回子进程的id

            int re = waitpid(-1,&state,WNOHANG);
            // 非阻塞

            if(re == -1){
                break;
            }else if(re == 0){
                //说明还有子进程存在 
                sleep(1);
                continue;
               
            }else if(re > 0){
                if(WIFEXITED(state)){
                    //正常退出
                    printf("退出的状态码 ：%d\n",WEXITSTATUS(state));
                }
                if(WIFSIGNALED(state)){
                    // 是否为异常终止
                    printf("被哪个信号干掉了 ：%d\n",WTERMSIG(state));
                }
                printf("child die, pid = %d\n",re);
            }

            sleep(1);
        }    
    }else if(pid == 0){
        // 子进程
        while(1){
            printf("child, pid = %d\n",getpid());
            sleep(1);
        }
        //exit(0);
    }
    return 0;
}
```

### 2.7 进程间通信

#### 2.7.1进程间通信的概念

- ◼ 进程是一个独立的资源分配单元，不同进程（这里所说的进程通常指的是用户进程）之间 的资源是独立的，没有关联，不能在一个进程中直接访问另一个进程的资源。
- ◼ 但是，进程不是孤立的，不同的进程需要进行信息的交互和状态的传递等，因此需要进程 间通信( IPC：Inter Processes Communication )。
- ◼ 进程间通信的目的：
  - ◼ 数据传输：一个进程需要将它的数据发送给另一个进程。 
  - ◼ 通知事件：一个进程需要向另一个或一组进程发送消息，通知它（它们）发生了某种 事件（如进程终止时要通知父进程）。
  - ◼ 资源共享：多个进程之间共享同样的资源。为了做到这一点，需要内核提供互斥和同 步机制。
  - ◼ 进程控制：有些进程希望完全控制另一个进程的执行（如 Debug 进程），此时控制
    进程希望能够拦截另一个进程的所有陷入和异常，并能够及时知道它的状态改变。

![image-20210320220627560](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210320220627560.png)

#### 2.7.2管道

![image-20210321105439399](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210321105439399.png)

> **管道(有名和匿名)的特点：**
>
> 1. 管道其实是一个在内核内存中维护的缓冲器，这个缓冲器的存储能力是有限的，不同的操作系统大小不一定相同。
> 2. 管道拥有文件的特质：读操作、写操作，匿名管道没有文件实体，有名管道有文件实体， 但不存储数据。可以按照操作文件的方式对管道进行操作。
> 3. 一个管道是一个字节流，使用管道时不存在消息或者消息边界的概念，从管道读取数据的进程可以读取任意大小的数据块，而不管写入进程写入管道的数据块的大小是多少。
> 4. 通过管道传递的数据是顺序的，从管道中读取出来的字节的顺序和它们被写入管道的顺序是完全一样的。
> 5. 在管道中的数据的传递方向是单向的，一端用于写入，一端用于读取，管道是半双工的。
> 6. 从管道读数据是一次性操作，数据一旦被读走，它就从管道中被抛弃，释放空间以便写更多的数据，在管道中无法使用 lseek() 来随机的访问数据。
> 7. 匿名管道只能在具有公共祖先的进程（父进程与子进程，或者两个兄弟进程，具有亲缘关系）之间使用。

![image-20210321110244660](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210321110244660.png)

![image-20210321110521787](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210321110521787.png)

**管道的数据结构**

![image-20210321122053032](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210321122053032.png)

##### 2.7.2.1 匿名管道

- 匿名管道也叫无名管道，它是unix系统IPC（进程间通信）的最古老的方式，所有的unix系统都支持这种通信机制。
- 统计一个目录文件的数目的命令：ls | wc -l，为了执行该命令，shell创建了两个进程分别执行ls和wc。

**匿名管道的使用**

- 创建匿名管道 
  - #include <unistd.h> 
  - int pipe(int pipefd[2]);
- 查看管道缓冲大小命令 
  - ulimit –a
- 查看管道缓冲大小函数 
  - #include <unistd.h>
  - long fpathconf(int fd, int name);

![image-20210321122458536](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210321122458536.png)

```c
/*
    #include <unistd.h>
    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用来进程间通信。
        参数：int pipefd[2] 这个数组是一个传出参数。
            pipefd[0] 对应的是管道的读端
            pipefd[1] 对应的是管道的写端
        返回值：
            成功 0
            失败 -1

    管道默认是阻塞的：如果管道中没有数据，read阻塞，如果管道满了，write阻塞

    注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
*/

// 子进程发送数据给父进程，父进程读取到数据输出
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int pipnum[2]={0};
    int pip = pipe(pipnum);
    if(pip != 0){
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        printf("I am parent process pid: %d\n",getpid());
        close(pipnum[0]);
        char buff[1024]={0};
        while(1){
            // printf("I am parent process pid: %d\n",getpid());
            char * str = "I am chinese !!!";
            write(pipnum[1],str,strlen(str));
            
            
            read(pipnum[0],buff,sizeof(buff));
            printf("parent : I read buff: %s pid : %d\n",buff,getpid());
            sleep(1);
            //bzero(buff, 1024);
        }
    }else if(pid == 0){
        // 子进程
        printf("I am child process pid: %d\n",getpid());
        close(pipnum[1]);
        while (1)
        {
            char buf[1024]={0};
            
            read(pipnum[0],buf,sizeof(buf));
            printf("child : I read buf : %s pid : %d\n",buf,getpid());
            
            char * str1 = "hello world!!!";
            write(pipnum[1],str1,strlen(str1));
            sleep(1);
        }
    }
    return 0;
}
```

**PS：以上代码存在一些问题**

![pipe](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/pipe.jpg)

```c
/*

    实现 ps aux | grep xxx
        父子进程通信
    子进程：ps aux，子进程结束后，将数据发送给父进程
    父进程：获取数据，过滤

    pipe()
    execlp()
    子进程将标准输出重定向到管道的写端。

    缓存区只有4k,需要循环的去写数据
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main(){
    int fd[2];
    int ret = pipe(fd);
    // 创建匿名管道
    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    // 创建子进程
    pid_t pid = fork();
    if(pid == 0){
        // 子进程
        // 文件描述符重定向 stdout_fileno -> fd[1]
        // 关闭读端
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        // 执行 ps aux
        execlp("ps","ps","aux",NULL);

    }else if(pid > 0){
        // 父进程
        // 关闭写端
        close(fd[1]);
        char buf[1024]={0};
        int len = -1;
        while((len = read(fd[0],buf,sizeof(buf)-1)) > 0){
            printf("%s",buf);
            memset(buf,0,sizeof(buf));
        }
        wait(NULL);
        
        // 从管道读取数据，并过滤输出
    }else if(pid == -1){
        perror("fork");
        exit(0);
    }


    return 0;
}
```

**管道的读写特点**
使用管道时，需要注意以下几种特殊的情况（假设都是阻塞I/O操作）

- 1.所有的指向管道写端的文件描述符都关闭了（管道写端引用计数为0），有进程从管道的读端读数据，那么管道中剩余的数据被读取以后，再次read会返回0，就像读到文件末尾一样。
- 2.如果有指向管道写端的文件描述符没有关闭（管道的写端引用计数大于0），而持有管道写端的进程也没有往管道中写数据，这个时候有进程从管道中读取数据，那么管道中剩余的数据被读取后，再次read会阻塞，直到管道中有数据可以读了才读取数据并返回。
- 3.如果所有指向管道读端的文件描述符都关闭了（管道的读端引用计数为0），这个时候有进程向管道中写数据，那么该进程会收到一个信号**SIGPIPE**, 通常会导致进程异常终止。
- 4.如果有指向管道读端的文件描述符没有关闭（管道的读端引用计数大于0），而持有管道读端的进程也没有从管道中读数据，这时有进程向管道中写数据，那么在管道被写满的时候再次write会阻塞，直到管道中有空位置才能再次写入数据并返回。

**总结：**
    读管道：
        管道中有数据，read返回实际读到的字节数。
        管道中无数据：
            写端被全部关闭，read返回0（相当于读到文件的末尾）
            写端没有完全关闭，read阻塞等待

**写管道：**
    管道读端全部被关闭，进程异常终止（进程收到SIGPIPE信号）
    管道读端没有全部关闭：
        管道已满，write阻塞
        管道没有满，write将数据写入，并返回实际写入的字节数



**设置为非阻塞**

```c
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
int main() {

    /*
    设置非阻塞
    int flag = fcntl(fd[0],F_GETFL) //获取原来文件的标记、
    flag |= O_NOBLOCK;
    fcntl(fd[0],F_SETFL,flag);
    */
    // 在fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1) {
        perror("pipe");
        exit(0);
    }

    // 创建子进程
    pid_t pid = fork();
    if(pid > 0) {
        // 父进程
        printf("i am parent process, pid : %d\n", getpid());

        // 关闭写端
        close(pipefd[1]);
        int flag = fcntl(pipefd[0],F_GETFL); //获取原来文件的标记、
        flag |= O_NONBLOCK;
        fcntl(pipefd[0],F_SETFL,flag);
        // 从管道的读取端读取数据
        char buf[1024] = {0};
        while(1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len = %d\n",len);
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            memset(buf,0,sizeof(buf));
            sleep(1);
        }

    } else if(pid == 0){
        // 子进程
        printf("i am child process, pid : %d\n", getpid());
        // 关闭读端
        close(pipefd[0]);
        char buf[1024] = {0};
        while(1) {
            // 向管道中写入数据
            char * str = "hello,i am child";
            write(pipefd[1], str, strlen(str));
            sleep(2);
        }
        
    }
    return 0;
}
```

##### 2.7.2.2 有名管道

- ◼ 匿名管道，由于没有名字，只能用于亲缘关系的进程间通信。为了克服这个缺点，提 出了有名管道（FIFO），也叫命名管道、FIFO文件。

- ◼ 有名管道（FIFO）不同于匿名管道之处在于它提供了一个路径名与之关联，以 FIFO 的文件形式存在于文件系统中，并且其打开方式与打开一个普通文件是一样的，这样 即使与 FIFO 的创建进程不存在亲缘关系的进程，只要可以访问该路径，就能够彼此 通过 FIFO 相互通信，因此，通过 FIFO 不相关的进程也能交换数据。

- ◼ 一旦打开了 FIFO，就能在它上面使用与操作匿名管道和其他文件的系统调用一样的 I/O系统调用了（如read()、write()和close()）。与管道一样，FIFO 也有一 个写入端和读取端，并且从管道中读取数据的顺序与写入的顺序是一样的。FIFO 的
  名称也由此而来：先入先出。

- ◼ 有名管道（FIFO)和匿名管道（pipe）有一些特点是相同的，不一样的地方在于： 

- 1. FIFO 在文件系统中作为一个特殊文件存在，但 FIFO 中的内容却存放在内存中。
  2. 当使用 FIFO 的进程退出后，FIFO 文件将继续保存在文件系统中以便以后使用。

  3. FIFO 有名字，不相关的进程可以通过打开有名管道进行通信。

**有名管道的使用**

- ◼ 通过命令创建有名管道 

  ​	mkfifo 名字

- ◼ 通过函数创建有名管道 

  ```c
  #include <sys/types.h> 
  #include <sys/stat.h> 
  int mkfifo(const char *pathname, mode_t mode);
  ```

- ◼ 一旦使用 mkfifo 创建了一个 FIFO，就可以使用 open 打开它，常见的文件 I/O 函数都可用于 fifo。如：close、read、write、unlink 等。

- ◼ FIFO 严格遵循先进先出（First in First out），对管道及 FIFO 的读总是 从开始处返回数据，对它们的写则把数据添加到末尾。它们不支持诸如 lseek()等文件定位操作。

**创建fifo文件**

```c
/*
    创建fifo文件
    1.通过命令： mkfifo 名字
    2.通过函数：int mkfifo(const char *pathname, mode_t mode);

    #include <sys/types.h>
    #include <sys/stat.h>
    int mkfifo(const char *pathname, mode_t mode);
        参数：
            - pathname: 管道名称的路径
            - mode: 文件的权限 和 open 的 mode 是一样的
                    是一个八进制的数
        返回值：成功返回0，失败返回-1，并设置错误号

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

    int re = mkfifo("fifo",0664);
    if(re == -1){
        perror("mkfifo");
        exit(0);
    }

    return 0;
}
```

**有名管道通信**

**write**

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// 向管道中写数据
/*
    有名管道的注意事项：
        1.一个为只读而打开一个管道的进程会阻塞，直到另外一个进程为只写打开管道
        2.一个为只写而打开一个管道的进程会阻塞，直到另外一个进程为只读打开管道

    读管道：
        管道中有数据，read返回实际读到的字节数
        管道中无数据：
            管道写端被全部关闭，read返回0，（相当于读到文件末尾）
            写端没有全部被关闭，read阻塞等待
    
    写管道：
        管道读端被全部关闭，进行异常终止（收到一个SIGPIPE信号）
        管道读端没有全部关闭：
            管道已经满了，write会阻塞
            管道没有满，write将数据写入，并返回实际写入的字节数。
*/
int main() {

    // 1.判断文件是否存在
    int ret = access("test", F_OK);
    if(ret == -1) {
        printf("管道不存在，创建管道\n");
        
        // 2.创建管道文件
        ret = mkfifo("test", 0664);

        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }       

    }

    // 3.以只写的方式打开管道
    int fd = open("test", O_WRONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }

    // 写数据
    for(int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("write data : %s\n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }

    close(fd);

    return 0;
}
```

**read**

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// 从管道中读取数据
int main() {

    // 1.打开管道文件
    int fd = open("test", O_RDONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }

    // 读数据
    while(1) {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == 0) {
            printf("写端断开连接了...\n");
            break;
        }
        printf("recv buf : %s\n", buf);
    }

    close(fd);

    return 0;
}
```

ps：如果管道的读端关闭，则写端也会终止，因为管道会破裂

![image-20210322131701043](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322131701043.png)

**简单的聊天代码**

chatA

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


int main(){
    // 判断有名管道是否存在
    int flag1 = access("fifo1",F_OK);
    if(flag1 == -1){
        int re1 = mkfifo("fifo1",0664);
        if(re1 == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    int flag2 = access("fifo2",F_OK);
    if(flag2 == -1){
        int re2 = mkfifo("fifo2",0664);
        if(re2 == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    int fdr = open("fifo1",O_RDONLY);
    if(fdr == -1){
        perror("open");
        exit(0);
    }
    printf("管道fifo1打开成功\n");

    int fdw = open("fifo2",O_WRONLY);
    if(fdw == -1){
        perror("open");
        exit(0);
    }
    printf("管道fifo2打开成功\n");
    char buf[128];

    while (1)
    {   
        // 读管道数据
        memset(buf,0,sizeof(buf));
        int ret = read(fdr,buf,sizeof(buf));
        if(ret <= 0){
            perror("read");
            break;
        }
        printf("I reve: %s\n",buf);

        memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);

        int re = write(fdw,buf,strlen(buf));
        if(re == -1){
            perror("write");
            exit(0);
            // break;
        }
    }
    close(fdw);
    close(fdr);
    return 0;

}
```

chatB

```c
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
```

#有名管带在读写的时候会进行阻塞，只有同时打开读和写的时候才能成功

**进行聊天程序的改进**

> 将管道的读取和写入放在不同的进程中，就可以避免阻塞，实现普通意义上的聊天程序

#### 2.7.3 内存映射

内存映射（Memory-mapped I/O）是将磁盘文件的数据映射到内存，用户通过修改内存就能修改磁盘文件。

![image-20210322145019613](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322145019613.png)

```c
#include <sys/mman.h> 
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```

**父子进程通过内存映射区实现进程间通信**

```c
/*
    #include <sys/mman.h>

    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    -功能
        映射一个文件或者设备到内存中
    -参数
        -void *addr：（起始地址）传递一个null，由内核指定
        -length ： 要映射的数据的长度，这个值不能为零。建议使用文件的长度。
            获取文件的长度：
                stat函数或者lseek函数，Linux分配内存大小的默认会取分页的整数倍
        -prot ：对我们申请的内存映射区的一个操作权限
            -PROT_EXEC  Pages may be executed.
            -PROT_READ  Pages may be read.
            -PROT_WRITE Pages may be written.
            -PROT_NONE  Pages may not be accessed.
            需要操作的内存必须要有读的权限
            一般写 PROT_READ 或者 PROT_READ | PROT_WRITE
        -flags：
            -MAP_SHARED ：映射区的数据会自动和磁盘文件进行同步，进程间通信必须要有这个选项
            -MAP_PRIVATE ：不同步，内存映射区的数据改变了，不会对原来的文件进行修改，会重新创建一个新的文件（copy on write）
        -fd： 需要映射的文件的文件描述符，由open()得到
            -文件的大小不能为0，open指定的权限不能和prot参数权限冲突
                prot ：PROT_READ    open：只读/读写
                prot ：PROT_READ | PROT_WRITE   open ：读写权限
        -offset ：偏移量
            -一般不适用，必须指定为4K的整数倍，0表示不偏移
    -返回值 ：
        失败返回：(void *) -1
        成功返回：创建内存的首地址

    int munmap(void *addr, size_t length);
    -功能：释放内存映射
    -参数：
        -addr 释放内存的首地址
        -length 要释放的内存大小，要和mmap内存中length参数值一样
*/
// 使用内存映射实现进程间通信
/**
 * 1.有关系的进程（父子进程）
 *      -没有子进程的时候：
 *          -通过唯一的父进程，先创建内存映射区
 *      -有了内存映射区以后，创建子进程
 *      -父子进程共享创建的内存映射区
 * 2.没有关系的进程间通信
 *      -准备一个大小不是0的磁盘文件
 *      -进程1：通过磁盘文件创建内存映射区
 *          -得到一个操作这块内存的指针
 *      -进程2：通过磁盘文件创建内存映射区
 *          -得到一个操作这块内存的指针
 *      -使用内存映射区通信
 *  内存映射区通信不会阻塞
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
    // 1.打开一个文件
    int fd = open("test.txt",O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }
    
    int file_size = lseek(fd,0,SEEK_END);//获取文件的大小
    void * ptr = mmap(NULL,file_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    // 3.创建进程
    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        strcpy((char * )ptr,"ni hao,son");

    }else if(pid == 0){
        // 子进程
        // 定义一个数组
        char buf[64] = {0};
        strcpy(buf,ptr);
        printf("I am son rev : %s\n",buf);

    }else if(pid == -1){
        perror("fork");
        exit(0);
    }


    munmap(ptr,file_size);

    
    return 0;
}
```

**思考问题**

```
1.如果对mmap的返回值(ptr)做++操作(ptr++), munmap是否能够成功?
void * ptr = mmap(...);
ptr++;  可以对其进行++操作
munmap(ptr, len);   // 错误,要保存地址，用于最后对系统的内存进行释放

2.如果open时O_RDONLY, mmap时prot参数指定PROT_READ | PROT_WRITE会怎样?
错误，返回MAP_FAILED
open()函数中的权限建议和prot参数的权限保持一致。

3.如果文件偏移量为1000会怎样?
偏移量必须是4K的整数倍，返回MAP_FAILED

4.mmap什么情况下会调用失败?
    - 第二个参数：length = 0
    - 第三个参数：prot
        - 只指定了写权限
        - prot PROT_READ | PROT_WRITE
          第5个参数fd 通过open函数时指定的 O_RDONLY / O_WRONLY

5.可以open的时候O_CREAT一个新文件来创建映射区吗?
    - 可以的，但是创建的文件的大小如果为0的话，肯定不行
    - 可以对新的文件进行扩展
        - lseek()
        - truncate()

6.mmap后关闭文件描述符，对mmap映射有没有影响？
    int fd = open("XXX");
    mmap(,,,,fd,0);
    close(fd); 
    映射区还存在，创建映射区的fd被关闭，没有任何影响。

7.对ptr越界操作会怎样？
void * ptr = mmap(NULL, 100,,,,,);
4K
越界操作操作的是非法的内存 -> 段错误
```

**使用内存映射完成文件的复制**

```c
// 使用内存映射实现文件拷贝功能

/*
    思路：
        1.对文件进行内存映射
        2.创建一个新文件（拓展该文件）
        3.把新文件的数据映射到内存中
        4.通过内存拷贝第一个文件的内存数据拷贝到新的文件内存中
        5.释放资源
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>

int main(){
    // 1.对原始文件进行映射
    int fd = open("english.txt",O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }


    // 计算原始文件的长度
    struct stat statbuf;
    int re = stat("english.txt",&statbuf);
    // int len = lseek(fd,0,SEEK_END);

    int fd_new = open("cpFile.txt",O_CREAT|O_RDWR,0664);
    if(fd_new == -1){
        perror("open");
        exit(0);
    }
    truncate("cpFile.txt",statbuf.st_size);
    // 2.分别做内存映射
    void * ptr1 = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr1 == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    void * ptr2 = mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd_new,0);
    if(ptr2 ==MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    // 内存拷贝
    memcpy(ptr2,ptr1,statbuf.st_size);

    munmap(ptr1,statbuf.st_size);
    munmap(ptr2,statbuf.st_size);
    
    return 0;
}
```

**匿名映射**

```c
/*
    匿名映射,不需要文件实体进程一个内存映射

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <wait.h>
int main(){

    int len = 4096;
    void * ptr = mmap(NULL, len, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANON,-1,0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        strcpy((char *)ptr,"hello , world !");
        wait(NULL);

    }else if(pid == 0){
        // 子进程
        sleep(1);
        printf("%s\n",(char *)ptr);
    }else if(pid == -1){
        perror("fork");
        exit(0);
    }

    int re = munmap(ptr,len);
    if(re == -1){
        perror("munmap");
        exit(0);
    }
    return 0;
}
```

 

#### 2.7.4 信号

◼ 信号是 Linux 进程间通信的最古老的方式之一，是事件发生时对进程的通知机制，有时也称之为软件中断，它是在软件层次上对中断机制的一种模拟，是一种异步通信的方式。信号 可以导致一个正在运行的进程被另一个正在运行的异步进程中断，转而处理某一个突发事件。
◼ 发往进程的诸多信号，通常都是源于内核。引发内核为进程产生信号的各类事件如下： 

​		 对于前台进程，用户可以通过输入特殊的终端字符来给它发送信号。比如输入Ctrl+C 通常会给进程发送一个中断信号。
​		 硬件发生异常，即硬件检测到一个错误条件并通知内核，随即再由内核发送相应信号给 相关进程。比如执行一条异常的机器语言指令，诸如被 0 除，或者引用了无法访问的 内存区域。
​		 系统状态变化，比如 alarm 定时器到期将引起 SIGALRM 信号，进程执行的 CPU 时间超限，或者该进程的某个子进程退出。
​		 运行 kill 命令或调用 kill 函数。



◼ 使用信号的两个主要目的是： 

​		 让进程知道已经发生了一个特定的事情。 

​		 强迫进程执行它自己代码中的信号处理程序。

◼ 信号的特点： 

​		 简单 

​		 不能携带大量信息 

​		 满足某个特定条件才发送 

​		 优先级比较高

◼ 查看系统定义的信号列表：kill –l
◼ 前 31 个信号为常规信号，其余为实时信号。

![image-20210322211237711](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322211237711.png)



![image-20210322211330472](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322211330472.png)



![image-20210322211351600](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322211351600.png)



![image-20210322211731921](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322211731921.png)

◼ 查看信号的详细信息：man 7 signal 

◼ 信号的 5 中默认处理动作 

​	 Term 		终止进程 

​	 Ign			当前进程忽略掉这个信号

​	 Core 		终止进程，并生成一个Core文件 

​	 Stop 		暂停当前进程 

​	 Cont 		继续执行当前被暂停的进程

◼ 信号的几种状态：产生、未决、递达

◼ SIGKILL 和 SIGSTOP 信号不能被捕捉、阻塞或者忽略，只能执行默认动作。

![image-20210322213223283](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322213223283.png)

ulimit -a 	查看是否会生成core文件

 ![image-20210322213253287](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322213253287.png) 

ulimit -c	将参数设置为非零，则可以生成core文件

**使用gdb就可以通过core文件查看错误信息**

![image-20210322213717148](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210322213717148.png)

**信号相关的函数**

```c
int kill(pid_t pid, int sig);
int raise(int sig);
void abort(void);
unsigned int alarm(unsigned int seconds);
int setitimer(int which, const struct itimerval *new_val,struct itimerval *old_value);
```

**kill raise abort函数**

```c
/*

#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
    -功能 ：给任何的某个进程或者进程组发送信号，发送某个信号
    -参数 ：
        -pid ：需要发送给进程的pid
            pid > 0     将信号发送到指定的进程
            pid = 0     将信号发给当前的进程组
            pid = -1    将信号发给每一个有权限接受该信号的进程
            pid < -1    将信号发送给某个进程组（pid为进程组的pid的绝对值）
        -sig ：需要发送信号的编号或者宏值

    kill(getpid(),sig);

int raise(int sig);
    -功能 ：给当前进程发送信号
    -参数 ：要发送的信号或者信号的宏值
    -返回值 ：如果成功返回0，如果失败返回-1

void abort(void);
    -功能 ：发送SIGABRT信号给当前的进程杀死当前进程
    kill(getpid(),SIGABRT);
unsigned int alarm(unsigned int seconds);
int setitimer(int which, const struct itimerval *new_val,
                struct itimerval *old_value);
*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    pid_t pid = fork();
    if(pid == 0){
        // 子进程
        for(int i = 0;i<5;i++){
            printf("child process\n");
            sleep(1);
        }
    }else if(pid > 0){
        // 父进程
        printf("parent process\n");
        sleep(2);
        printf("Kill child process\n");
        kill(pid,SIGABRT);

    }else{
        perror("fork");
        exit(0);
    }

    return 0;
}
```

**alarm函数**

```c
/*
    一秒钟可以数多少个数字

    实际运行时间 = 内核时间 + 用户时间 + 消耗时间
    文件I/O比较耗费时间

    定时器和进程的状态无关，采用了自然定时法，无论进程处于什么状态alarm都会计时
    
*/

#include <unistd.h>
#include <stdio.h>

int main(){
    alarm(1);
    int i = 0;
    while (1)
    {
        printf("i = %d\n",i++);
    }
    
    return 0;

}
```

**setitimer()函数**

```c
/*
    #include <sys/time.h>
    int setitimer(int which, const struct itimerval *new_value,
                        struct itimerval *old_value);
    
        - 功能：设置定时器（闹钟）。可以替代alarm函数。精度微妙us，可以实现周期性定时
        - 参数：
            - which : 定时器以什么时间计时
              ITIMER_REAL: 真实时间，时间到达，发送 SIGALRM   常用
              ITIMER_VIRTUAL: 用户时间，时间到达，发送 SIGVTALRM
              ITIMER_PROF: 以该进程在用户态和内核态下所消耗的时间来计算，时间到达，发送 SIGPROF

            - new_value: 设置定时器的属性
            
                struct itimerval {      // 定时器的结构体
                struct timeval it_interval;  // 每个阶段的时间，间隔时间
                struct timeval it_value;     // 延迟多长时间执行定时器
                };

                struct timeval {        // 时间的结构体
                    time_t      tv_sec;     //  秒数     
                    suseconds_t tv_usec;    //  微秒    
                };

            过10秒后，每个2秒定时一次
           
            - old_value ：记录上一次的定时的时间参数，一般不使用，指定NULL
        
        - 返回值：
            成功 0
            失败 -1 并设置错误号
*/
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


int main(){
    struct itimerval newvalue;
    // 设置间隔时间
    newvalue.it_interval.tv_sec = 2;
    newvalue.it_interval.tv_usec = 0;
    // 设置延迟时间
    newvalue.it_value.tv_sec = 3;
    newvalue.it_value.tv_usec = 0;

    int re = setitimer(ITIMER_REAL,&newvalue,NULL);
    printf("定时器开始了！\n");
    if(re == -1){
        perror("setitimer");
        exit(0);
    }
    while(1){
        getchar();
    }
    return 0;
}
```

**信号捕捉函数**

```c
sighandler_t signal(int signum, sighandler_t handler); 
int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
```

**信号捕捉函数signal()实现：**

```c
/*
    #include <signal.h>
    sighandler 设置某个信号的捕捉行为
        -参数 :
            -signum 要捕捉的信号
            -handler 捕捉到信号要如何处理
                -SIG_IGN ：忽略信号
                -SIG_DEF ：使用默认信号的行为
                -回调函数 ：这个函数是通过内核调用，程序员只负责进行写
                回调函数：
                    - 需要程序员实现，提前准备好的，函数的类型根据实际需求，看函数指针的定义
                    - 不是程序员调用，而是当信号产生，由内核调用
                    - 函数指针是实现回调的手段，函数实现之后，将函数名放到函数指针的位置就可以了。                    
        -返回值 ：
            成功：返回上一次注册的处理函数的地址，第一次调用返回NULL
            失败：返回SIG_ERR,设置错误信号
            
    SIGKILL SIGSTOP 不能捕捉，不能被忽略
*/
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
void myProcess(int num);

int main(){

    // 注册信号捕捉
    // signal(SIGALRM,SIG_IGN);
    // signal(SIGALRM,SIG_DFL);
    // void ( * sighandler) (int); 函数指针,int为信号的编号
    signal(SIGALRM,myProcess);
    struct itimerval newvalue;
    // 设置间隔时间
    newvalue.it_interval.tv_sec = 2;
    newvalue.it_interval.tv_usec = 0;
    // 设置延迟时间
    newvalue.it_value.tv_sec = 3;
    newvalue.it_value.tv_usec = 0;

    int re = setitimer(ITIMER_REAL,&newvalue,NULL);
    printf("定时器开始了！\n");
    if(re == -1){
        perror("setitimer");
        exit(0);
    }
    // while(1){
        getchar();
    // }
    return 0;
}
void myProcess(int num){
    printf("回调函数被执行\n");
    printf("捕捉到的信号为：%d\n",num);

}
```

**信号集**

◼ 许多信号相关的系统调用都需要能表示一组不同的信号，多个信号可使用一个称之为信号集的数据结构来表示，其系统数据类型为 sigset_t。

◼ 在 PCB 中有两个非常重要的信号集。一个称之为 “阻塞信号集” （阻塞信号递达），另一个称之为 “未决信号集” 。这两个信号集都是内核使用位图机制来实现的。但操作系统不允许我 们直接对这两个信号集进行位操作。而需自定义另外一个集合，借助信号集操作函数 来对 PCB 中的这两个信号 集进行修改。

◼ 信号的 “未决” 是一种状态，指的是从信号的产生到信号被处理前的这一段时间。 

◼ 信号的 “阻塞” 是一个开关动作，指的是阻止信号被处理，但不是阻止信号产生。 

◼ 信号的阻塞就是让系统暂时保留信号留待以后发送。由于另外有办法让系统忽略信号，
所以一般情况下信号的阻塞只是暂时的，只是为了防止信号打断敏感的操作。

![image-20210323111525131](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210323111525131.png)

> 1.用户通过键盘  Ctrl + C, 产生2号信号SIGINT (信号被创建)
>
> 2.信号产生但是没有被处理 （未决）
>
>    - 在内核中将所有的**没有被处理的信号存储在一个集合中** （未决信号集）
>    - SIGINT信号状态被存储在第二个标志位上
>      - 这个标志位的值为0， 说明信号不是未决状态
>      - 这个标志位的值为1， 说明信号处于未决状态
>
> 3.这个未决状态的信号，需要被处理，处理之前需要和另一个信号集（阻塞信号集），进行比较
>
>    - 阻塞信号集默认不阻塞任何的信号
>    - 如果想要阻塞某些信号需要用户调用系统的API
>
> 4.在处理的时候和阻塞信号集中的标志位进行查询，看是不是对该信号设置阻塞了
>
>    - 如果没有阻塞，这个信号就被处理
>    - 如果阻塞了，这个信号就继续处于未决状态，直到阻塞解除，这个信号就被处理

**信号集相关的函数**

```c
int sigemptyset(sigset_t *set); 
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
```



```c
/*
以下信号集都是对我们自定义的信号集进行操作

int sigemptyset(sigset_t *set); 
    -功能：清空信号集中的数据，将信号集中的所有标志位置为0
    -参数：set 为一个传出参数，是我门需要操作的信号集
    -返回值：
        成功 0
        失败 -1
int sigfillset(sigset_t *set);
    -功能：将信号集中的所有标志位置为1
    -参数：set 为一个传出参数，是我门需要操作的信号集
    -返回值：
        成功 0
        失败 -1
int sigaddset(sigset_t *set, int signum);
    -功能：将信号集中的某一个标志位置为1，表示阻塞这个信号
    -参数：
        -set 为一个传出参数，是我门需要操作的信号集
        -signum 需要阻塞的那个信号
    -返回值：
        成功 0
        失败 -1
int sigdelset(sigset_t *set, int signum);
    -功能：将信号集中的某一个标志位置为0，表示不阻塞这个信号
    -参数：
        -set 为一个传出参数，是我门需要操作的信号集
        -signum 需要不阻塞的那个信号
    -返回值：
        成功 0
        失败 -1
int sigismember(const sigset_t *set, int signum);
    -功能：判断某个信号是否阻塞
    -参数：
        -set 是我门需要操作的信号集
        -signum 需要判断的那个信号
    -返回值：
        1 表示signum被阻塞
        0 表示signum不阻塞、
        -1 表示调用失败

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
*/
#include <stdio.h>
#include <signal.h>

int main(){
    // 创建一个信号集
    sigset_t set;

    // 清空信号集
    sigemptyset(&set);

    // 判断信号SIGINT是否在信号集中
    int flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);
    // 判断

    flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }

    flag = sigismember(&set,SIGQUIT);
    if(flag == 0){
        printf("SIGQUIT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGQUIT 阻塞\n");
    }
    // 删除信号
    sigdelset(&set,SIGQUIT);

    flag = sigismember(&set,SIGINT);
    if(flag == 0){
        printf("SIGINT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGINT 阻塞\n");
    }

    flag = sigismember(&set,SIGQUIT);
    if(flag == 0){
        printf("SIGQUIT 不阻塞\n");
    }else if(flag != 0){
        printf("SIGQUIT 阻塞\n");
    }

    return 0;
}
```

```shell
./sigpromask &
#以后台进程运行

fg
#切换到前台
```

```c
/*
    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
        - 功能：将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）
        - 参数：
            - how : 如何对内核阻塞信号集进行处理
                SIG_BLOCK: 将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变
                    假设内核中默认的阻塞信号集是mask， mask | set
                SIG_UNBLOCK: 根据用户设置的数据，对内核中的数据进行解除阻塞
                    mask &= ~set
                SIG_SETMASK:覆盖内核中原来的值
            
            - set ：已经初始化好的用户自定义的信号集
            - oldset : 保存设置之前的内核中的阻塞信号集的状态，可以是 NULL
        - 返回值：
            成功：0
            失败：-1
                设置错误号：EFAULT、EINVAL

    int sigpending(sigset_t *set);
        - 功能：获取内核中的未决信号集
        - 参数：set,传出参数，保存的是内核中的未决信号集中的信息。
*/

// 编写一个程序，把所有的常规信号（1-31）的未决状态打印到屏幕
// 设置某些信号是阻塞的，通过键盘产生这些信号
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    // 设置2，3号信号阻塞

    sigset_t set;
    sigemptyset(&set); 

    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);

    // 修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK,&set,NULL);
    int num = 0;
    while(1){
        // 获取当前未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);
        for(int i = 1;i<32;i++){
            //信号是从1号开始
            if(sigismember(&pendingset,i) == 1){
                printf("1");
            }else if(sigismember(&pendingset,i) == 0){
                printf("0");
            }else{
                perror("sigismember");
                exit(0);
            }
        }
        num++;
        
        sleep(1);
        printf("\n");
        if(num == 10){
            //break;
            sigprocmask(SIG_UNBLOCK,&set,NULL);
        }
    }

    return 0;
}

```

**sigaction()函数使用**

```c
/*
    #include <signal.h>
    int sigaction(int signum, const struct sigaction *act,
                            struct sigaction *oldact);

        - 功能：检查或者改变信号的处理。信号捕捉 
        - 参数：
            - signum : 需要捕捉的信号的编号或者宏值（信号的名称）
            - act ：捕捉到信号之后的处理动作
            - oldact : 上一次对信号捕捉相关的设置，一般不使用，传递NULL
        - 返回值：
            成功 0
            失败 -1

     struct sigaction {
        // 函数指针，指向的函数就是信号捕捉到之后的处理函数
        void     (*sa_handler)(int);
        // 不常用,第一个参数，int信号捕捉到的编号，siginfo保存信号相关的信息
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        // 临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号。
        sigset_t   sa_mask;
        // 使用哪一个信号处理对捕捉到的信号进行处理
        // 这个值可以是0，表示使用sa_handler,也可以是SA_SIGINFO表示使用sa_sigaction
        int        sa_flags;
        // 被废弃掉了
        void     (*sa_restorer)(void);
    };
*/
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
void myProcess(int num);

int main(){

    // 注册信号捕捉
    // signal(SIGALRM,SIG_IGN);
    // signal(SIGALRM,SIG_DFL);
    // void ( * sighandler) (int); 函数指针,int为信号的编号
    // signal(SIGALRM,myProcess);
    struct sigaction set;
    set.sa_flags = 0;
    set.sa_handler = myProcess;
    sigemptyset(&set.sa_mask);
    // 清空临时阻塞信号集

    sigaction(SIGALRM,&set,NULL);
    struct itimerval newvalue;
    // 设置间隔时间
    newvalue.it_interval.tv_sec = 2;
    newvalue.it_interval.tv_usec = 0;
    // 设置延迟时间
    newvalue.it_value.tv_sec = 3;
    newvalue.it_value.tv_usec = 0;

    int re = setitimer(ITIMER_REAL,&newvalue,NULL);
    printf("定时器开始了！\n");
    if(re == -1){
        perror("setitimer");
        exit(0);
    }
    while(1){
        getchar();
    }
    return 0;
}
void myProcess(int num){
    printf("回调函数被执行\n");
    printf("捕捉到的信号为：%d\n",num);

}
```

**内核实现信号捕捉的过程**

![image-20210323164707443](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210323164707443.png)

![image-20210323164859923](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210323164859923.png)

 

**回调时使用零时阻塞的信号集，在回调结束以后使用内核中的阻塞信号集。**

**常规信号阻塞不支持排队**



**利用SIGCHILD来处理僵尸进程**

- ◼ SIGCHLD信号产生的条件 
  -  子进程终止时 
  -  子进程接收到 SIGSTOP 信号停止时 
  -  子进程处在停止态，接受到SIGCONT后唤醒时
- ◼ 以上三种条件都会给父进程发送 SIGCHLD 信号，父进程默认会忽略该信号

```c
/*
    SIGCHILD信号产生的三个条件：
        1.子进程结束
        2.子进程暂停了
        3.子进程继续运行
        都会给父进程发送该信号，父进程默认忽略该信号
    使用SIGCHILD解决僵尸进程问题
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void MyFun(int num){
    printf("捕捉到的信号 ： %d\n",num);
    while (1)
    {
        //wait(NULL); //阻塞

        // 提前设置好阻塞信号集，阻塞SIGCHLD，因为子进程可能很快结束，信号没有被捕捉

        int re = waitpid(-1,NULL,WNOHANG);
        if(re > 0){
            printf("child die , pid = %d\n",re);
        }else if(re == 0){
            break;
        }else if(re == -1){
            perror("waitpid");
            exit(0);
        }
    }
}
int main(){

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    pid_t pid;
    // // 捕捉子进程死亡时发送的SIGCHILD信号
    // struct sigaction sig;
    // sig.sa_flags = 0;
    // sig.sa_handler = MyFun;
    // sigemptyset(&sig.sa_mask);
    // sigaction(SIGCHLD,&sig,NULL);

    for(int i = 0;i<20;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }
    if(pid > 0){
        // // 父进程
        // // 捕捉子进程死亡时发送的SIGCHILD信号
        struct sigaction sig;
        sig.sa_flags = 0;
        sig.sa_handler = MyFun;
        sigemptyset(&sig.sa_mask);
        sigaction(SIGCHLD,&sig,NULL);
        // 可能出现子进程已经结束，然而信号未捕捉到
        // 注册完信号捕捉以后
        sigprocmask(SIG_UNBLOCK,&set,NULL);

        while (1)
        {
            printf("parent process pid : %d\n",getpid());
            sleep(1);
        }
    }else if(pid == 0){
        // 子进程
        printf("child process pid : %d\n",getpid());
    }
    return 0;
}
```



#### 2.7.5 共享内存

- ◼ 共享内存允许两个或者多个进程共享物理内存的同一块区域（通常被称为段）。由于 一个共享内存段会称为一个进程用户空间的一部分，因此这种 IPC 机制无需内核介 入。所有需要做的就是让一个进程将数据复制进共享内存中，并且这部分数据会对其 他所有共享同一个段的进程可用。
- ◼ 与管道等要求发送进程将数据从用户空间的缓冲区复制进内核内存和接收进程将数据从内核内存复制进用户空间的缓冲区的做法相比，这种 IPC 技术的速度更快。

**使用步骤**

1. ◼ 调用 shmget() 创建一个新共享内存段或取得一个既有共享内存段的标识符（即由其 他进程创建的共享内存段）。这个调用将返回后续调用中需要用到的共享内存标识符。
2. ◼ 使用 shmat() 来附上共享内存段，即使该段成为调用进程的虚拟内存的一部分。
3. ◼ 此刻在程序中可以像对待其他可用内存那样对待这个共享内存段。为引用这块共享内存， 程序需要使用由 shmat() 调用返回的 addr 值，它是一个指向进程的虚拟地址空间 中该共享内存段的起点的指针。
4. ◼ 调用 shmdt() 来分离共享内存段。在这个调用之后，进程就无法再引用这块共享内存 了。这一步是可选的，并且在进程终止时会自动完成这一步。
5. ◼ 调用 shmctl() 来删除共享内存段。只有当当前所有附加内存段的进程都与之分离之
   后内存段才会销毁。只有一个进程需要执行这一步。

**共享内存操作函数**

```c
◼ int shmget(key_t key, size_t size, int shmflg); 
◼ void *shmat(int shmid, const void *shmaddr, int shmflg); 
◼ int shmdt(const void *shmaddr); 
◼ int shmctl(int shmid, int cmd, struct shmid_ds *buf);
◼ key_t ftok(const char *pathname, int proj_id);
```

共享内存

```c
共享内存相关的函数
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
    - 功能：创建一个新的共享内存段，或者获取一个既有的共享内存段的标识。
        新创建的内存段中的数据都会被初始化为0
    - 参数：
        - key : key_t类型是一个整形，通过这个找到或者创建一个共享内存。
                一般使用16进制表示，非0值
        - size: 共享内存的大小，按分页的大小创建
        - shmflg: 属性
            - 访问权限
            - 附加属性：创建/判断共享内存是不是存在
                - 创建：IPC_CREAT
                - 判断共享内存是否存在： IPC_EXCL , 需要和IPC_CREAT一起使用
                    IPC_CREAT | IPC_EXCL | 0664//如果内存已经存在，就会失败
        - 返回值：
            失败：-1 并设置错误号
            成功：>0 返回共享内存的引用的ID，后面操作共享内存都是通过这个值。


void *shmat(int shmid, const void *shmaddr, int shmflg);
    - 功能：和当前的进程进行关联
    - 参数：
        - shmid : 共享内存的标识（ID）,由shmget返回值获取
        - shmaddr: 申请的共享内存的起始地址，指定NULL，内核指定
        - shmflg : 对共享内存的操作
            - 读 ： SHM_RDONLY, 必须要有读权限
            - 读写： 0
    - 返回值：
        成功：返回共享内存的首（起始）地址。  失败(void *) -1


int shmdt(const void *shmaddr);
    - 功能：解除当前进程和共享内存的关联
    - 参数：
        shmaddr：共享内存的首地址
    - 返回值：成功 0， 失败 -1

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    - 功能：对共享内存进行操作。删除共享内存，共享内存要删除才会消失，创建共享内存的进程被销毁了对共享内存是没有任何影响。
    - 参数：
        - shmid: 共享内存的ID
        - cmd : 要做的操作
            - IPC_STAT : 获取共享内存的当前的状态
            - IPC_SET : 设置共享内存的状态
            - IPC_RMID: 标记共享内存被销毁
        - buf：需要设置或者获取的共享内存的属性信息
            - IPC_STAT : buf存储数据
            - IPC_SET : buf中需要初始化数据，设置到内核中
            - IPC_RMID : 没有用，NULL

key_t ftok(const char *pathname, int proj_id);
    - 功能：根据指定的路径名，和int值，生成一个共享内存的key
    - 参数：
        - pathname:指定一个存在的路径
            /home/wen/Linux/a.txt
            / 
        - proj_id: int类型的值，但是这系统调用只会使用其中的1个字节
                   范围 ： 0-255  一般指定一个字符 'a'


问题1：操作系统如何知道一块共享内存被多少个进程关联？
    - 共享内存维护了一个结构体struct shmid_ds 这个结构体中有一个成员 shm_nattch
    - shm_nattach 记录了关联的进程个数

问题2：可不可以对共享内存进行多次删除 shmctl
    - 可以的
    - 因为shmctl 标记删除共享内存，不是直接删除
    - 什么时候真正删除呢?
        当和共享内存关联的进程数为0的时候，就真正被删除
    - 当共享内存的key为0的时候，表示共享内存被标记删除了
        如果一个进程和共享内存取消关联，那么这个进程就不能继续操作这个共享内存。也不能进行关联。

    共享内存和内存映射的区别
    1.共享内存可以直接创建，内存映射需要磁盘文件（匿名映射除外）
    2.共享内存效率更高
    3.内存
        所有的进程操作的是同一块共享内存。
        内存映射，每个进程在自己的虚拟地址空间中有一个独立的内存。
    4.数据安全
        - 进程突然退出
            共享内存还存在
            内存映射区消失
        - 运行进程的电脑死机，宕机了
            数据存在在共享内存中，没有了
            内存映射区的数据 ，由于磁盘文件中的数据还在，所以内存映射区的数据还存在。

    5.生命周期
        - 内存映射区：进程退出，内存映射区销毁
        - 共享内存：进程退出，共享内存还在，标记删除（所有的关联的进程数为0），或者关机
            如果一个进程退出，会自动和共享内存进行取消关联。
```

**共享内存进程间通信**

**write**

```c
/*

*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int main(){
    // 创建共享内存
    int shmid = shmget(100,4096,IPC_CREAT|0664);
    printf("shmid : %d\n", shmid);
    // if(shmid == -1){
    //     perror("shmget");
    //     exit(0);
    // }
    // 和当前进程进行关联
    void * shmaddr = shmat(shmid,NULL,0);


    // 写数据
    char * str = "Hello World !";
    memcpy(shmaddr,str,strlen(str)+1);
    printf("按任意键继续\n");
    getchar();

    // 解除关联
    shmdt(shmaddr);

    // 删除共享内存
    // struct shmid_ds buf;
    shmctl(shmid,IPC_RMID,NULL);

    return 0;

}
```

read

```c
/*

*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // 获取共享内存
    int shmid = shmget(100,4096,IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
        exit(0);
    }
    // 和当前进程进行关联
    void * shmaddr = shmat(shmid,NULL,0);


    // 读数据
    printf("读到的数据为：%s\n",(char *)shmaddr);
    printf("按任意键继续\n");
    getchar();

    // 解除关联
    shmdt(shmaddr);

    // 删除共享内存
    // struct shmid_ds buf;
    shmctl(shmid,IPC_RMID,NULL);

    return 0;

}
```

共享内存的操作命令

![image-20210324130749252](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210324130749252.png)

### 2.8 守护进程

#### 2.8.1 终端

- ◼ 在 UNIX 系统中，用户通过终端登录系统后得到一个 shell 进程，这个终端成 为 shell 进程的控制终端（Controlling Terminal），进程中，控制终端是 保存在 PCB 中的信息，而 fork() 会复制 PCB 中的信息，因此由 shell 进程启动的其它进程的控制终端也是这个终端。
- ◼ 默认情况下（没有重定向），每个进程的标准输入、标准输出和标准错误输出都指向控制终端，进程从标准输入读也就是读用户的键盘输入，进程往标准输出或标准错误输出写也就是输出到显示器上。
- ◼ 在控制终端输入一些特殊的控制键可以给前台进程发信号，例如 Ctrl + C 会产生 SIGINT 信号，Ctrl + \ 会产生 SIGQUIT 信号。

#### 2.8.2 进程组

- ◼ 进程组和会话在进程之间形成了一种两级层次关系：进程组是一组相关进程的集合，会话是一组相关进程组的集合。进程组和会话是为支持 shell 作业控制而定义的抽 象概念，用户通过 shell 能够交互式地在前台或后台运行命令。
- ◼ 进行组由一个或多个共享同一进程组标识符（PGID）的进程组成。一个进程组拥有一 个进程组首进程，该进程是创建该组的进程，其进程 ID为该进程组的 ID，新进程 会继承其父进程所属的进程组ID。
- ◼ 进程组拥有一个生命周期，其开始时间为首进程创建组的时刻，结束时间为最后一个 成员进程退出组的时刻。一个进程可能会因为终止而退出进程组，也可能会因为加入了另外一个进程组而退出进程组。进程组首进程无需是最后一个离开进程组的成员。

#### 2.8.3 会话

- ◼ 会话是一组进程组的集合。会话首进程是创建该新会话的进程，其进程 ID 会成为会 话 ID。新进程会继承其父进程的会话 ID。
- ◼ 一个会话中的所有进程共享单个控制终端。控制终端会在会话首进程首次打开一个终端设备时被建立。一个终端最多可能会成为一个会话的控制终端。
- ◼ 在任一时刻，会话中的其中一个进程组会成为终端的前台进程组，其他进程组会成为 后台进程组。只有前台进程组中的进程才能从控制终端中读取输入。当用户在控制终 端中输入终端字符生成信号后，该信号会被发送到前台进程组中的所有成员。
- ◼ 当控制终端的连接建立起来之后，会话首进程会成为该终端的控制进程。



![image-20210324161618211](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210324161618211.png)



进程组会话操作函数

```c
◼ pid_t getpgrp(void); 
◼ pid_t getpgid(pid_t pid); 
◼ int setpgid(pid_t pid, pid_t pgid); 
◼ pid_t getsid(pid_t pid);
◼ pid_t setsid(void);
```

#### 2.8.3 守护进程

- ◼ 守护进程（Daemon Process），也就是通常说的 Daemon 进程（精灵进程），是 Linux 中的后台服务进程。它是一个生存期较长的进程，通常独立于控制终端并且周期性地执行某种任务或等待处理某些发生的事件。一般采用以 d 结尾的名字。
- ◼ 守护进程具备下列特征：
  -   生命周期很长，守护进程会在系统启动的时候被创建并一直运行直至系统被关闭。
  -   它在后台运行并且不拥有控制终端。没有控制终端确保了内核永远不会为守护进程自动生成任何控制信号以及终端相关的信号（如 SIGINT、SIGQUIT）。
- ◼ Linux 的大多数服务器就是用守护进程实现的。比如，Internet 服务器 inetd，
  Web 服务器 httpd 等。

**守护进程的创建步骤：**

```
◼ 执行一个 fork()，之后父进程退出，子进程继续执行。 
◼ 子进程调用 setsid() 开启一个新会话。 
◼ 清除进程的 umask 以确保当守护进程创建文件和目录时拥有所需的权限。 
◼ 修改进程的当前工作目录，通常会改为根目录（/）。 
◼ 关闭守护进程从其父进程继承而来的所有打开着的文件描述符。 
◼ 在关闭了文件描述符0、1、2之后，守护进程通常会打开/dev/null 并使用dup2()使所有这些描述符指向这个设备。
◼ 核心业务逻辑
```

**守护进程案例**

```c
/*
写一个守护进程，每隔两秒获取一下系统时间，并将这个时间写入磁盘文件
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void myFun(int num){
    // 捕捉到信号，获取系统时间，写入磁盘
    printf("This is my func\n");
    time_t t1 = time(NULL);
    struct tm *loc = localtime(&t1);
    char buff[1024];
    // sprintf(buff,"%d-%d-%d %d:%d:%d\n",loc->tm_year,loc->tm_mon,loc->tm_mday,loc->tm_hour,loc->tm_min,loc->tm_sec);
    // printf("%s\n",buff);
    char * str = asctime(loc);
    int fd = open("/home/wen/Linux/log.txt",O_RDWR | O_CREAT | O_APPEND,0664);
    write(fd,str,strlen(str));

    close(fd);
}
int main(){
    // 1.创建子进程，杀死父进程
    pid_t pid = fork();
    if(pid > 0){
        exit(0);
    }

    // 2.将子进程重新创建一个会话
    setsid();

    // 3.设置掩码
    umask(022);

    // 4.更改工作目录
    chdir("/");

    // 5.关闭，重定向文件描述符
    int fd = open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    // 6.业务逻辑

    // 信号捕捉
    // signal(SIGALRM,myFun);
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myFun;

    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM,&act,NULL);
    
    // 创建定时器
    struct itimerval val;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL,&val,NULL);
    while (1)
    {
        sleep(10);
    }
    
    return 0;
}
```

## 3. linux多线程开发

### 3.1 线程概述

- ◼ 与进程（process）类似，线程（thread）是允许应用程序并发执行多个任务的一种机制。**一个进程可以包含多个线程。**同一个程序中的所有线程均会独立执行相同程序，且**共享同一份全局内存区域**，其中包括初始化数据段、未初始化数据段，以及堆内存段。（传统意义上的 UNIX 进程只是多线程程序的一个特例，该进程只包含一个线程）
- ◼ **进程是 CPU 分配资源的最小单位，线程是操作系统调度执行的最小单位。**
- ◼ 线程是轻量级的进程（LWP：Light Weight Process），在 Linux 环境下线程的本质仍是进程。
- ◼ 查看指定进程的 LWP 号：ps –Lf pid

### 3.2 线程和进程的区别

- ◼ 进程间的信息难以共享。由于除去只读代码段外，父子进程并未共享内存，因此必须采用 一些进程间通信方式，在进程间进行信息交换。
- ◼ 调用 fork() 来创建进程的代价相对较高，即便利用写时复制技术，仍然需要复制诸如 内存页表和文件描述符表之类的多种进程属性，这意味着 fork() 调用在时间上的开销 依然不菲。
- ◼ 线程之间能够方便、快速地共享信息。只需将数据复制到共享（全局或堆）变量中即可。
- ◼ 创建线程比创建进程通常要快 10 倍甚至更多。线程间是共享虚拟地址空间的，无需采用写时复制来复制内存，也无需复制页表。

**线程与进程相比，不会进行写时复制整个虚拟地址空间，而会将代码段和栈空间分成每一个线程执行的小段**

### 3.3 线程之间共享资源和非共享资源

**共享资源**

 进程 ID 和父进程 ID 

 进程组 ID 和会话 ID 

 用户 ID 和 用户组 ID 

 文件描述符表 

 信号处置 

 文件系统的相关信息：文件权限掩码 （umask）、当前工作目录

 虚拟地址空间（除栈、.text）

**非共享资源**

 线程 ID 

 信号掩码 （阻塞信号集，每个线程有自己的阻塞信号集）

 线程特有数据 

 error 变量 

 实时调度策略和优先级

 栈，本地变量和函数的调用链接信息

![image-20210324223823255](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210324223823255.png)

```
◼ 当 Linux 最初开发时，在内核中并不能真正支持线程。但是它的确可以通过 clone()系统调用将进程作为可调度的实体。这个调用创建了调用进程（calling process）的一个拷贝，这个拷贝与调用进程共享相同的地址空间。LinuxThreads 项目使用这个调用 来完成在用户空间模拟对线程的支持。不幸的是，这种方法有一些缺点，尤其是在信号处 理、调度和进程间同步等方面都存在问题。另外，这个线程模型也不符合POSIX的要求。
◼ 要改进 LinuxThreads，需要内核的支持，并且重写线程库。有两个相互竞争的项目开始 来满足这些要求。一个包括 IBM 的开发人员的团队开展了 NGPT（Next-Generation POSIX Threads）项目。同时，Red Hat 的一些开发人员开展了 NPTL 项目。NGPT 在 2003 年中期被放弃了，把这个领域完全留给了NPTL。
◼ NPTL，或称为 Native POSIX Thread Library，是 Linux 线程的一个新实现，它克服LinuxThreads 的缺点，同时也符合POSIX的需求。与LinuxThreads相比它在性能和稳定性方面都提供了重大的改进。
◼ 查看当前 pthread 库版本：getconf GNU_LIBPTHREAD_VERSION
```

### 3.4 线程的操作

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
pthread_t pthread_self(void); 
int pthread_equal(pthread_t t1, pthread_t t2); 
void pthread_exit(void *retval); 
int pthread_join(pthread_t thread, void **retval); 
int pthread_detach(pthread_t thread);
int pthread_cancel(pthread_t thread);
```

**PS : gcc pthread_create.c -o pthread_create -pthread**

由于线程不是Linux系统内置的库，所以需要用-l指定库名称

**线程创建：**

```c
/*
    main线程所在的线程成为主线程（main线程），其余创建的线程叫做子线程
    程序中默认只有一个进程，调用fork()函数，子进程出现
    程序中默认只有一个线程，调用pthread()函数，子线程出现

    #include <pthread.h>
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine) (void *), void *arg);

        -功能：创建一个子线程（main函数为主线程）
        -参数：
            -*thread    传出参数，线程创建成功，子线程的线程ID会保存在这一个指针中
            -*attr      需要设置的线程的属性，一般使用默认的属性，传递null
            -start_routine 函数指针，子线程需要处理的逻辑代码
            -arg        给第三个参数使用，向函数传递参数
        -返回值：
            成功 返回0
            失败 返回错误号。这个错误号与之前的errno不一样
                不能使用perror()进行错误的输出
                获取错误号信息： char * strerror(int errnum);
*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * MyFunc(void * arg){
    printf("child pthread now ...\n");
    printf("arg val : %d\n",*(int *)arg);

    return NULL;
}
int main(){
    pthread_t tid;
    int num = 10;
    int re = pthread_create(&tid,NULL,MyFunc,(void *)&num);
    if(re != 0){
        char * str = strerror(re);
        printf("errorno : %s\n",str);
        exit(0);
    }
    for(int i = 0;i < 5;i++){
        printf("i = %d \n",i);
    }

    sleep(1);

    return 0;
}
```

**退出线程：**

```c
/*
    #include <pthread.h>
    void pthread_exit(void *retval);
    -功能：终止一个线程，在哪一个线程中调用，终止哪个线程
    -参数：
        retval:需要传递一个指针，作为一个返回值，可以在pthread_join()中可以
    pthread_t pthread_self(void)
        -功能：
            获取当前线程的线程id

    int pthread_equal(pthread_t t1, pthread_t t2);
        -功能：
            判断两个线程ID是否相等
        不同的操作系统，pthread_t在不同的操作系统实现不一样，有的有符号长整型，有的是通过结构体去实现
        
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void * MyFunc(void * arg){
    printf("child thread id : %ld\n",pthread_self());

    return NULL;
    // pthread_exit(NULL);
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,MyFunc,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("%s\n",errstr);
        exit(0);
    }
    for(int i = 0;i < 100;i++){
        printf("i = %d\n",i);
    }

    printf("tid : %ld, main thread id : %ld\n",tid ,pthread_self());

    pthread_exit(NULL);
    // 主线程退出，对其他线程没有影响

    printf("pthread exit \n");
    return 0;
}
```

**连接已经终止的子线程**

```c
/*
    #include <pthread.h>
    int pthread_join(pthread_t thread, void **retval);
    -功能：
        和一个已经终止的线程进行连接，就是回收子线程的资源
            这个函数是阻塞函数
            这个函数调用一次只能回收一个子进程
            一般在子线程中使用
    -参数
        -thread :需要回收的子线程id
        -retval ：接受子线程结束时的返回值
    -返回值
        -0 表示成功
        -非零 设置错误号
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int value = 10;
void * MyFunc(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    // sleep(3);
    // int value = 0;
    // 线程的栈空间的数据会在线程结束以后清空
    pthread_exit((void *)(&value));
    // return NULL;
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,MyFunc,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("%s\n",errstr);
        exit(0);
    }
    for(int i = 0;i < 100;i++){
        printf("i = %d\n",i);
    }

    printf("tid : %ld, main thread id : %ld\n",tid ,pthread_self());

    // 主线程调用pthread_join()回收子线程的资源
    int * thread_retval = NULL;
    int ret = pthread_join(tid, (void **)&thread_retval);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error : %s\n",errstr);
    }
    printf("I receive value : %d\n",*thread_retval);
    printf("回收子线程资源成功\n");
    pthread_exit(NULL);
    // 主线程退出，对其他线程没有影响

    
    return 0;
}

// 使用二级指针的目的
int a = 20;
void fun(int a){
    a = 10;
}
// 如果要完成对a的值进行修改,则必须使用指针
void fun(int * a){
    *a = 20;
}
//如果当int传入的为int * a,如果他们想要修改a的值就必须使用*a的指针,也就是二级指针
```

**线程的分离**

```c
/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);
    -功能：分离线程，被分离的线程在终止的时候，会自动释放资源返回给系统
        1.不能多次分离，会产生不可预期的错误
        2.不能去链接一个已经分离的线程,会报错
    -参数：
        需要分离的线程的ID
    -返回值：
        成功： 返回0
        失败： 返回错误号

*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void * callback(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    return NULL;
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }

    printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());

    // 设置子线程分离
    pthread_detach(tid);

    // 对分离的子线程进行连接
    // int err = pthread_join(tid,NULL);
    // if(err != 0){
    //     char * str = strerror(err);
    //     printf("join err : %s\n",str);
    //     exit(0);
    // }
    pthread_exit(NULL);
    return 0;
}
```

**线程取消**

```c
/*
    #include <pthread.h>
    int pthread_cancel(pthread_t thread);

    -功能：取消线程（让线程终止）
        取消某个线程，可以终止某个线程的运行，而是当某个线程执行到某个取消点，线程才会被终止。
        取消点：系统规定好的一些系统调用，我们可以粗略的理解为系统从用户区到内核区的系统调用
*/

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void * callback(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    for(int i = 0;i < 5 ; i++){
        printf("child i = %d\n",i);
    }
    return NULL;
}
int main(){
    pthread_t tid;
    int re = pthread_create(&tid,NULL,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }

    // printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());
    pthread_cancel(tid);

    for(int i = 0;i < 5 ; i++){
        printf("i = %d\n",i);
    }


    pthread_exit(NULL);
    return 0;
}
```

### 3.5 线程的属性

```c
◼ 线程属性类型 pthread_attr_t 
◼ int pthread_attr_init(pthread_attr_t *attr); 
◼ int pthread_attr_destroy(pthread_attr_t *attr); 
◼ int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
◼ int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```

**属性操作**

```c
/*
    int pthread_attr_init(pthread_attr_t *attr); 
        -初始化线程属性变量

    int pthread_attr_destroy(pthread_attr_t *attr); 
        -释放线程属性的资源

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
        -获取线程分离的状态属性

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
        -设置线程分离模型的状态属性

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void * callback(void * arg){
    printf("child thread id : %ld\n",pthread_self());
    return NULL;
}
int main(){
    // 创建一个线程属性变量
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    // 获取线程的栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr,&size);
    

    pthread_t tid;

    int re = pthread_create(&tid,&attr,callback,NULL);
    if(re != 0){
        char * errstr = strerror(re);
        printf("err: %s",errstr);
        exit(0);
    }
    printf("stack size : %ld\n",size);

    pthread_attr_destroy(&attr);

    printf("tid : %ld , main thread tid : %ld\n",tid,pthread_self());

    // 设置子线程分离
    //pthread_detach(tid);


    pthread_exit(NULL);
    return 0;
}
```

### 3.6 线程同步

**sellticket**

```c
/*
    使用多线程实现买票的案例
    三个窗口，一共一百张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticketd = 100;
void * Myfunc(void * arg){
    
    while(ticketd > 0){
        printf("%ld 正在卖第 %d 张票\n",pthread_self(),ticketd);
        usleep(6000);
        ticketd--;
    }
    return NULL;
}
int main(){
    // 创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid1,NULL,Myfunc,NULL);

    // 回收子线程的资源，阻塞
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    // 设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);
    
    return 0;
}
```

◼ 线程的主要优势在于，能够通过全局变量来共享信息。不过，这种便捷的共享是有代价 的：必须确保多个线程不会同时修改同一变量，或者某一线程不会读取正在由其他线程修改的变量。
◼ 临界区是指访问某一共享资源的代码片段，并且这段代码的执行应为**原子操作**，也就是同时访问同一共享资源的其他线程不应终端该片段的执行。
◼ 线程同步：即当有一个线程在对内存进行操作时，其他线程都不可以对这个内存地址进行操作，直到该线程完成操作，其他线程才能对该内存地址进行操作，而其他线程则处于等待状态。

#### 3.6.1 **互斥量(互斥锁)**

◼ 为避免线程更新共享变量时出现问题，可以使用互斥量（mutex 是 mutual exclusion 的缩写）来确保同时仅有一个线程可以访问某项共享资源。可以使用互斥量来保证对任意共 享资源的原子访问。

◼ 互斥量有两种状态：已锁定（locked）和未锁定（unlocked）。任何时候，至多只有一 个线程可以锁定该互斥量。试图对已经锁定的某一互斥量再次加锁，将可能阻塞线程或者报 错失败，具体取决于加锁时使用的方法。

◼ 一旦线程锁定互斥量，随即成为该互斥量的所有者，只有所有者才能给互斥量解锁。一般情 况下，对每一共享资源（可能由多个相关变量组成）会使用不同的互斥量，每一线程在访问 同一资源时将采用如下协议： 

​		⚫ 针对共享资源锁定互斥量 

​		⚫ 访问共享资源

​		⚫ 对互斥量解锁

◼ 如果多个线程试图执行这一块代码（一个临界区），事实上只有一个线程能够持有该互斥量（其他线程将遭到阻塞），即同时只有一个线程能够进入这段代码区域，如下图所示：

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210326164313071.png" alt="image-20210326164313071" style="zoom:50%;" />

**互斥量的操作函数**

```c
◼ 互斥量的类型 pthread_mutex_t 
◼ int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
◼ int pthread_mutex_destroy(pthread_mutex_t *mutex); 
◼ int pthread_mutex_lock(pthread_mutex_t *mutex); 
◼ int pthread_mutex_trylock(pthread_mutex_t *mutex);
◼ int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

**互斥量的操作**

```c
/*
    互斥量的类型 pthread_mutex_t 
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
        -初始化互斥量
        -参数：
            -mutex：需要初始化的互斥量变量、
            -attr： 互斥量相关的属性，null
        -restrict：C语言的修饰符，被修饰的指针，不能由另外的指针进行操作。
            pthread_mutex_t *restrict mutex = xxx;
            pthread_mutex_t *restrict mutex1 =  mutex;
            不能使用mutex1来操作mutex

    int pthread_mutex_destroy(pthread_mutex_t *mutex); 
        -释放互斥锁资源

    int pthread_mutex_lock(pthread_mutex_t *mutex); 
        -加锁，阻塞的，如果有一个线程加锁，其他线程只能阻塞等待

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        -尝试上锁，如果枷锁失败，不会阻塞，直接返回

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        -解锁
*/  
/*
    使用多线程实现买票的案例
    三个窗口，一共一百张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticketd = 1000;

// 创建线程锁
pthread_mutex_t mutex;
void * Myfunc(void * arg){
    // 临界区
    while(1){
        // 加锁
        pthread_mutex_lock(&mutex);
        if(ticketd > 0){
            printf("%ld 正在卖第 %d 张票\n",pthread_self(),ticketd);
            usleep(6000);
            ticketd--;
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    // 解锁
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_mutex_init(&mutex,NULL);

    // 创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid2,NULL,Myfunc,NULL);
    pthread_create(&tid3,NULL,Myfunc,NULL);

    // 回收子线程的资源，阻塞
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    // 设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

**死锁**

◼ 有时，一个线程需要同时访问两个或更多不同的共享资源，而每个资源又都由不同的互斥量管理。当超过一个线程加锁同一组互斥量时，就有可能发生死锁。

◼ 两个或两个以上的进程在执行过程中，因争夺共享资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁。

◼ 死锁的几种场景： 

​	 忘记释放锁 

​	 重复加锁

​	 多线程多锁，抢占锁资源

![image-20210326172507988](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210326172507988.png)

**deadlock1**

```c
/*
    互斥量的类型 pthread_mutex_t 
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
        -初始化互斥量
        -参数：
            -mutex：需要初始化的互斥量变量、
            -attr： 互斥量相关的属性，null
        -restrict：C语言的修饰符，被修饰的指针，不能由另外的指针进行操作。
            pthread_mutex_t *restrict mutex = xxx;
            pthread_mutex_t *restrict mutex1 =  mutex;
            不能使用mutex1来操作mutex

    int pthread_mutex_destroy(pthread_mutex_t *mutex); 
        -释放互斥锁资源

    int pthread_mutex_lock(pthread_mutex_t *mutex); 
        -加锁，阻塞的，如果有一个线程加锁，其他线程只能阻塞等待

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        -尝试上锁，如果枷锁失败，不会阻塞，直接返回

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        -解锁
*/  
/*
    使用多线程实现买票的案例
    三个窗口，一共一百张票
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int ticketd = 1000;

// 创建线程锁
pthread_mutex_t mutex;
void * Myfunc(void * arg){
    // 临界区
    while(1){
        // 加锁
        pthread_mutex_lock(&mutex);
        // pthread_mutex_trylock(&mutex);
        // trylock尝试上锁，上锁失败就不阻塞

        // pthread_mutex_lock(&mutex);
        // pthread_mutex_lock(&mutex);
        // 重复加锁


        if(ticketd > 0){
            printf("%ld 正在卖第 %d 张票\n",pthread_self(),ticketd);
            usleep(6000);
            ticketd--;
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    // 解锁
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_mutex_init(&mutex,NULL);

    // 创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,Myfunc,NULL);
    pthread_create(&tid2,NULL,Myfunc,NULL);
    pthread_create(&tid3,NULL,Myfunc,NULL);

    // 回收子线程的资源，阻塞
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    // 设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

**deadlock2**

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// 创建两个互斥量
pthread_mutex_t mutex1,mutex2;

void * WorkA(void * arg){
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);

    printf("WorkA .....\n");

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
void * WorkB(void * arg){
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);

    printf("WorkB .....\n");

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

}
int main(){
    // 初始化互斥量
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    // 创建两个子线程
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,WorkA,NULL);
    pthread_create(&tid2,NULL,WorkB,NULL);

    // 回收子线程资源
    pthread_join(tid1,NULL);
    pthread_join(tid1,NULL);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}
```

#### 3.6.2 读写锁

◼ 当有一个线程已经持有互斥锁时，互斥锁将所有试图进入临界区的线程都阻塞住。但是考 虑一种情形，当前持有互斥锁的线程只是要读访问共享资源，而同时有其它几个线程也想 读取这个共享资源，但是由于互斥锁的排它性，所有其它线程都无法获取锁，也就无法读 访问共享资源了，但是实际上多个线程同时读访问共享资源并不会导致问题。

◼ 在对数据的读写操作中，更多的是读操作，写操作较少，例如对数据库数据的读写应用。 为了满足当前能够允许多个读出，但只允许一个写入的需求，线程提供了读写锁来实现。

◼ 读写锁的特点： 

​		 如果有其它线程读数据，则允许其它线程执行读操作，但不允许写操作。 

​		 如果有其它线程写数据，则其它线程都不允许读、写操作。

​		 写是独占的，写的优先级高。

**读写锁相关操作函数**

```c
◼ 读写锁的类型 pthread_rwlock_t 
◼ int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
◼ int pthread_rwlock_destroy(pthread_rwlock_t *rwlock); 
◼ int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); 
◼ int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 
◼ int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock); 
◼ int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

**读写锁的案例**

```c
/*
    读写锁的类型 pthread_rwlock_t 
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock); 
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int num = 10;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void * ThreadRead(void * arg){
    while(1){
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);
        printf("===read tid: %ld, num = %d\n",pthread_self(),num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}
void * ThreadWrite(void * arg){
    while (1)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("num++ tid: %ld, num = %d\n",pthread_self(),num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL; 
    
}
int main(){
    // pthread_mutex_init(&mutex,NULL);
    pthread_rwlock_init(&rwlock,NULL);
    pthread_t w_tid[3],r_tid[5];
    for(int i = 0;i< 5 ;i++){
        pthread_create(&r_tid[i],NULL,ThreadRead,NULL);
    }

    for(int i = 0;i< 3 ;i++){
        pthread_create(&w_tid[i],NULL,ThreadWrite,NULL);
    }

    for(int i = 0;i< 5 ;i++){
        pthread_detach(r_tid[i]);
    }

    for(int i = 0;i< 3 ;i++){
        pthread_detach(w_tid[i]);
    }
    
    pthread_exit(NULL);
    // pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
```

### 3.7 生产者消费者模型

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210327123138047.png" alt="image-20210327123138047" style="zoom:50%;" />

**代码实现**

```c
/*
    生产者消费者模型
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct Node
{
    int num;
    struct Node * next;
    
};
struct Node * head = NULL; 
pthread_mutex_t mutex;

void * Consume(void * arg){
    // 保存头节点的指针
    while(1){
       pthread_mutex_lock(&mutex);
       struct Node * temp =  head;
       if(head != NULL){
            //有数据
            head = head->next;
            printf("del node, num : %d, tid : %ld\n",temp->num,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
       }else{
            pthread_mutex_unlock(&mutex);
       }
       

    }
    return NULL;
}

void * Product(void * arg){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand()%1000 + 1;
        printf("add Mode, num : %d, tid : %ld\n",newNode->num,pthread_self());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
    // 创建5个生产者线程和5个消费者线程
    pthread_t produ[5],consu[5];
    for(int i = 0;i < 5; i++){
        pthread_create(&produ[i],NULL,Product,NULL);
        pthread_create(&consu[i],NULL,Consume,NULL);

    }
    
    for(int i = 0;i < 5;i++){
        pthread_detach(produ[i]);
        pthread_detach(consu[i]);
    }

    while (1)
    {
        sleep(10);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    
    return 0;
}
```

#### 3.7.1**条件变量**

```c
◼ 条件变量的类型 pthread_cond_t 
◼ int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
◼ int pthread_cond_destroy(pthread_cond_t *cond);
◼ int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	//等待产生可以消费的数据
◼ int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
	//等待多久的时间
◼ int pthread_cond_signal(pthread_cond_t *cond);
	//唤醒消费者一个或者多个
◼ int pthread_cond_broadcast(pthread_cond_t *cond);
	//唤醒所有的消费者
```

**条件变量的使用**

```c
/*

    条件变量的类型 pthread_cond_t 
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);

    int pthread_cond_destroy(pthread_cond_t *cond);

    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	    //等待产生可以消费的数据
        阻塞函数，等待唤醒
    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
	    //等待多久的时间
        调用这个函数，线程会阻塞，知道等待时间结束
    int pthread_cond_signal(pthread_cond_t *cond);
	    //唤醒消费者一个或者多个
        唤醒一个或者多个等待的线程
    int pthread_cond_broadcast(pthread_cond_t *cond);
	    //唤醒所有的消费者
        唤醒所有的等待的线程

*/
/*
    生产者消费者模型
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
struct Node
{
    int num;
    struct Node * next;
    
};
struct Node * head = NULL; 
// 创建互斥变量
pthread_mutex_t mutex;

// 创建条件变量
pthread_cond_t cond; 

void * Consume(void * arg){
    // 保存头节点的指针
    while(1){
       pthread_mutex_lock(&mutex);
       struct Node * temp =  head;
       if(head != NULL){
            //有数据
            head = head->next;
            printf("del node, num : %d, tid : %ld\n",temp->num,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
       }else{
            // 没有数据需要等待
            pthread_cond_wait(&cond,&mutex);
            // 阻塞进入拿到锁以后，阻塞等待器件，就会解锁，其他线程就可以抢占
            // 如果阻塞结束被唤醒，向下执行就会加锁。
            pthread_mutex_unlock(&mutex);
       }
       
    }
    return NULL;
}

void * Product(void * arg){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand()%1000 + 1;
        printf("add Mode, num : %d, tid : %ld\n",newNode->num,pthread_self());
        
        // 只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    // 创建5个生产者线程和5个消费者线程
    pthread_t produ[5],consu[5];
    for(int i = 0;i < 5; i++){
        pthread_create(&produ[i],NULL,Product,NULL);
        pthread_create(&consu[i],NULL,Consume,NULL);

    }
    
    for(int i = 0;i < 5;i++){
        pthread_detach(produ[i]);
        pthread_detach(consu[i]);
    }

    while (1)
    {
        sleep(10);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    
    return 0;
}
```

#### 3.7.2 信号量

```c
◼ 信号量的类型 sem_t 
◼ int sem_init(sem_t *sem, int pshared, unsigned int value); 
◼ int sem_destroy(sem_t *sem); 
◼ int sem_wait(sem_t *sem); 
◼ int sem_trywait(sem_t *sem); 
◼ int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout); ◼ int sem_post(sem_t *sem);
◼ int sem_getvalue(sem_t *sem, int *sval);
```

```c
/*
    信号量的类型 sem_t 
    int sem_init(sem_t *sem, int pshared, unsigned int value); 
        -初始化信号量
        -参数
            -sem ：信号变量的地址
            -pshared ：
                0 用在线程间
                非零 用在进程间
            -value ：
                信号量中的值

    int sem_destroy(sem_t *sem); 
        -释放资源
    int sem_wait(sem_t *sem); 
        -对信号量加锁，调用一次，对信号量的值减一，如果为0，就阻塞
        原来在信号量里边的值存在，每当调用一次就减一，直到信号量为零就阻塞
    int sem_trywait(sem_t *sem); 
        - 
    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
        -等待多久的时间
    int sem_post(sem_t *sem);、
        -对信号量解锁，调用一次，对信号量的值加一
    int sem_getvalue(sem_t *sem, int *sval);

    sem_t psem;
    sem_t csem;
    init(psem, 0, 8);
    init(csem, 0, 0);

    producer() {
        sem_wait(&psem);
        sem_post(&csem)
    }

    customer() {
        sem_wait(&csem);
        sem_post(&psem)
    }
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
struct Node
{
    int num;
    struct Node * next;
    
};
struct Node * head = NULL; 
// 创建互斥变量
pthread_mutex_t mutex;

// 创建条件变量
pthread_cond_t cond; 

// 创建信号量
sem_t sempro,semcon;

void * Consume(void * arg){
    // 保存头节点的指针
    while(1){

        sem_wait(&semcon);
        pthread_mutex_lock(&mutex);
        struct Node * temp =  head;
        //有数据
        head = head->next;
        printf("del node, num : %d, tid : %ld\n",temp->num,pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        sem_post(&sempro);
        usleep(100);

       
    }
    return NULL;
}

void * Product(void * arg){
    while(1){
        sem_wait(&sempro);
        // 调用一次减小1，当为零的时候就阻塞
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand()%1000 + 1;
        printf("add Mode, num : %d, tid : %ld\n",newNode->num,pthread_self());
        
        // 只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        sem_post(&semcon);
        // 表示消费者可以消费多少个
        usleep(100);
    }
    return NULL;
}
int main(){
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    sem_init(&sempro,0,10);
    sem_init(&semcon,0,0);
    // 创建5个生产者线程和5个消费者线程
    pthread_t produ[5],consu[5];
    for(int i = 0;i < 5; i++){
        pthread_create(&produ[i],NULL,Product,NULL);
        pthread_create(&consu[i],NULL,Consume,NULL);

    }
    
    for(int i = 0;i < 5;i++){
        pthread_detach(produ[i]);
        pthread_detach(consu[i]);
    }

    while (1)
    {
        sleep(10);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    sem_destroy(&sempro);
    sem_destroy(&semcon);
    pthread_exit(NULL);
    
    return 0;
}
```

## 4. Linux网络编程

### 4.1 网络结构模式

#### 4.1.1 C/S结构

**简介**

> 服务器 - 客户机，即 Client - Server（C/S）结构。C/S 结构通常采取两层结构。服务器负责数据的管理，客户机负责完成与用户的交互任务。客户机是因特网上访问别人信息的机器，服务器则是提供信息供人访问的计算机。
>
> 客户机通过局域网与服务器相连，接受用户的请求，并通过网络向服务器提出请求，对数据库进行操作。服务器接受客户机的请求，将数据提交给客户机，客户机将数据进行计算并将结果呈现给用户。服务器还要提供完善安全保护及对数据完整性的处理等操作，并允许多个客户机同时访问服务器，这就对服务器的硬件处理数据能力提出了很高的要求。
>
> 在C/S结构中，应用程序分为两部分：服务器部分和客户机部分。服务器部分是多个用户共享的信息与功能，执行后台服务，如控制共享数据库的操作等；客户机部分为用户所专有，负责执行前台功能，在出错提示、在线帮助等方面都有强大的功能，并且可以在子程序间自由切换。

**优点**

1. 能充分发挥客户端 PC 的处理能力，很多工作可以在客户端处理后再提交给服务器，所以 C/S 结构客户端响应速度快；
2. 操作界面漂亮、形式多样，可以充分满足客户自身的个性化要求； 
3. C/S 结构的管理信息系统具有较强的事务处理能力，能实现复杂的业务流程； 
4. 安全性较高，C/S 一般面向相对固定的用户群，程序更加注重流程，它可以对权限进行多层次校 验，提供了更安全的存取模式，对信息安全的控制能力很强，一般高度机密的信息系统采用 C/S 结构适

**缺点**

1. 客户端需要安装专用的客户端软件。首先涉及到安装的工作量，其次任何一台电脑出问题，如病 毒、硬件损坏，都需要进行安装或维护。系统软件升级时，每一台客户机需要重新安装，其维护和升级成本非常高；
2. 对客户端的操作系统一般也会有限制，不能够跨平台。

#### 4.1.2 B/S结构

**简介**

> B/S 结构（Browser/Server，浏览器/服务器模式），是 WEB 兴起后的一种网络结构模式，WEB 浏览器是客户端最主要的应用软件。这种模式统一了客户端，将系统功能实现的核心部分集中到服务器上，简化了系统的开发、维护和使用。客户机上只要安装一个浏览器，如 Firefox 或 Internet Explorer，服务器安装 SQL Server、Oracle、MySQL 等数据库。浏览器通过 Web Server 同数据库进行数据交互。

**优点**

B/S 架构最大的优点是总体拥有成本低、维护方便、 分布性强、开发简单，可以不用安装任何专门的软件就能实现在任何地方进行操作，客户端零维护，系统的扩展非常容易，只要有一台能上网的电脑就能使用。

**缺点**

1. 通信开销大、系统和数据的安全性较难保障; 
2. 个性特点明显降低，无法实现具有个性化的功能要求； 
3. 协议一般是固定的：http/https
4. 客户端服务器端的交互是请求-响应模式，通常动态刷新页面，响应速度明显降低。

### 4.2 MAC地址

> 网卡是一块被设计用来允许计算机在计算机网络上进行通讯的计算机硬件，又称为网络适配器或网络接口卡NIC。其拥有 MAC 地址，属于 OSI 模型的第 2 层，它使得用户可以通过电缆或无线相互连接。每一个网卡都有一个被称为 MAC 地址的独一无二的 48 位串行号。网卡的主要功能：1.数据的封装与解封装、2.链路管理、3.数据编码与译码。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210328134237210.png" alt="image-20210328134237210" style="zoom:50%;" />

> MAC 地址（Media Access Control Address），直译为媒体存取控制位址，也称为**局域网地址、以太网地址、物理地址或硬件地址**，它是一个用来确认网络设备位置的位址，由网络设备制造商生产时烧录在网卡中。在 OSI 模型中，第三层网络层负责 IP 地址，第二层数据链路层则负责 MAC 位址 。MAC地址用于在网络中唯一标识一个网卡，一台设备若有一或多个网卡，则每个网卡都需要并会有一个唯一的 MAC 地址。
>
> MAC 地址的长度为 48 位（6个字节），通常表示为 12 个 16 进制数，如：00-16-EA-AE-3C-40 就是一个MAC 地址，其中前 3 个字节，16进制数 00-16-EA 代表网络硬件制造商的编号，它由 IEEE（电气与电子工程师协会）分配，而后 3 个字节，16进制数 AE-3C-40 代表该制造商所制造的某个网络产品（如网卡）的系列号。只要不更改自己的 MAC 地址，MAC 地址在世界是唯一的。
> 形象地说，MAC 地址就如同身份证上的身份证号码，具有唯一性。

### 4.3 IP地址

**简介**

> IP 协议是为计算机网络相互连接进行通信而设计的协议。在因特网中，它是能使连接到网上的所有计算机网络实现相互通信的一套规则，规定了计算机在因特网上进行通信时应当遵守的规则。任何厂家生产的计算机系统，只要遵守 IP 协议就可以与因特网互连互通。各个厂家生产的网络系统和设备，如以太网、分组交换网等，它们相互之间不能互通，不能互通的主要原因是因为它们所传送数据的基本单元（技术上称之为“帧”）的格式不同。IP 协议实际上是一套由软件程序组成的协议软件，它把各种不同“帧”统一转换成“IP 数据报”格式，这种转换是因特网的一个最重要的特点，使所有各种计算机都能在因特网上实现互通，即具有“开放性”的特点。正是因为有了 IP 协议，因特网才得以迅速发展成为世界上最大的、开放的计算机通信网络。因此，IP 协议也可以叫做“因特网协议”。

> IP 地址（Internet Protocol Address）是指互联网协议地址，又译为网际协议地址。IP 地址是 IP 协议提供的一种统一的地址格式，它为互联网上的每一个网络和每一台主机分配一个逻辑地址，以 此来屏蔽物理地址的差异。 
>
> IP 地址是一个 32 位的二进制数，通常被分割为 4 个“ 8 位二进制数”（也就是 4 个字节）。IP 地址通常用“点分十进制”表示成（a.b.c.d）的形式，其中，a,b,c,d都是 0~255 之间的十进制整数。例：点分十进IP地址（100.4.5.6），实际上是 32 位二进制数
> （01100100.00000100.00000101.00000110）。

**IP地址的编制方式**

最初设计互联网络时，为了便于寻址以及层次化构造网络，每个 IP 地址包括两个标识码（ID），即网络ID 和主机 ID。同一个物理网络上的所有主机都使用同一个网络 ID，网络上的一个主机（包括网络上工作站，服务器和路由器等）有一个主机 ID 与其对应。Internet 委员会定义了 5 种 IP 地址类型以适合不同容量的网络，即A类~E类。

其中 A、B、C 3类（如下表格）由 InternetNIC 在全球范围内统一分配，D、E 类为特殊地址。

| 类别 |  最大网络数   |        IP地址范围         | 单个网段最大主机数 |       私有IP地址范围        |
| :--: | :-----------: | :-----------------------: | :----------------: | :-------------------------: |
|  A   |  126(2^7-2)   |  1.0.0.1-126.255.255.254  |      16777214      | 10.0.0.0 -   10.255.255.255 |
|  B   |  16384(2^14)  | 128.0.0.1-191.255.255.254 |       65534        |  172.16.0.0-172.31.255.255  |
|  C   | 2097152(2^21) | 192.0.0.1-223.255.255.254 |        254         | 192.168.0.0-192.168.255.255 |

----

**A类IP地址**

一个 A 类 IP 地址是指， 在 IP 地址的四段号码中，第一段号码为网络号码，剩下的三段号码为本地计算机的号码。如果用二进制表示 IP 地址的话，A 类 IP 地址就由 1 字节的网络地址和 3 字节主机地址组 成，网络地址的最高位必须是“0”。A 类 IP 地址中网络的标识长度为 8 位，主机标识的长度为 24 位，A 类网络地址数量较少，有 126 个网络，每个网络可以容纳主机数达 1600 多万台。

A 类 IP 地址 地址范围 1.0.0.1 - 126.255.255.254（二进制表示为：00000001 00000000 00000000 00000001 - 01111111 11111111 11111111 11111110）。最后一个是广播地址。

A 类 IP 地址的子网掩码为 255.0.0.0，每个网络支持的最大主机数为 256 的 3 次方 - 2 = 16777214 台。

**B类IP地址**

一个 B 类 IP 地址是指，在 IP 地址的四段号码中，前两段号码为网络号码。如果用二进制表示 IP 地址的 话，B 类 IP 地址就由 2 字节的网络地址和 2 字节主机地址组成，网络地址的最高位必须是“10”。B 类 IP 地址中网络的标识长度为 16 位，主机标识的长度为 16 位，B 类网络地址适用于中等规模的网络，有 16384 个网络，每个网络所能容纳的计算机数为 6 万多台。 B 类 IP 地址地址范围 128.0.0.1 - 191.255.255.254 （二进制表示为：10000000 00000000 00000000 00000001 - 10111111 11111111 11111111 11111110）。 最后一个是广播地址。

B 类 IP 地址的子网掩码为 255.255.0.0，每个网络支持的最大主机数为 256 的 2 次方 - 2 = 65534 台。

**C类IP地址**

一个 C 类 IP 地址是指，在 IP 地址的四段号码中，前三段号码为网络号码，剩下的一段号码为本地计算 机的号码。如果用二进制表示 IP 地址的话，C 类 IP 地址就由 3 字节的网络地址和 1 字节主机地址组 成，网络地址的最高位必须是“110”。C 类 IP 地址中网络的标识长度为 24 位，主机标识的长度为 8 位， C 类网络地址数量较多，有 209 万余个网络。适用于小规模的局域网络，每个网络最多只能包含254台 计算机。 C 类 IP 地址范围 192.0.0.1-223.255.255.254 （二进制表示为: 11000000 00000000 00000000 00000001 - 11011111 11111111 11111111 11111110）。

C类IP地址的子网掩码为 255.255.255.0，每个网络支持的最大主机数为 256 - 2 = 254 台。

**D类IP地址**

D 类 IP 地址在历史上被叫做多播地址（multicast address），即组播地址。在以太网中，多播地址命 名了一组应该在这个网络中应用接收到一个分组的站点。多播地址的最高位必须是 “1110”，范围从
224.0.0.0 - 239.255.255.255。

**特殊的网址**

每一个字节都为 0 的地址（ “0.0.0.0” ）对应于当前主机；

IP 地址中的每一个字节都为 1 的 IP 地址（ “255.255.255.255” ）是当前子网的广播地址； 

IP 地址中凡是以 “11110” 开头的 E 类 IP 地址都保留用于将来和实验使用。

IP地址中不能以十进制 “127” 作为开头，该类地址中数字 127.0.0.1 到 127.255.255.255 用于回路测
试，如：127.0.0.1可以代表本机IP地址。

**子网掩码**

> 子网掩码（subnet mask）又叫网络掩码、地址掩码、子网络遮罩，它是一种用来指明一个IP 地址的哪些位标识的是主机所在的子网，以及哪些位标识的是主机的位掩码。子网掩码不能单独存 在，它必须结合 IP 地址一起使用。子网掩码只有一个作用，**就是将某个 IP 地址划分成网络地址和主机地址两部分。**
>
> 子网掩码是一个 32 位地址，用于屏蔽 IP 地址的一部分以区别网络标识和主机标识，并说明该 IP
> 地址是在局域网上，还是在广域网上。

子网掩码是在 IPv4 地址资源紧缺的背景下为了解决 lP 地址分配而产生的虚拟 lP 技术，通过子网掩码将A、B、C 三类地址划分为若干子网，从而显著提高了 IP 地址的分配效率，有效解决了 IP 地址资源紧张的局面。另一方面，在企业内网中为了更好地管理网络，网管人员也利用子网掩码的作用，人为地将一 个较大的企业内部网络划分为更多个小规模的子网，再利用三层交换机的路由功能实现子网互联，从而有效解决了网络广播风暴和网络病毒等诸多网络管理方面的问题。

在大多数的网络教科书中，一般都将子网掩码的作用描述为通过逻辑运算，将 IP 地址划分为网络标识 (Net.ID) 和主机标识(Host.ID)，只有网络标识相同的两台主机在无路由的情况下才能相互通信。

根据 RFC950 定义，子网掩码是一个 32 位的 2 进制数， 其对应网络地址的所有位都置为 1，对应于主机地址的所有位置都为 0。子网掩码告知路由器，地址的哪一部分是网络地址，哪一部分是主机地址，使路由器正确判断任意 IP 地址是否是本网段的，从而正确地进行路由。网络上，数据从一个地方传到另外一个地方，是依靠 IP 寻址。从逻辑上来讲，是两步的。第一步，从 IP 中找到所属的网络，好比是去找这个人是哪个小区的；第二步，再从 IP 中找到主机在这个网络中的位置，好比是在小区里面找到这个人。

子网掩码的设定必须遵循一定的规则。与二进制 IP 地址相同，子网掩码由 1 和 0 组成，且 1 和 0 分别连续。子网掩码的长度也是 32 位，左边是网络位，用二进制数字 “1” 表示，1 的数目等于网络位的长度；右边是主机位，用二进制数字 “0” 表示，0 的数目等于主机位的长度。这样做的目的是为了让掩码与 IP 地址做按位与运算时用 0 遮住原主机数，而不改变原网络段数字，而且很容易通过 0 的位数确定子网的主机数（ 2 的主机位数次方 - 2，因为主机号全为 1 时表示该网络广播地址，全为 0 时表示该网络的网络号，这是两个特殊地址）。通过子网掩码，才能表明一台主机所在的子网与其他子网的关系，使网络正常工作。

### 4.4 **端口**

端口号是用来标记进程的唯一的编号

> “端口” 是英文 port 的意译，可以认为是设备与外界通讯交流的出口。端口可分为虚拟端口和物理端口，其中虚拟端口指计算机内部或交换机路由器内的端口，不可见，是特指TCP/IP协议中的端口，是逻辑意义上的端口。例如计算机中的 80 端口、21 端口、23 端口等。物理端口又称为接口，是可见端口，计算机背板的 RJ45 网口，交换机路由器集线器等 RJ45 端口。电话使用 RJ11 插口也属于物理端口的范畴。
>
> 如果把 IP 地址比作一间房子，端口就是出入这间房子的门。真正的房子只有几个门，但是一个 IP 地址的端口可以有65536（即：2^16）个之多！端口是通过端口号来标记的，端口号只有整数，
>
> 范围是从 0 到65535（2^16-1）。

**端口类型**

1.周知端口（Well Known Ports）

周知端口是众所周知的端口号，也叫知名端口、公认端口或者常用端口，范围从 0 到 1023，它们紧密 绑定于一些特定的服务。例如 80 端口分配给 WWW 服务，21 端口分配给 FTP 服务，23 端口分配给 Telnet服务等等。我们在 IE 的地址栏里输入一个网址的时候是不必指定端口号的，因为在默认情况下 WWW 服务的端口是 “80”。网络服务是可以使用其他端口号的，如果不是默认的端口号则应该在地址栏 上指定端口号，方法是在地址后面加上冒号“:”（半角），再加上端口号。比如使用 “8080” 作为 WWW 服务的端口，则需要在地址栏里输入“网址:8080”。但是有些系统协议使用固定的端口号，它是不能被改变的，比如 139 端口专门用于 NetBIOS 与 TCP/IP 之间的通信，不能手动改变。

2.注册端口（Registered Ports）

端口号从 1024 到 49151，它们松散地绑定于一些服务，分配给用户进程或应用程序，这些进程主要是用户选择安装的一些应用程序，而不是已经分配好了公认端口的常用程序。这些端口在没有被服务器资源占用的时候，可以用用户端动态选用为源端口。

3.动态端口 / 私有端口（Dynamic Ports / Private Ports）

动态端口的范围是从 49152 到 65535。之所以称为动态端口，是因为它一般不固定分配某种服务，而是 动态分配。

### 4.5 **网络模型**

**OSI 七层参考模型**

-----

> 七层模型，亦称 OSI（Open System Interconnection）参考模型，即开放式系统互联。参考模型是国际标准化组织（ISO）制定的一个用于计算机或通信系统间互联的标准体系，一般称为 OSI 参考模型或七层模型。
>
> 它是一个七层的、抽象的模型体，不仅包括一系列抽象的术语或概念，也包括具体的协议。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330100442670.png" alt="image-20210330100442670" style="zoom:50%;" />

记忆口诀（物数网传会表应）底层 -> 顶层

1. 物理层：主要定义物理设备标准，如网线的接口类型、光纤的接口类型、各种传输介质的传输速率等。它的主要作用是传输比特流（就是由1、0转化为电流强弱来进行传输，到达目的地后再转化为1、0，也就是我们常说的数模转换与模数转换）。这一层的数据叫做比特。
2. 数据链路层：建立逻辑连接、**进行硬件地址寻址**、差错校验等功能。定义了如何让格式化数据以帧为单位进行传输，以及如何让控制对物理介质的访问。将比特组合成字节进而组合成帧，用MAC地址访问介质。（对mac地址的封装）
3. 网络层：进行逻辑地址寻址（IP地址），在位于不同地理位置的网络中的两个主机系统之间提供连接和路径选择。Internet的发展使得从世界各站点访问信息的用户数大大增加，而网络层正是管理这种连接的层。
4. 传输层：定义了一些传输数据的协议和端口号（ WWW 端口 80 等），如：TCP（传输控制协议，传输效率低，可靠性强，用于传输可靠性要求高，数据量大的数据），UDP（用户数据报协议，与 TCP 特性恰恰相反，用于传输可靠性要求不高，数据量小的数据，如 QQ 聊天数据就是通过这种方式传输的）。 主要是将从下层接收的数据进行分段和传输，到达目的地址后再进行重组。常常把这一层数据叫做段。
5. 会话层：通过传输层（端口号：传输端口与接收端口）建立数据传输的通路。主要在你的系统之间发起会话或者接受会话请求。
6. 表示层：数据的表示、安全、压缩。主要是进行对接收的数据进行解释、加密与解密、压缩与解压缩等（也就是把计算机能够识别的东西转换成人能够能识别的东西（如图片、声音等）。
7. 应用层：网络服务与最终用户的一个接口。这一层为用户的应用程序（例如电子邮件、文件传输和终端仿真）提供网络服务。

**TCP/IP 四层模型**

----

**简介**

> 现在 Internet（因特网）使用的主流协议族是 TCP/IP 协议族，它是一个分层、多协议的通信体系。TCP/IP协议族是一个四层协议系统，自底而上分别是数据链路层、网络层、传输层和应用层。每一层完成不同的功能，且通过若干协议来实现，上层协议使用下层协议提供的服务。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330100659006.png" alt="image-20210330100659006" style="zoom:50%;" />

TCP/IP 协议在一定程度上参考了 OSI 的体系结构。OSI 模型共有七层，从下到上分别是物理层、数据链路层、网络层、传输层、会话层、表示层和应用层。但是这显然是有些复杂的，所以在 TCP/IP 协议中，它们被简化为了四个层次。

（1）应用层、表示层、会话层三个层次提供的服务相差不是很大，所以在 TCP/IP 协议中，它们被合并为应用层一个层次。 

（2）由于传输层和网络层在网络协议中的地位十分重要，所以在 TCP/IP 协议中它们被作为独立的两个层次。 

（3）因为数据链路层和物理层的内容相差不多，所以在 TCP/IP 协议中它们被归并在网络接口层一个层次里。只有四层体系结构的 TCP/IP 协议，与有七层体系结构的 OSI 相比要简单了不少，也正是这样，TCP/IP 协议在实际的应用中效率更高，成本更低。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330100807851.png" alt="image-20210330100807851" style="zoom:50%;" />

**四层介绍**

1. 应用层：应用层是 TCP/IP 协议的第一层，是直接为应用进程提供服务的。 
   1. 对不同种类的应用程序它们会根据自己的需要来使用应用层的不同协议，邮件传输应用使用 了 SMTP 协议、万维网应用使用了 HTTP 协议、远程登录服务应用使用了有 TELNET 协议。 
   2. 应用层还能加密、解密、格式化数据。 
   3. 应用层可以建立或解除与其他节点的联系，这样可以充分节省网络资源。
2. 传输层：作为 TCP/IP 协议的第二层，运输层在整个 TCP/IP 协议中起到了中流砥柱的作用。且在运 输层中， TCP 和 UDP 也同样起到了中流砥柱的作用。
3. 网络层：网络层在 TCP/IP 协议中的位于第三层。在 TCP/IP 协议中网络层可以进行网络连接的建立和终止以及 IP 地址的寻找等功能。
4. 网络接口层：在 TCP/IP 协议中，网络接口层位于第四层。由于网络接口层兼并了物理层和数据链路层所以，网络接口层既是传输数据的物理媒介，也可以为网络层提供一条准确无误的线路。

### 4.6 协议

> 协议，网络协议的简称，网络协议是通信计算机双方必须共同遵从的一组约定。如怎么样建立连接、怎么样互相识别等。只有遵守这个约定，计算机之间才能相互通信交流。它的三要素是：语法、语义、时序。
>
> 为了使数据在网络上从源到达目的，网络通信的参与方必须遵循相同的规则，这套规则称为协议（protocol），它最终体现为在网络上传输的数据包的格式。
>
> 协议往往分成几个层次进行定义，分层定义是为了使某一层协议的改变不影响其他层次的协议。

**常见协议**

应用层常见的协议有：FTP协议（File Transfer Protocol 文件传输协议）、HTTP协议（Hyper Text Transfer Protocol 超文本传输协议）、NFS（Network File System 网络文件系统）。 

传输层常见协议有：TCP协议（Transmission Control Protocol 传输控制协议）、UDP协议（User Datagram Protocol 用户数据报协议）。 

网络层常见协议有：IP 协议（Internet Protocol 因特网互联协议）、ICMP 协议（Internet Control Message Protocol 因特网控制报文协议）、IGMP 协议（Internet Group Management Protocol 因特 网组管理协议）。 

网络接口层常见协议有：ARP协议（Address Resolution Protocol 地址解析协议）、RARP协议（Reverse Address Resolution Protocol 反向地址解析协议）。

**UDP协议**

----

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330102347803.png" alt="image-20210330102347803" style="zoom:50%;" />

1. 源端口号：发送方端口号
2. 目的端口号：接收方端口号
3. 长度：UDP用户数据报的长度，最小值是8（仅有首部）
4. 校验和：检测UDP用户数据报在传输中是否有错，有错就丢弃

**TCP协议**

----

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330102507254.png" alt="image-20210330102507254" style="zoom: 70%;" />

1. 源端口号：发送方端口号 
2. 目的端口号：接收方端口号 
3. 序列号：本报文段的数据的第一个字节的序号 
4. 确认序号：期望收到对方下一个报文段的第一个数据字节的序号 
5. 首部长度（数据偏移）：TCP 报文段的数据起始处距离 TCP 报文段的起始处有多远，即首部长 度。单位：32位，即以 4 字节为计算单位
6. 保留：占 6 位，保留为今后使用，目前应置为 0 
7. 紧急 URG ：此位置 1 ，表明紧急指针字段有效，它告诉系统此报文段中有紧急数据，应尽快传送 
8. 确认 ACK：仅当 ACK=1 时确认号字段才有效，TCP 规定，在连接建立后所有传达的报文段都必须 把 ACK 置1
9. 推送 PSH：当两个应用进程进行交互式的通信时，有时在一端的应用进程希望在键入一个命令后立 即就能够收到对方的响应。在这种情况下，TCP 就可以使用推送（push）操作，这时，发送方 TCP 把 PSH 置 1，并立即创建一个报文段发送出去，接收方收到 PSH = 1 的报文段，就尽快地 （即“推送”向前）交付给接收应用进程，而不再等到整个缓存都填满后再向上交付
10. 复位 RST：用于复位相应的 TCP 连接 
11. 同步 SYN：仅在三次握手建立 TCP 连接时有效。当 SYN = 1 而 ACK = 0 时，表明这是一个连接请 求报文段，对方若同意建立连接，则应在相应的报文段中使用 SYN = 1 和 ACK = 1。因此，SYN 置 1 就表示这是一个连接请求或连接接受报文
12. 终止 FIN：用来释放一个连接。当 FIN = 1 时，表明此报文段的发送方的数据已经发送完毕，并要 求释放运输连接
13. 窗口：指发送本报文段的一方的接收窗口（而不是自己的发送窗口） 
14. 校验和：校验和字段检验的范围包括首部和数据两部分，在计算校验和时需要加上 12 字节的伪头 部
15. 紧急指针：仅在 URG = 1 时才有意义，它指出本报文段中的紧急数据的字节数（紧急数据结束后就 是普通数据），即指出了紧急数据的末尾在报文中的位置，注意：即使窗口为零时也可发送紧急数 据
16. 选项：长度可变，最长可达 40 字节，当没有使用选项时，TCP 首部长度是 20 字节

**IP协议**

----

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330104731870.png" alt="image-20210330104731870" style="zoom:50%;" />

1. 版本：IP 协议的版本。通信双方使用过的 IP 协议的版本必须一致，目前最广泛使用的 IP 协议版本 号为 4（即IPv4)

2. 首部长度：单位是 32 位（4 字节） 

3. 服务类型：一般不适用，取值为 0 

4. 总长度：指首部加上数据的总长度，单位为字节 

5. 标识（identification）：IP 软件在存储器中维持一个计数器，每产生一个数据报，计数器就加 1， 并将此值赋给标识字段

6. 标志（flag）：目前只有两位有意义。 标志字段中的最低位记为 MF。MF = 1 即表示后面“还有分片”的数据报。MF = 0 表示这已是若干数 据报片中的最后一个。 

   标志字段中间的一位记为 DF，意思是“不能分片”，只有当 DF = 0 时才允许分片

7. 片偏移：指出较长的分组在分片后，某片在源分组中的相对位置，也就是说，相对于用户数据段的 起点，该片从何处开始。片偏移以 8 字节为偏移单位。

8. 生存时间：TTL，表明是数据报在网络中的寿命，即为“跳数限制”，由发出数据报的源点设置这个字段。路由器在转发数据之前就把 TTL 值减一，当 TTL 值减为零时，就丢弃这个数据报。

9. 协议：指出此数据报携带的数据时使用何种协议，以便使目的主机的 IP 层知道应将数据部分上交 给哪个处理过程，常用的 ICMP(1)，IGMP(2)，TCP(6)，UDP(17)，IPv6（41）

10. 首部校验和：只校验数据报的首部，不包括数据部分。 

11. 源地址：发送方 IP 地址

12. 目的地址：接收方 IP 地址

**以太网帧协议**

----

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330105025279.png" alt="image-20210330105025279" style="zoom: 67%;" />

类型：0x800表示 IP、0x806表示 ARP、0x835表示 RARP

**ARP协议**

----

![image-20210330105114361](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330105114361.png)

1. 硬件类型：1 表示 MAC 地址 
2. 协议类型：0x800 表示 IP 地址 
3. 硬件地址长度：6 
4. 协议地址长度：4
5. 操作：1 表示 ARP 请求，2 表示 ARP 应答，3 表示 RARP 请求，4 表示 RARP 应答

**封装**

---

上层协议是如何使用下层协议提供的服务的呢？其实这是通过封装（encapsulation）实现的。应用程序数据在发送到物理网络上之前，将沿着协议栈从上往下依次传递。每层协议都将在上层数据的基础上加上自己的头部信息（有时还包括尾部信息），以实现该层的功能，这个过程就称为封装。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330105228982.png" alt="image-20210330105228982" style="zoom:50%;" />

**分用**

---

当帧到达目的主机时，将沿着协议栈自底向上依次传递。各层协议依次处理帧中本层负责的头部数据，以获取所需的信息，并最终将处理后的帧交给目标应用程序。这个过程称为分用（demultiplexing）。分用是依靠头部信息中的类型字段实现的。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330105331771.png" alt="image-20210330105331771" style="zoom:50%;" />

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330105341694.png" alt="image-20210330105341694" style="zoom:50%;" />

![网络通信的过程](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/%E7%BD%91%E7%BB%9C%E9%80%9A%E4%BF%A1%E7%9A%84%E8%BF%87%E7%A8%8B.png)

![arp请求封装](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/arp%E8%AF%B7%E6%B1%82%E5%B0%81%E8%A3%85.png)

### 4.7 socket 介绍

> 所谓 socket（套接字），就是对网络中不同主机上的应用进程之间进行双向通信的端点的抽象。一个套接字就是网络上进程通信的一端，提供了应用层进程利用网络协议交换数据的机制。从所处的地位来讲，套接字上联应用进程，下联网络协议栈，是应用程序通过网络协议进行通信的接口，是应用程序与网络协议根进行交互的接口。
>
>  socket 可以看成是两个网络应用程序进行通信时，各自通信连接中的端点，这是一个逻辑上的概念。它是网络环境中进程间通信的 API，也是可以被命名和寻址的通信端点，使用中的每一个套接字都有其类型和一个与之相连进程。通信时其中一个网络应用程序将要传输的一段信息写入它所在主机的 socket 中，该 socket 通过与网络接口卡（NIC）相连的传输介质将这段信息送到另外一台主机的 socket 中，使对方能够接收到这段信息。**socket 是由 IP 地址和端口结合的，提供向应用层进程传送数据包的机制。** 
>
> socket 本身有“插座”的意思，在 Linux 环境下，用于表示进程间网络通信的特殊文件类型。本质为内核借助缓冲区形成的伪文件。既然是文件，那么理所当然的，我们可以使用文件描述符引用套接字。与管道类似的，Linux 系统将其封装成文件的目的是为了统一接口，使得读写套接字和读写文件的操作一致。区别是管道主要应用于本地进程间通信，而套接字多应用于网络进程间数据的传递。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330194701940.png" alt="image-20210330194701940" style="zoom:50%;" />

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330202706650.png" alt="image-20210330202706650" style="zoom:67%;" />

```c
// 套接字通信分两部分： 
- 服务器端：被动接受连接，一般不会主动发起连接 
- 客户端：主动向服务器发起连接
    
socket是一套通信的接口，Linux 和 Windows 都有，但是有一些细微的差别。
```

### 4.8 字节序

**简介**

---

> 现代 CPU 的累加器一次都能装载（至少）4 字节（这里考虑 32 位机），即一个整数。那么这 4 字节在内存中排列的顺序将影响它被累加器装载成的整数的值，这就是字节序问题。在各种计算机体系结构中，对于字节、字等的存储机制有所不同，因而引发了计算机通信领域中一个很重要的问题，即通信双方交流的信息单元（比特、字节、字、双字等等）应该以什么样的顺序进行传送。如果不达成一致的规则，通信双方将无法进行正确的编码/译码从而导致通信失败。
>
> **字节序，顾名思义字节的顺序，就是大于一个字节类型的数据在内存中的存放顺序(一个字节的数据当然就无需谈顺序的问题了)。**
>
> 字节序分为大端字节序（Big-Endian） 和小端字节序（Little-Endian）。大端字节序是指一个整数的最高位字节（23 ~ 31 bit）存储在内存的低地址处，低位字节（0 ~ 7 bit）存储在内存的高地址处；小端字节序则是指整数的高位字节存储在内存的高地址处，而低位字节则存储在内存的低地址处。

**字节序举例**

----

- 小端字节序

 0x 01 02 03 04 - ff = 255 

内存的方向 -----> 

内存的低位 -----> 内存的高位 

04 03 02 01

0x 11 22 33 44 12 34 56 78

![image-20210330203755821](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330203755821.png)

- 大端字节序

  0x 01 02 03 04

  内存的方向 ----->

  内存的低位 -----> 内存的高位

  01 02 03 04

  0x 12 34 56 78 11 22 33 44

  ![image-20210330203920178](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330203920178.png)

  ```c
  /*
      字节序：就是字节在内存中存放的顺序
      小端字节序：数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位地址
      大端字节序：数据的低位字节存储在内存的高位地址，高位字节存储在内存的高位地址
  */
  
  // 通过代码检测当前的主机字节序
  
  #include <stdio.h>
  
  int main(){
      union 
      {
          short val;  // 两个字节
          char byte[sizeof(short)];   // char[2]
      }test;
  
      test.val = 0x0102;
      if((test.byte[0] == 1) && (test.byte[1] == 2)){
          printf("大端字节序。\n");
      }else if((test.byte[0] == 2) && (test.byte[1] == 1)){
          printf("小端字节序。\n");
      }else{
          printf("未知！\n");
      }
      return 0;
  }
  ```

  

**字节序转换函数**

---

当格式化的数据在两台使用不同字节序的主机之间直接传递时，接收端必然错误的解释之。解决问题的方法是：发送端总是把要发送的数据转换成大端字节序数据后再发送，而接收端知道对方传送过来的数据总是采用大端字节序，所以接收端可以根据自身采用的字节序决定是否对接收到的数据进行转换（小端机转换，大端机不转换）。

网络字节顺序是 TCP/IP 中规定好的一种数据表示格式，它与具体的 CPU 类型、操作系统等无关，从而可以保证数据在不同主机之间传输时能够被正确解释，网络字节顺序采用大端排序方式。 

BSD Socket提供了封装好的转换接口，方便程序员使用。包括从主机字节序到网络字节序的转换函数：
htons、htonl；从网络字节序到主机字节序的转换函数：ntohs、ntohl。

```c
h - host 主机，主机字节序
to - 转换成什么 n s l
- network 网络字节序
- short unsigned short 
- long unsigned int
```

```c
#include <arpa/inet.h> 
// 转换端口 
uint16_t htons(uint16_t hostshort); // 主机字节序 - 网络字节序 
uint16_t ntohs(uint16_t netshort);	// 主机字节序 - 网络字节序

// 转IP 
uint32_t htonl(uint32_t hostlong); 	// 主机字节序 - 网络字节序
uint32_t ntohl(uint32_t netlong);	// 主机字节序 - 网络字节序

```

```c
/*
    网络通信的时候，需要将主机字节序转换为网络字节序（大端）
    另外一端获取到数据以后，根据情况将网络字节序转换为主机字节序。

    // 转换端口 
    uint16_t htons(uint16_t hostshort); // 主机字节序 - 网络字节序 
    uint16_t ntohs(uint16_t netshort);	// 主机字节序 - 网络字节序

    // 转IP 
    uint32_t htonl(uint32_t hostlong); 	// 主机字节序 - 网络字节序
    uint32_t ntohl(uint32_t netlong);	// 主机字节序 - 网络字节序

*/


#include <stdio.h>
#include <arpa/inet.h>

int main(){
    // htons    转换端口
    unsigned short a = 0x0102;
    printf(" a = %x \n",a);
    unsigned short b = htons(a);
    printf(" b = %x \n",b);

    printf ("----------------------\n");
    // htonl 转换IP
    char buf[4] = {192,168,1,101};
    unsigned int num = *(int *)buf;
    int sum = htonl(num);

    unsigned char *p = (char *)&sum;
    printf("%d.%d.%d.%d\n",*p,*(p+1),*(p+2),*(p+3));

    printf ("----------------------\n");

    // ntohl 转IP
    unsigned char buf1[4] = {1,1,168,192};
    int num1 = *(int *)buf1;
    int sum1 = ntohl(num1);
    unsigned char *p1 = (unsigned char *)&sum1;
    printf("%d.%d.%d.%d\n",*p1,*(p1+1),*(p1+2),*(p1+3));

    printf ("----------------------\n");
    // noths 转端口
    unsigned short num2 = 0x0201;
    unsigned short sum2 = ntohs(num2);

    printf(" %x \n",sum2);


    return 0;
}
```

### 4.9 socket 地址

```c
// socket地址其实是一个结构体，封装端口号和IP等信息。后面的socket相关的api中需要使用到这个socket地址。 
// 客户端 -> 服务器（IP, Port）
```

**通用 socket 地址** 
socket 网络编程接口中表示 socket 地址的是结构体 sockaddr，其定义如下：

```c
#include <bits/socket.h> 
struct sockaddr { 
    sa_family_t sa_family; 
    char sa_data[14];
}
typedef unsigned short int sa_family_t;
```

sa_family 成员是地址族类型（sa_family_t）的变量。地址族类型通常与协议族类型对应。常见的协议 族（protocol family，也称 domain）和对应的地址族入下所示：

![image-20210330213756510](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330213756510.png)

宏 PF_  * 和 AF_ * 都定义在 bits/socket.h 头文件中，且后者与前者有完全相同的值，所以二者通常混 用。

![image-20210330213859831](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330213859831.png)

由上表可知，14 字节的 sa_data 根本无法容纳多数协议族的地址值。因此，Linux 定义了下面这个新的 通用的 socket 地址结构体，这个结构体不仅提供了足够大的空间用于存放地址值，而且是内存对齐的。

```c
#include <bits/socket.h> 
struct sockaddr_storage {
sa_family_t sa_family; 
unsigned long int __ss_align; 
char __ss_padding[ 128 - sizeof(__ss_align) ];
};
typedef unsigned short int sa_family_t;
```

**专用 socket 地址**

很多网络编程函数诞生早于 IPv4 协议，那时候都使用的是 struct sockaddr 结构体，为了向前兼容，现在sockaddr 退化成了（void *）的作用，传递一个地址给函数，至于这个函数是 sockaddr_in 还是sockaddr_in6，由地址族确定，然后函数内部再强制类型转化为所需的地址类型。

![image-20210330223752565](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210330223752565.png)

UNIX 本地域协议族使用如下专用的 socket 地址结构体：

```c
#include <sys/un.h> 
struct sockaddr_un {
sa_family_t sin_family; 
char sun_path[108];
};
```

TCP/IP 协议族有 sockaddr_in 和 sockaddr_in6 两个专用的 socket 地址结构体，它们分别用于 IPv4 和 IPv6：

```c
#include <netinet/in.h> 
struct sockaddr_in {
	sa_family_t sin_family; /* __SOCKADDR_COMMON(sin_) */ 
    in_port_t sin_port;/* Port number. */
	struct in_addr sin_addr;/* Internet address. */
	
	/* Pad to size of `struct sockaddr'. */ 
    unsigned char sin_zero[sizeof (struct sockaddr) - __SOCKADDR_COMMON_SIZE sizeof 		(in_port_t) - sizeof (struct in_addr)];
};
struct in_addr {
	in_addr_t s_addr; 
};
struct sockaddr_in6 {
	sa_family_t sin6_family; 
    in_port_t sin6_port;/* Transport layer port # */
	uint32_t sin6_flowinfo; /* IPv6 flow information */ 
    struct in6_addr sin6_addr; /* IPv6 address */ 
    uint32_t sin6_scope_id; /* IPv6 scope-id */
};
typedef unsigned short uint16_t; 
typedef unsigned int uint32_t;
typedef uint16_t in_port_t; 
typedef uint32_t in_addr_t;
#define __SOCKADDR_COMMON_SIZE (sizeof (unsigned short int))
```

所有专用 socket 地址（以及 sockaddr_storage）类型的变量在实际使用时都需要转化为通用 socket 地 址类型 sockaddr（强制转化即可），因为所有 socket 编程接口使用的地址参数类型都是 sockaddr。

### 4.10 IP地址转换

（字符串ip-整数 ，主机、网络 字节序的转换）

通常，人们习惯用可读性好的字符串来表示 IP 地址，比如用点分十进制字符串表示 IPv4 地址，以及用十六进制字符串表示 IPv6 地址。但编程中我们需要先把它们转化为整数（二进制数）方能使用。而记录日志时则相反，我们要把整数表示的 IP 地址转化为可读的字符串。下面 3 个函数可用于用点分十进制字符串表示的 IPv4 地址和用网络字节序整数表示的 IPv4 地址之间的转换：

```c
#include <arpa/inet.h> 
in_addr_t inet_addr(const char *cp); //把（点分十进制的字符串）日志字节序转为网络字节序
int inet_aton(const char *cp, struct in_addr *inp);	//第二个参数用于保存转换后的字符内容（传出参数）
	-返回值
        0 - 表示成功
        1 - 表示失败，设置错误码
char *inet_ntoa(struct in_addr in); // 网络型的字符串转为点分式的字符串
```

下面这对更新的函数也能完成前面 3 个函数同样的功能，并且它们同时适用 IPv4 地址和 IPv6 地址：

```c
#include <arpa/inet.h> 
// p:点分十进制的IP字符串，n:表示network，网络字节序的整数 
int inet_pton(int af, const char *src, void *dst); 
	af:地址族： AF_INET AF_INET6 
    src:需要转换的点分十进制的IP字符串 
    dst:转换后的结果保存在这个里面
	-返回值
        0 - 表示成功
        1 - 表示失败，设置错误码        
// 将网络字节序的整数，转换成点分十进制的IP地址字符串 
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size); 
		af:地址族： AF_INET AF_INET6 
        src: 要转换的ip的整数的地址 
        dst: 转换成IP地址字符串保存的地方 
        size：第三个参数的大小（数组的大小）
		返回值：返回转换后的数据的地址（字符串），和 dst 是一样的
```

### 4.11 TCP通信流程

```c
// TCP 和 UDP -> 传输层的协议 
UDP:用户数据报协议，面向无连接，可以单播，多播，广播， 面向数据报，不可靠
TCP:传输控制协议，面向连接的，可靠的，基于字节流，仅支持单播传输
```

|                |              UDP               |            TCP             |
| :------------: | :----------------------------: | :------------------------: |
|  是否创建连接  |             无连接             |          面向连接          |
|    是否可靠    |             不可靠             |           可靠的           |
| 连接的对象个数 | 一对一、一对多、多对一、多对多 |         支持一对一         |
|   传输的方式   |           面向数据报           |         面向字节流         |
|    首部开销    |            8个字节             |        最少20个字节        |
|    适用场景    |   实时应用（视频会议，直播）   | 可靠性高的应用（文件传输） |

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210331125038719.png" alt="image-20210331125038719" style="zoom:50%;" />

```c
// TCP 通信的流程 
// 服务器端 （被动接受连接的角色） 
1. 创建一个用于监听的套接字 
    - 监听：监听有客户端的连接 
    - 套接字：这个套接字其实就是一个文件描述符
2. 将这个监听文件描述符和本地的IP和端口绑定（IP和端口就是服务器的地址信息） 
    - 客户端连接服务器的时候使用的就是这个IP和端口
3. 设置监听，监听的fd开始工作 
4. 阻塞等待，当有客户端发起连接，解除阻塞，接受客户端的连接，会得到一个和客户端通信的套接字 （fd） 5. 通信
	- 接收数据 
    - 发送数据
6. 通信结束，断开连接
    
// 客户端 
1. 创建一个用于通信的套接字（fd） 
2. 连接服务器，需要指定连接的服务器的 IP 和 端口
3. 连接成功了，客户端可以直接和服务器通信 
    	- 接收数据 
    	- 发送数据
4. 通信结束，断开连接
```

### 4.12 套接字函数

```c
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> // 包含了这个头文件，上面两个就可以省略

int socket(int domain, int type, int protocol); 
	- 功能：创建一个套接字 
    - 参数： 
        - domain: 协议族 
            AF_INET : ipv4 
            AF_INET6 : ipv6 
            AF_UNIX, AF_LOCAL : 本地套接字通信（进程间通信）
		- type: 通信过程中使用的协议类型 
            SOCK_STREAM : 流式协议 
            SOCK_DGRAM  : 报式协议
		- protocol : 具体的一个协议。一般写0 
            - SOCK_STREAM : 流式协议默认使用 TCP 
            - SOCK_DGRAM  : 报式协议默认使用 UDP
	- 返回值： 
            - 成功：返回文件描述符，操作的就是内核缓冲区。 
            - 失败：-1
                
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); // socket命名
	- 功能：绑定，将fd和本地的IP + 端口进行绑定 
    - 参数：
		- sockfd  : 通过socket函数得到的文件描述符 
    	- addr 	  : 需要绑定的socket地址，这个地址封装了ip和端口号的信息 
        - addrlen : 第二个参数结构体占的内存大小
     -返回值：
            -成功 返回0
            -失败 返回-1，设置错误号
            
int listen(int sockfd, int backlog); // /proc/sys/net/core/somaxconn
	- 功能：监听这个socket上的连接 
        - 参数： 
        	- sockfd : 通过socket()函数得到的文件描述符 
            - backlog : 未连接的和已经连接的和的最大值， 5
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 
	- 功能：接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接 
    - 参数：
		- sockfd : 用于监听的文件描述符 
        - addr : 传出参数，记录了连接成功后客户端的地址信息（ip，port） 
        - addrlen : 指定第二个参数的对应的内存大小
	- 返回值：
	- 成功 ：用于通信的文件描述符 - -1 ： 失败
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	- 功能： 客户端连接服务器 
    	- 参数：
			- sockfd : 用于通信的文件描述符 
        	- addr : 客户端要连接的服务器的地址信息 
       	 	- addrlen : 第二个参数的内存大小
	- 返回值：
            成功 0， 失败 -1
ssize_t write(int fd, const void *buf, size_t count); 	// 写数据
ssize_t read(int fd, void *buf, size_t count);			// 读数据
```

**server**

```c
// tcp服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    // 1.创建socket(用于监听的套接字)
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }
    // 2.绑定IP端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;

    inet_pton(AF_INET,"192.168.190.143",&saddr.sin_addr.s_addr);
    // saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 3.监听
    re = listen(lfd,5);
    if(re == -1){
        perror("listen");
        exit(0);
    }

    // 4.接受客户端连接
    struct sockaddr_in Clientaddr;
    socklen_t len1 = sizeof(Clientaddr);
    int cfd = accept(lfd,(struct sockaddr *)&Clientaddr,&len1);
    if(cfd == -1){
        perror("accept");
        exit(0);
    }
    // 输出客户端的信息
    char clientIP[16] = {};
    inet_ntop(AF_INET,&Clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short clientport = ntohs(Clientaddr.sin_port);

    printf("IP : %s port : %d",clientIP,clientport);
    char recvBuf[1024] = {};
    // 获取客户端信息
    while(1){
        
        int len = read(cfd,recvBuf,sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(0);
        }else if(len > 0){
            printf("rev client data : %s\n",recvBuf);

        }else if(len == 0){
            printf("客户端断开连接\n");
            break;
        }
        // 给客户端发送数据
        char * data = "hello I am server";
        write(cfd,data,strlen(data));
    }
    
    // 关闭文件描述符
    close(cfd);
    close(lfd);
    return 0;
}


```

**client**

```c
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
```

### 4.13 TCP三次握手

**TCP 是一种面向连接的单播协议**，在发送数据前，通信双方必须在彼此间建立一条连接。所谓的“连接”，其实是客户端和服务器的内存里保存的一份关于对方的信息，如 IP 地址、端口号等。 

TCP 可以看成是一种字节流，它会处理 IP 层或以下的层的丢包、重复以及错误问题。在连接的建立过程 中，双方需要交换一些连接的参数。这些参数可以放在 TCP 头部。

 TCP 提供了一种可靠、面向连接、字节流、传输层的服务，采用三次握手建立一个连接。采用四次挥手
来关闭一个连接。

三次握手保证TCP之间建立可靠的连接。

三次握手发生在客户端连接的时候，当调用connect(),底层会通过TCP协议进行三次握手

![image-20210331200649572](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401110359947.png)

![image-20210401112211237](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401112211237.png)

- 16 位端口号（port number）：告知主机报文段是来自哪里（源端口）以及传给哪个上层协议或应用程序（目的端口）的。进行 TCP 通信时，客户端通常使用系统自动选择的临时端口号。
- 32 位序号（sequence number）：一次 TCP 通信（从 TCP 连接建立到断开）过程中某一个传输 方向上的字节流的每个字节的编号。假设主机 A 和主机 B 进行 TCP 通信，A 发送给 B 的第一个 TCP 报文段中，序号值被系统初始化为某个随机值 ISN（Initial Sequence Number，初始序号 值）。那么在该传输方向上（从 A 到 B），后续的 TCP 报文段中序号值将被系统设置成 ISN 加上 该报文段所携带数据的第一个字节在整个字节流中的偏移。例如，某个 TCP 报文段传送的数据是字 节流中的第 1025 ~ 2048 字节，那么该报文段的序号值就是 ISN + 1025。另外一个传输方向（从 B 到 A）的 TCP 报文段的序号值也具有相同的含义。 
- 32 位确认号（acknowledgement number）：用作对另一方发送来的 TCP 报文段的响应。其值是 收到的 TCP 报文段的序号值 + 标志位长度（SYN，FIN） + 数据长度 。假设主机 A 和主机 B 进行 TCP 通信，那么 A 发送出的 TCP 报文段不仅携带自己的序号，而且包含对 B 发送来的 TCP 报文段 的确认号。反之，B 发送出的 TCP 报文段也同样携带自己的序号和对 A 发送来的报文段的确认序 号。 
- 4 位头部长度（head length）：标识该 TCP 头部有多少个 32 bit(4 字节)。因为 4 位最大能表示
  15，所以 TCP 头部最长是60 字节。
- 6 位标志位包含如下几项：
  - URG 标志，表示紧急指针（urgent pointer）是否有效。 
  - ACK 标志，表示确认号是否有效。我们称携带 ACK 标志的 TCP 报文段为确认报文段。 
  - PSH 标志，提示接收端应用程序应该立即从 TCP 接收缓冲区中读走数据，为接收后续数据腾 出空间（如果应用程序不将接收到的数据读走，它们就会一直停留在 TCP 接收缓冲区中）。 
  - RST 标志，表示要求对方重新建立连接。我们称携带 RST 标志的 TCP 报文段为复位报文段。 
  - SYN 标志，表示请求建立一个连接。我们称携带 SYN 标志的 TCP 报文段为同步报文段。 
  - FIN 标志，表示通知对方本端要关闭连接了。我们称携带 FIN 标志的 TCP 报文段为结束报文段。 
  - 16 位窗口大小（window size）：是 TCP 流量控制的一个手段。这里说的窗口，指的是接收 通告窗口（Receiver Window，RWND）。它告诉对方本端的 TCP 接收缓冲区还能容纳多少 字节的数据，这样对方就可以控制发送数据的速度。 
  - 16 位校验和（TCP checksum）：由发送端填充，接收端对 TCP 报文段执行 CRC 算法以校验 TCP 报文段在传输过程中是否损坏。注意，这个校验不仅包括 TCP 头部，也包括数据部分。 这也是 TCP 可靠传输的一个重要保障。 
  - 16 位紧急指针（urgent pointer）：是一个正的偏移量。它和序号字段的值相加表示最后一 个紧急数据的下一个字节的序号。因此，确切地说，这个字段是紧急指针相对当前序号的偏移，不妨称之为紧急偏移。TCP 的紧急指针是发送端向接收端发送紧急数据的方法。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210331201301756.png" alt="image-20210331201301756" style="zoom: 67%;" />

```c
第一次握手
	1.客户端会将SYN位置为1
	2.生成一个随机的32的序号，这个序列号后边携带数据（数据的大小）
第二次握手
	1.服务器端接受客户端的连接 ACK = 1
	2.服务器回发确认信号 ack = 客户端的序号+数据长度+SYN/FIN(按一个字节算)
	3.服务器端向客户端发起连接请求：SYN = 1
	4.服务器生成一个随机序号：seq = K
第三次握手
	1.客户端应答服务器的连接请求ACK = 1
	2.客户端回复收到了服务器的数据：ack = 服务端的序号+数据长度+SYN/FIN(按一个字节算)
```

### 4.14 TCP 滑动窗口

> 滑动窗口（Sliding window）是一种流量控制技术。早期的网络通信中，通信双方不会考虑网络的拥挤情况直接发送数据。由于大家不知道网络拥塞状况，同时发送数据，导致中间节点阻塞掉包，谁也发不了数据，所以就有了滑动窗口机制来解决此问题。滑动窗口协议是用来改善吞吐量的一种技术，即容许发送方在接收任何应答之前传送附加的包。接收方告诉发送方在某一时刻能送多少包 （称窗口寸）。
>
>  TCP 中采用滑动窗口来进行传输控制，滑动窗口的大小意味着接收方还有多大的缓冲区可以用于接收数据。发送方可以通过滑动窗口的大小来确定应该发送多少字节的数据。当滑动窗口为 0 时，发送方一般不能再发送数据报。
>
> 滑动窗口是 TCP 中实现诸如 ACK 确认、流量控制、拥塞控制的承载结构。

窗口理解为缓冲区的大小

滑动缓冲区会随着发送数据和接受数据的情况而变化

通信的双方都有发送缓冲区和接受数据的缓冲区

服务器

​	发送缓冲器

​	接受缓冲器

客户端

​	发送缓冲区

​	接受缓冲区

![image-20210401124625425](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401124625425.png)

```c
发送方的缓冲区：
    白色格子：空闲的空间
    灰色格子：数据已经发送出去没有接受
    紫色格子：还未发送出去的数据
    
接收方的缓冲区：
    白色格子：空闲的空间
    紫色格子：已经接收到还未处理的数据
```

![image-20210401124737234](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401124737234.png)

```c
# mss ： Maximum Segment Siz(一条数据的最大数据量)
# win : 滑动窗口
1.客户端向服务器发起连接，客户端的滑动窗口是4096，一次发送的最大数据量为1460
2.服务器接受连接请求，告诉客户端服务器的窗口大小是6144，一次发送最大数据是1024
3.第三次握手
4.4-9 客户端连接服务器并发送了6k的数据，每次发送1k
5.第10次，服务器告诉客户端，发送的6K数据以及接收到，存储到缓冲区，缓冲区数据已经处理了2k，窗口大小是4k
6.第11次，服务器告诉客户端：发送的6k数据以及接收到，存储在缓冲区，缓冲区数据已经处理了4k, 窗口大小是4k
7. 第12次，客户端给服务器发送了1k的数据 
// 四次挥手
    
8. 第13次，客户端主动请求和服务器断开连接，并且给服务器发送了1k的数据 
9. 第14次，服务器回复ACK 8194, a:同意断开连接的请求 b:告诉客户端已经接受到方才发的2k的数据 c:滑动窗口2k 
10.第15、16次，通知客户端滑动窗口的大小 
11.第17次，第三次挥手，服务器端给客户端发送FIN,请求断开连接
12.第18次，第四次回收，客户端同意了服务器端的断开请求
```

### 4.15 TCP 四次挥手

![image-20210401142344147](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401142344147.png)

```c
四次挥手发生在连接断开的时候，在程序调用了close（）会使用TCP协议进行四次挥手.
客户端和服务端都可以主动发起断开连接，看谁先调用close()谁就先发起
因为在TCP连接时候采用三次握手的连接是双向的，所以我们在断开的时候也需要进行双向断开
```

![image-20210401143648845](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210401143648845.png)

### 4.16 TCP通信并发

> **并发**就是指的是有一个咖啡机，两队人轮流来喝咖啡
>
> **并行**就是指的是有两个咖啡机，两队人分别来喝咖啡

```
要实现TCP通信服务器处理并发的任务，要使用多线程或者多进程来解决。

思路：
	1.一个父进程，多个子进程
	2.父进程负责接受等待并接受客户端的连接
	3.子进程；完成通信，接受一个客户端连接，就创建一个子进程用于通信。
```

多进程实现并发通信

**server.c**

```c
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
```

**client.c**

```c
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
    int i = 0;
    char recvBuf[1024] = {0};
    while(1) {
        // 给服务端发送数据
        sprintf(recvBuf,"data : %d\n",i++);
        write(fd, recvBuf , strlen(recvBuf));
        
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            exit(-1);
        } else if(len > 0) {
            printf("recv server : %s\n", recvBuf);
        } else if(len == 0) {
            // 表示服务器端断开连接
            printf("server closed...");
            break;
        }
        sleep(1);
        

    }
    // 关闭连接
    close(fd);
    return 0;
}
```

### 4.17 TCP多线程实现并发服务器

**client_thread.c**

```c
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
    int i = 0;
    char recvBuf[1024] = {0};
    while(1) {
        // 给服务端发送数据
        sprintf(recvBuf,"data : %d\n",i++);
        write(fd, recvBuf , strlen(recvBuf));
        
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            exit(-1);
        } else if(len > 0) {
            printf("recv server : %s\n", recvBuf);
        } else if(len == 0) {
            // 表示服务器端断开连接
            printf("server closed...");
            break;
        }
        sleep(1);
        

    }
    // 关闭连接
    close(fd);
    return 0;
}
```

**server_thread.c**

```c
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct sockInfo{
    int fd;
    struct sockaddr_in addr;
    pthread_t tid;
};

struct sockInfo sockinfos[128];

void * MyFun(void * arg){
    // 进行子线程和客户端通信  cfd  客户端信息  线程号
    char cliIP[16] = {};
    struct sockInfo * pinfo = (struct sockInfo *)arg;
    inet_ntop(AF_INET,&pinfo->addr.sin_addr.s_addr,cliIP,(socklen_t)sizeof(cliIP));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("recv client IP : %s, PORT : %d \n",cliIP,cliPort);

    // 接受客户端发来的数据
    char buf[1024] = {};
    while(1){
        int num = read(pinfo->fd,buf,sizeof(buf));
        if(num == -1){
            perror("read");
            exit(0);
        }else if(num > 0){
            printf("rev client : %s \n",buf);
        }else if(num == 0){
            printf("client closed ...\n");
            break;
        }

        write(pinfo->fd,buf,strlen(buf)+1);
    }
    close(pinfo->fd);
    pthread_exit(NULL);
    return NULL;
}

int main(){
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
    // 初始化数据
    int max = sizeof(sockinfos)/sizeof(sockinfos[0]);
    for(int i = 0; i < max; i ++){
        bzero(&sockinfos[i],sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        // 设置文件描述符不可用

    }
    //循环的等待客户端的连接，一但有子进程连接进来就创建子线程
    while(1){
        // 接受连接
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);

        struct sockInfo *pinfo;
        for(int i = 0; i < max; i++){
            // 从数组中找到一个可用的sockInfo元素
            if(sockinfos[i].fd == -1){
                pinfo = &sockinfos[i];
                break;
            }
            if(i == max-1){
                sleep(1);
                i--;
            }
        }

        pinfo->fd = cfd;

        // memcpy(&pinfo->addr,&cliaddr,sizeof(cliaddr));
        pinfo->addr.sin_addr.s_addr = cliaddr.sin_addr.s_addr;
        pinfo->addr.sin_family = cliaddr.sin_family;
        pinfo->addr.sin_port = cliaddr.sin_port;
        

        // 创建子线程
        pthread_create(&pinfo->tid,NULL,MyFun,pinfo);

        // pthread_join(NULL);
        // 阻塞的

        pthread_detach(pinfo->tid);
    }
    
    return 0;

}
```

### 4.18 TCP状态转换

![image-20210406105052203](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210406105052203.png)

四次回收的ACK和FIN分开发送，是因为断开连接时单方面的，肯另外一端还要发送数据中间还要间隔一段时间，所以需要分开进行发送。

![image-20210406105812591](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210406105812591.png)

**客户端进入TIME_WAIT状态后经过两倍的报文寿命以后结束**

- 2MSL（Maximum Segment Lifetime）

  主动断开连接的一方, 最后进入一个 TIME_WAIT状态, 这个状态会持续: 2msl
  - msl: 官方建议: 2分钟, 实际是30s

  > 当 TCP 连接主动关闭方接收到被动关闭方发送的 FIN 和最终的 ACK 后，连接的主动关闭方必须处于TIME_WAIT 状态并持续 2MSL 时间。 这样就能够让 TCP 连接的主动关闭方在它发送的ACK 丢失的情况下重新发送最终的ACK。 主动关闭方重新发送的最终 ACK 并不是因为被动关闭方重传了ACK（它们并不消耗序列号，被动关闭方也不会重传），而是因为被动关闭方重传了它的 FIN。事实上，被动关闭方总是重传 FIN 直到它收到一个最终的 ACK。

- 半关闭

  > 当 TCP 链接中 A 向 B 发送 FIN 请求关闭，另一端 B 回应 ACK 之后（A 端进入 FIN_WAIT_2 状态），并没有立即发送 FIN 给 A，A 方处于半连接状态（半开关），此时 A 可以接收 B 发送的数据，但是 A 已经不能再向 B 发送数据。

从程序的角度，可以使用 API 来控制实现半连接状态：

```c
#include <sys/socket.h> 
int shutdown(int sockfd, int how); 
sockfd: 需要关闭的socket的描述符 
how: 允许为shutdown操作选择以下几种方式:
	SHUT_RD(0)：关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。
        		该套接字不再接收数据，任何当前在套接字接受缓冲区的数据将被无声的丢弃掉。
	SHUT_WR(1): 关闭sockfd的写功能，此选项将不允许sockfd进行写操作。进程不能在对此套接字发
出写操作。 
    SHUT_RDWR(2):关闭sockfd的读写功能。相当于调用shutdown两次：首先是以SHUT_RD,然后以
SHUT_WR。
```

使用 close 中止一个连接，但它只是减少描述符的引用计数，并不直接关闭连接，只有当描述符的引用计数为0时才关闭连接。shutdown不考虑描述符的引用计数，直接关闭描述符。也可选择中止一个方向的连接，只中止读或只中止写。

注意: 

1. 如果有多个进程共享一个套接字，close 每被调用一次，计数减 1 ，直到计数为 0 时，也就是所用进程都调用了close，套接字将被释放。

2. 在多进程中如果一个进程调用了 shutdown(sfd, SHUT_RDWR) 后，其它的进程将无法进行通信。但如果一个进程 close(sfd) 将不会影响到其它进程。

### 4.19 端口复用

> 端口复用最常用的用途是: 
>
> ​	防止服务器重启时之前绑定的端口还未释放
> ​	程序突然退出而系统没有释放端口

```c
#include <sys/types.h> 
#include <sys/socket.h> 
// 设置套接字的属性（不仅仅能设置端口复用） 
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen); 
	参数： 
        - sockfd : 要操作的文件描述符 
        - level : 级别 - SOL_SOCKET (端口复用的级别) 
        - optname : 选项的名称 
            - SO_REUSEADDR 
            - SO_REUSEPORT
		- optval : 端口复用的值（整形） 
            - 1 : 可以复用 
            - 0 : 不可以复用
		- optlen : optval参数的大小
            
端口复用，设置的时机是在服务器绑定端口之前。 
            setsockopt();
			bind();
```

常看网络相关信息的命令 

netstat
	参数：

​	-a 所有的socket 

​	-p 显示正在使用socket的程序的名称

​	-n 直接使用IP地址，而不通过域名服务器

TCP_server.c

```c
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);

    if(lfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    
    //int optval = 1;
    //setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        return -1;
    }

    // 接收客户端连接
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    if(cfd == -1) {
        perror("accpet");
        return -1;
    }

    // 获取客户端信息
    char cliIp[16];
    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof(cliIp));
    unsigned short cliPort = ntohs(cliaddr.sin_port);

    // 输出客户端的信息
    printf("client's ip is %s, and port is %d\n", cliIp, cliPort );

    // 接收客户端发来的数据
    char recvBuf[1024] = {0};
    while(1) {
        int len = recv(cfd, recvBuf, sizeof(recvBuf), 0);
        if(len == -1) {
            perror("recv");
            return -1;
        } else if(len == 0) {
            printf("客户端已经断开连接...\n");
            break;
        } else if(len > 0) {
            printf("read buf = %s\n", recvBuf);
        }

        // 小写转大写
        for(int i = 0; i < len; ++i) {
            recvBuf[i] = toupper(recvBuf[i]);
        }

        printf("after buf = %s\n", recvBuf);

        // 大写字符串发给客户端
        ret = send(cfd, recvBuf, strlen(recvBuf) + 1, 0);
        if(ret == -1) {
            perror("send");
            return -1;
        }
    }
    
    close(cfd);
    close(lfd);

    return 0;
}

```

TCP_client.c

```c
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

    // 创建socket
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in seraddr;
    inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr.s_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(9999);

    // 连接服务器
    int ret = connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    if(ret == -1){
        perror("connect");
        return -1;
    }

    while(1) {
        char sendBuf[1024] = {0};
        fgets(sendBuf, sizeof(sendBuf), stdin);

        write(fd, sendBuf, strlen(sendBuf) + 1);

        // 接收
        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            return -1;
        }else if(len > 0) {
            printf("read buf = %s\n", sendBuf);
        } else {
            printf("服务器已经断开连接...\n");
            break;
        }
    }

    close(fd);

    return 0;
}

```

### 4.20 I/O多路复用（I/O多路转接）

I/O 多路复用使得程序能同时监听多个文件描述符，能够提高程序的性能，Linux 下实现 I/O 多路复用的系统调用主要有 select、poll 和 epoll。

**常用I/O模型**

![image-20210407093349276](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407093349276.png)

![image-20210407093946828](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407093946828.png)

![image-20210407094012747](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407094012747.png)

![image-20210407094229399](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407094229399.png)

![image-20210407094528117](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407094528117.png)

![image-20210407094836019](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407094836019.png)

#### 4.20.1 select

> 主旨思想： 
>
> 1. 首先要构造一个关于文件描述符的列表，将要监听的文件描述符添加到该列表中。 
>
> 2. 调用一个系统函数，监听该列表中的文件描述符，直到这些描述符中的一个或者多个进行I/O 操作时，该函数才返回。
>    1. a.这个函数是阻塞 
>    2. b.函数对文件描述符的检测的操作是由内核完成的
>
> 3. 在返回时，它会告诉进程有多少（哪些）描述符要进行I/O操作。

```c
// sizeof(fd_set) = 128 1024
#include <sys/time.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/select.h> 
int select(int nfds, fd_set *readfds, 
           fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	- 参数：
		- nfds : 委托内核检测的最大文件描述符的值 + 1 
        - readfds : 要检测的文件描述符的读的集合，委托内核检测哪些文件描述符的读的属性 
        	- 一般检测读操作 
            - 对应的是对方发送过来的数据，因为读是被动的接收数据，检测的就是读缓冲区 
        	- 是一个传入传出参数
		- writefds : 要检测的文件描述符的写的集合，委托内核检测哪些文件描述符的写的属性 
            - 委托内核检测写缓冲区是不是还可以写数据（不满的就可以写）
		- exceptfds : 检测发生异常的文件描述符的集合 
        - timeout : 设置的超时时间 
        struct timeval { 
            long tv_sec ;	/* seconds */
            long tv_usec;	/* microseconds */
        }; 
		- NULL : 永久阻塞，直到检测到了文件描述符有变化 
        - tv_sec = 0 tv_usec = 0， 不阻塞 
        - tv_sec > 0 tv_usec > 0， 阻塞对应的时间
 	- 返回值 : 
		- -1 : 失败 
    	- >0(n) : 检测的集合中有n个文件描述符发生了变化
        
// 将参数文件描述符fd对应的标志位设置为0 
void FD_CLR(int fd, fd_set *set); 
// 判断fd对应的标志位是0还是1， 返回值 ： fd对应的标志位的值，0，返回0， 1，返回1 
int FD_ISSET(int fd, fd_set *set); 
// 将参数文件描述符fd 对应的标志位，设置为1
void FD_SET(int fd, fd_set *set);
// fd_set一共有1024 bit, 全部初始化为0 
void FD_ZERO(fd_set *set);
```

![image-20210407192553698](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407192553698.png)

![image-20210407203831149](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407203831149.png)

![image-20210407203858127](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407203858127.png)

**client.c**

```c
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

    // 创建socket
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in seraddr;
    inet_pton(AF_INET, "127.0.0.1", &seraddr.sin_addr.s_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(9999);

    // 连接服务器
    int ret = connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    if(ret == -1){
        perror("connect");
        return -1;
    }

    int num = 0;
    while(1) {
        char sendBuf[1024] = {0};
        // fgets(sendBuf, sizeof(sendBuf), stdin);

        sprintf(sendBuf,"send data %d",num++);
        
        write(fd, sendBuf, strlen(sendBuf) + 1);

        // 接收
        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            return -1;
        }else if(len > 0) {
            printf("read buf = %s\n", sendBuf);
        } else {
            printf("服务器已经断开连接...\n");
            break;
        }
        sleep(1);
    }

    close(fd);

    return 0;
}

```

**server.c**

```c
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){

    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);

    // 创建一个fd_set的一个集合，用来存放需要检测的文件描述符
    fd_set rdset,temp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd = lfd;

    while(1){
        temp = rdset;
        int ret = select(maxfd+1,&temp,NULL,NULL,NULL);
        if(ret == -1){
            perror("select");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 说明对应文件描述符的缓冲区发生了改变。
            if(FD_ISSET(lfd,&temp)){
                // 表示有新的客户端连接进来
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                // 将新的文件描述符加入到集合中
                FD_SET(cfd,&rdset);
                
                // 更新最大的文件描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }
            for(int i = lfd + 1;i <= maxfd; i++){
                if(FD_ISSET(i,&temp)){
                    // 说明文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        close(i);
                        FD_CLR(i,&rdset);
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(i,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;
}
```

#### 4.20.2 poll

```c
#include <poll.h> 
struct pollfd { 
    int	fd;				/* 委托内核检测的文件描述符 */
	short events; 		/* 委托内核检测文件描述符的什么事件 */
    short revents;		/* 文件描述符实际发生的事件 */
};
struct pollfd myfd; 
myfd.fd = 5; 
myfd.events = POLLIN | POLLOUT;

int poll(struct pollfd *fds, nfds_t nfds, int timeout); 
	- 参数： 
        - fds : 是一个struct pollfd 结构体数组，这是一个需要检测的文件描述符的集合 
    	- nfds : 这个是第一个参数数组中最后一个有效元素的下标 + 1 
        - timeout : 阻塞时长 
            0 : 不阻塞 
            -1 : 阻塞，当检测到需要检测的文件描述符有变化，解除阻塞 
            >0 : 阻塞的时长
	- 返回值： 
            -1 : 失败 
            >0（n） : 成功,n表示检测到集合中有n个文件描述符发生变化
```

![image-20210407204834712](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407204834712.png)

**server.c**

```c
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>

int main(){

    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);
    // 调用poll系统函数
    struct pollfd fds[1024];
    for(int i = 0;i < 1024;i++){
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;

    int nfds = 0;
    
    while(1){
        int ret = poll(fds,nfds + 1,-1);
        if(ret == -1){
            perror("poll");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 说明对应文件描述符的缓冲区发生了改变。
            if(fds[0].revents & POLLIN){
                // 表示有新的客户端连接进来
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                // 将新的文件描述符加入到集合中
                for(int i = 1 ;i < 1024; i++){
                    if(fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                // 更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd;
            }
            for(int i = 1;i <= nfds; i++){
                if(fds[i].revents & POLLIN){
                    // 说明文件描述符对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(fds[i].fd,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;
}
```

#### 4.20.3 epoll

![image-20210407214918549](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210407214918549.png)

```c
#include <sys/epoll.h> 
// 创建一个新的epoll实例。在内核中创建了一个数据，这个数据中有两个比较重要的数据，一个是需要检测的文件描述符的信息（红黑树），还有一个是就绪列表，存放检测到数据发送改变的文件描述符信息（双向链表）。 
int epoll_create(int size); 
- 参数： 
    size : 目前没有意义了。随便写一个数，必须大于0
- 返回值： 
    -1 : 失败
	> 0 : 文件描述符，操作epoll实例的
typedef union epoll_data { 
        void		*ptr;
		int			fd;
		uint32_t 	u32;
        uint64_t	u64;
} epoll_data_t;

// 对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息 
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); 
- 参数：
	- epfd : epoll实例对应的文件描述符 
    - op : 要进行什么操作 
        EPOLL_CTL_ADD: 添加 
        EPOLL_CTL_MOD: 修改 
        EPOLL_CTL_DEL: 删除
	- fd : 要检测的文件描述符 
    - event : 检测文件描述符什么事情
        struct epoll_event {
    		uint32_t event; 		/* Epoll events */
    		epoll_data_t data;		/* User data variable */
			};

    typedef union epoll_data { 
        void		*ptr;
        int			fd;
        uint32_t 	u32;
        uint64_t	u64;
    } epoll_data_t;
// 检测函数
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 
- 参数： 
    - epfd : epoll实例对应的文件描述符 
    - events : 传出参数，保存了发生了变化的文件描述符的信息 
    - maxevents : 第二个参数结构体数组的大小 
    - timeout : 阻塞时间 
        - 0 : 不阻塞 
        - -1 : 阻塞，直到检测到fd数据发生变化，解除阻塞 
        - > 0 : 阻塞的时长（毫秒）
- 返回值： 
        - 成功，返回发送变化的文件描述符的个数 > 0
		- 失败 -1
```

**server_epoll.c**

```c
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(){
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(10);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN | EPOLLOUT;
    epev.data.fd = lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    // 创建数组，用来保存内核返回的数据
    struct epoll_event arrep[1024];
    while(1){
        int ret = epoll_wait(epfd,arrep,1024,-1);
        if(ret == -1){
            perror("epoll");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            printf("ret = %d\n",ret);
            for(int i = 0;i < ret;i++){
                if(arrep[i].data.fd == lfd){
                    // 监听的文件描述符号，有客户端连接
                    struct sockaddr_in clientaddr;
                    int len = sizeof(clientaddr);
                    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                    epev.events = EPOLLIN;
                    epev.data.fd = cfd;

                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
                }else{
                    if(arrep[i].events & EPOLLOUT){
                        continue;
                    }
                    // 说明有数据到达
                    char buf[1024] = {0};
                    int len = read(arrep[i].data.fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        epoll_ctl(epfd,EPOLL_CTL_DEL,arrep[i].data.fd,NULL);
                        close(arrep[i].data.fd);
                        
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(arrep[i].data.fd,buf,strlen(buf)+1);
                    }

                }
            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}
```



- Epoll 的工作模式：
  
  - LT 模式 （水平触发） 
    
    假设委托内核检测读事件 -> 检测fd的读缓冲区 
    
    ​	读缓冲区有数据 - > epoll检测到了会给用户通知 
    
    ​		a.用户不读数据，数据一直在缓冲区，epoll 会一直通知 
    
    ​		b.用户只读了一部分数据，epoll会通知
    
    ​		c.缓冲区的数据读完了，不通知
    
    > LT（level - triggered）是缺省的工作方式，并且同时支持 block 和 no-block socket。在这 种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的 fd 进行 IO 操作。如果你不作任何操作，内核还是会继续通知你的。
    
  - ET 模式（边沿触发）
  
    假设委托内核检测读事件 -> 检测fd的读缓冲区 
  
    ​	读缓冲区有数据 - > epoll检测到了会给用户通知 
  
    ​		a.用户不读数据，数据一致在缓冲区中，epoll下次检测的时候就不通知了 
  
    ​		b.用户只读了一部分数据,epoll不通知
  
    ​		c.缓冲区的数据读完了，不通知
  
    > ET（edge - triggered）是高速工作方式，只支持 no-block socket。在这种模式下，当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了。但是请注意，如果一直不对这个 fd 作 IO 操作（从而导致它再次变成 未就绪），内核不会发送更多的通知（only once）。 
    >
    > ET 模式在很大程度上减少了 epoll 事件被重复触发的次数，因此效率要比 LT 模式高。epoll 工作在 ET 模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。

```c
struct epoll_event { 
	uint32_t events;		/* Epoll events */
	epoll_data_t data;		/* User data variable */
}; 
常见的Epoll检测事件： 
	- EPOLLIN 
	- EPOLLOUT 
	- EPOLLERR 
	- EPOLLET     //边沿触发
```

**epoll_it.c**

```c
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(){
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(10);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    // 创建数组，用来保存内核返回的数据
    struct epoll_event arrep[1024];
    while(1){
        int ret = epoll_wait(epfd,arrep,1024,-1);
        if(ret == -1){
            perror("epoll");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            printf("ret = %d\n",ret);
            for(int i = 0;i < ret;i++){
                if(arrep[i].data.fd == lfd){
                    // 监听的文件描述符号，有客户端连接
                    struct sockaddr_in clientaddr;
                    int len = sizeof(clientaddr);
                    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);

                    epev.events = EPOLLIN;
                    epev.data.fd = cfd;

                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
                }else{
                    // 说明有数据到达
                    char buf[5] = {0};
                    int len = read(arrep[i].data.fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(0);
                    }else if(len == 0){
                        printf("client closed ....\n");
                        epoll_ctl(epfd,EPOLL_CTL_DEL,arrep[i].data.fd,NULL);
                        close(arrep[i].data.fd);
                        
                    }else if(len > 0){
                        printf("rev data : %s\n",buf);
                        write(arrep[i].data.fd,buf,strlen(buf)+1);
                    }

                }
            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}
```

**epoll_et.c**

```c
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

int main(){
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr =  INADDR_ANY;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;

    // 绑定
    int re = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));

    // 监听
    listen(lfd,8);

    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(100);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    // 创建数组，用来保存内核返回的数据
    struct epoll_event arrep[1024];
    while(1){
        int ret = epoll_wait(epfd,arrep,1024,-1);
        if(ret == -1){
            perror("epoll_wait");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            printf("ret = %d\n",ret);
            for(int i = 0;i < ret;i++){
                if(arrep[i].data.fd == lfd){
                    // 监听的文件描述符号，有客户端连接
                    struct sockaddr_in clientaddr;
                    int len = sizeof(clientaddr);
                    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);
                    // 设置cfd非阻塞
                    int flag = fcntl(cfd, F_GETFL);
                    flag = flag | O_NONBLOCK;
                    fcntl(cfd,F_SETFL,flag);

                    epev.events = EPOLLIN | EPOLLET;
                    epev.data.fd = cfd;

                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
                }else{
                    // 说明有数据到达
                    if(arrep[i].events & EPOLLOUT){
                        continue;
                    }
                    char buf[5] = {0};
                    // 循环读取所有的数据
                    int len = 0;
                    // 设置read为非阻塞的情形，通过文件描述符来设置
                    while((len = read(arrep[i].data.fd,buf,sizeof(buf)))>0){
                        // printf("rev data : %s\n",buf);
                        write(STDOUT_FILENO,buf,len);
                        write(arrep[i].data.fd,buf,len);
                    }    
                    if(len == 0){
                        printf("client closed ...\n");
                    }else if(len == -1){
                        if(errno == EAGAIN){
                            printf("data over ...\n");
                        }else{
                            perror("read");
                            exit(0);
                        }
                        
                    }
                    
                }
            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}
```

### 4.21 UDP

#### 4.21.1 通信

![image-20210408125658320](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408125658320.png)

```c
#include <sys/types.h> 
#include <sys/socket.h> 
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct 				sockaddr *dest_addr, socklen_t addrlen);
- 参数： 
    - sockfd : 通信的fd 
    - buf : 要发送的数据 
    - len : 发送数据的长度 
    - flags : 0 
    - dest_addr : 通信的另外一端的地址信息 
    - addrlen : 地址的内存大小
- 返回值：
    成功 返回发送的字节数
    失败 返回-1，设置错误号
        
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr 				*src_addr, socklen_t *addrlen);
- 参数：
    - sockfd : 通信的fd 
    - buf : 接收数据的数组
    - len : 数组的大小
    - flags : 0 
    - src_addr : 用来保存另外一端的地址信息，不需要可以指定为NULL
    - addrlen : 地址的内存大小
```

**UDP_server.c**

```c
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
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    int re = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(0);
    }

    // 3.通信
    while(1){
        // 接受数据
        char buf[128];
        char ipbuf[16];
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);
        int num = recvfrom(lfd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&len);
        if(num == -1){
            perror("recvfrom");
            exit(0);
        }

        printf("client IP  :  %s , Port  ： %d \n",
        inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ipbuf,sizeof(ipbuf)),
        ntohs(clientaddr.sin_port));

        printf("client say : %s\n",buf);

        sendto(lfd,buf,strlen(buf)+1,0,(struct sockaddr *)&clientaddr,sizeof(clientaddr));

    }
    close(lfd);
    
    return 0;
}
```

**UDP_client.c**

```c
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
```

#### 4.21.2 广播

> 向子网中多台计算机发送消息，并且子网中所有的计算机都可以接收到发送方发送的消息，每个广播消息都包含一个特殊的IP地址，这个IP中子网内主机标志部分的二进制全部为1。 
>
> a.只能在局域网中使用。
>
> b.客户端需要绑定服务器广播使用的端口，才可以接收到广播消息。

![image-20210408140250524](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408140250524.png)

```c
// 设置广播属性的函数 
int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen); 
	- sockfd : 文件描述符 
	- level : SOL_SOCKET 
	- optname : SO_BROADCAST 
	- optval : int类型的值，为1表示允许广播
	- optlen : optval的大小
```

**bro_server.c**

```c
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
```

**bro_client.c**

```c
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
```

#### 4.21.3 组播

> 单播地址标识单个 IP 接口，广播地址标识某个子网的所有 IP 接口，多播地址标识一组 IP 接口。单播和广播是寻址方案的两个极端（要么单个要么全部），多播则意在两者之间提供一种折中方案。多播数据报只应该由对它感兴趣的接口接收，也就是说由运行相应多播会话应用系统的主机上的接口接收。另外，广播一般局限于局域网内使用，而多播则既可以用于局域网，也可以跨广域网 使用。 
>
> a.组播既可以用于局域网，也可以用于广域网
> b.客户端需要加入多播组，才能接收到多播的数据

![image-20210408144116547](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408144116547.png)

组播地址

> IP 多播通信必须依赖于 IP 多播地址，在 IPv4 中它的范围从 224.0.0.0 到 239.255.255.255，并被划分为局部链接多播地址、预留多播地址和管理权限多播地址三类:

|          IP地址           |                             说明                             |
| :-----------------------: | :----------------------------------------------------------: |
|   224.0.0.0~224.0.0.255   | 局部链接多播地址：是为路由协议和其它用途保留的地址，路由 器并不转发属于此范围的IP包 |
|   224.0.1.0~224.0.1.255   |  预留多播地址：公用组播地址，可用于Internet；使用前需要申请  |
| 224.0.2.0~238.255.255.255 |  预留多播地址：用户可用组播地址(临时组地址)，全网范围内有效  |
| 239.0.0.0~239.255.255.255 | 本地管理组播地址，可供组织内部使用，类似于私有 IP 地址，不 能用于 Internet，可限制多播范围 |

**设置组播**

```c
int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
	// 服务器设置多播的信息，外出接口
    - level : IPPROTO_IP 
    - optname : IP_MULTICAST_IF 
    - optval : struct in_addr
    // 客户端加入到多播组： 
    - level : IPPROTO_IP 
    - optname : IP_ADD_MEMBERSHIP
    - optval : struct ip_mreq
    
struct ip_mreq {
        /* IP multicast address of group. */ 
        struct in_addr imr_multiaddr;      //组播的IP地址
        
        /* Local IP address of interface. */ 
        struct in_addr imr_interface;      //本地的IP地址
};

typedef uint32_t in_addr_t; 
struct in_addr {
	in_addr_t s_addr;
};
```

### 4.22 本地套接字

> 本地套接字的作用：本地的进程间通信 
>
> ​		有关系的进程间的通信 
>
> ​		没有关系的进程间的通信
>
> 本地套接字实现流程和网络套接字类似，一般呢采用TCP的通信流程。

![image-20210408151336754](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408151336754.png)

```c
// 本地套接字通信的流程 - tcp 
// 服务器端 
1. 创建监听的套接字 
	int lfd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
2. 监听的套接字绑定本地的套接字文件 -> server端 
	struct sockaddr_un addr; 
	// 绑定成功之后，指定的sun_path中的套接字文件会自动生成。 
	bind(lfd, addr, len);
3. 监听 
	listen(lfd, 100);
4. 等待并接受连接请求 
	struct sockaddr_un cliaddr; 
	int cfd = accept(lfd, &cliaddr, len);
5. 通信 
	接收数据：read/recv 
	发送数据：write/send
6. 关闭连接 close();


// 客户端的流程 
1. 创建通信的套接字 
	int fd = socket(AF_UNIX/AF_LOCAL, SOCK_STREAM, 0);
2. 监听的套接字绑定本地的IP端口 
	struct sockaddr_un addr; 
	// 绑定成功之后，指定的sun_path中的套接字文件会自动生成。 
	bind(lfd, addr, len);
3. 连接服务器 
	struct sockaddr_un serveraddr; 
	connect(fd, &serveraddr, sizeof(serveraddr));
4. 通信 
	接收数据：read/recv 
	发送数据：write/send
5. 关闭连接
	close();
```

```c
// 头文件: sys/un.h 
#define UNIX_PATH_MAX 108 
struct sockaddr_un { 
	sa_family_t sun_family; // 地址族协议 af_local 
	char sun_path[UNIX_PATH_MAX]; // 套接字文件的路径, 这是一个伪文件, 大小永远=0
};
```

![image-20210408153559679](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408153559679.png)

**ipc_server.c**

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(){
    unlink("server.sock");
    // 1.创建套接字
    int lfd = socket(AF_LOCAL,SOCK_STREAM,0);
    
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,"server.sock");
    int re = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(-1);
    }

    // 3.监听
    re = listen(lfd,8);
    if(re == -1){
        perror("listen");
        exit(-1);
    }

    // 4.等待客户端连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
    if( cfd == -1){
        perror("accept");
        exit(-1);
    }

    printf("client sockt filename : %s",cliaddr.sun_path);

    // 5.通信
    while(1){
        char buf[128];
        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("client closed ...\n");
            break;
        }else if(len > 0){
            printf("client says : %s \n",buf);
            send(cfd,buf,len,0);
        }

    }
    close(cfd);
    close(lfd);
    return 0;
}
```

**ipc_client.c**

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(){
    unlink("client.sock");
    // 1.创建套接字
    int cfd = socket(AF_LOCAL,SOCK_STREAM,0);
    
    if(cfd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path,"client.sock");
    int re = bind(cfd,(struct sockaddr *)&addr,sizeof(addr));
    if(re == -1){
        perror("bind");
        exit(-1);
    }

    // 3.连接服务器
    struct sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    strcpy(saddr.sun_path,"server.sock");
    re = connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(re == -1){
        perror("connect");
        exit(-1);
    }

    // 5.通信
    int num = 0;
    while(1){
        char buf[128];
        sprintf(buf,"hello I am client %d \n",num++);
        send(cfd,buf,strlen(buf)+1,0);

        printf("client says : %s \n",buf);

        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("server closed ...\n");
            break;
        }else if(len > 0){
            printf("server says : %s \n",buf);   
        }
        sleep(1);
    }
    close(cfd);
    return 0;
}
```

## 5. 项目实战

### 5.1 阻塞/非阻塞、 同步/异步（网络I/O）

> 典型的一次IO的两个阶段是什么？数据就绪 和 数据读写

数据就绪：根据系统IO操作的就绪状态

- 阻塞 
- 非阻塞

数据读写：根据应用程序和内核的交互方式

- 同步 
- 异步

陈硕：在处理 IO 的时候，阻塞和非阻塞都是同步 IO，只有使用了特殊的 API 才是异步 IO。

<img src="https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408213706136.png" alt="image-20210408213706136" style="zoom:50%;" />

![image-20210408215009519](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210408215009519.png)

> 一个典型的网络IO接口调用，分为两个阶段，分别是“数据就绪” 和 “数据读写”，数据就绪阶段分为阻塞和非阻塞，表现得结果就是，阻塞当前线程或是直接返回。
>
> 同步表示A向B请求调用一个网络IO接口时（或者调用某个业务逻辑API接口时），数据的读写都是由请求方A自己来完成的（不管是阻塞还是非阻塞）；异步表示A向B请求调用一个网络IO接口时（或者调用某个业务逻辑API接口时），向B传入请求的事件以及事件发生时通知的方式，A就可以处理其它逻辑了，当B监听到事件处理完成后，会用事先约定好的通知方式，通知A处理结果。

- 同步阻塞 
- 同步非阻塞 
- 异步阻塞
- 异步非阻塞

### 5.2 Unix/Linux上的五种IO模型

#### 5.2.1 阻塞 blocking

调用者调用了某个函数，等待这个函数返回，期间什么也不做，不停的去检查这个函数有没有返回，必须等这个函数返回才能进行下一步动作。

![image-20210409090112224](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409090112224.png)

#### 5.2.2 非阻塞 non-blocking（NIO）

非阻塞等待，每隔一段时间就去检测IO事件是否就绪。没有就绪就可以做其他事。非阻塞I/O执行系统调用总是立即返回，不管事件是否已经发生，若事件没有发生，则返回-1，此时可以根据 errno 区分这两种情况，对于accept，recv 和 send，事件未发生时，errno 通常被设置成 EAGAIN。

![image-20210409090518303](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409090518303.png)

#### 5.2.3 IO复用（IO multiplexing）

Linux 用 select/poll/epoll 函数实现 IO 复用模型，这些函数也会使进程阻塞，但是和阻塞IO所不同的是这些函数可以同时阻塞多个IO操作。而且可以同时对多个读操作、写操作的IO函数进行检测。直到有数据可读或可写时，才真正调用IO操作函数。

![image-20210409090941858](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409090941858.png)

#### 5.2.4 信号驱动（signal-driven）

Linux 用套接口进行信号驱动 IO，安装一个信号处理函数，进程继续运行并不阻塞，当IO事件就绪，进程收到SIGIO 信号，然后处理 IO 事件。

![image-20210409091329173](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409091329173.png)

内核在第一个阶段是异步，在第二个阶段是同步；与非阻塞IO的区别在于它提供了消息通知机制，不需要用户进程不断的轮询检查，减少了系统API的调用次数，提高了效率。

#### 5.2.5 异步（asynchronous）

Linux中，可以调用 aio_read 函数告诉内核描述字缓冲区指针和缓冲区的大小、文件偏移及通知的方式，然后立即返回，当内核将数据拷贝到缓冲区后，再通知应用程序。

![image-20210409091713541](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409091713541.png)

### 5.3 Web Server （网页服务器）

一个 Web Server 就是一个服务器软件（程序），或者是运行这个服务器软件的硬件（计算机）。其主要功能是通过 HTTP 协议与客户端（通常是浏览器（Browser））进行通信，来接收，存储，处理来自客户端的 HTTP 请求，并对其请求做出 HTTP 响应，返回给客户端其请求的内容（文件、网页等）或返回一个 Error 信息。

![image-20210409094819223](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409094819223.png)

通常用户使用 Web 浏览器与相应服务器进行通信。在浏览器中键入“域名”或“IP地址:端口号”，浏览器则先将你的域名解析成相应的 IP 地址或者直接根据你的IP地址向对应的 Web 服务器发送一个 HTTP 请求。这一过程首先要通过 TCP 协议的三次握手建立与目标 Web 服务器的连接，然后 HTTP 协议生成针对目标 Web 服务器的 HTTP 请求报文，通过 TCP、IP 等协议发送到目标 Web 服务器上。

### 5.4 HTTP协议(应用层的协议)   

**简介**

超文本传输协议（Hypertext Transfer Protocol，HTTP）是一个简单的请求 - 响应协议，它通常运行在TCP之上。它指定了客户端可能发送给服务器什么样的消息以及得到什么样的响应。请求和响应消息的头以ASCII 形式给出；而消息内容则具有一个类似 MIME 的格式。HTTP是万维网的数据通信的基础。

HTTP的发展是由蒂姆·伯纳斯-李于1989年在欧洲核子研究组织（CERN）所发起。HTTP的标准制定由万维网协会（World Wide Web Consortium，W3C）和互联网工程任务组（Internet Engineering Task Force,IETF）进行协调，最终发布了一系列的RFC，其中最著名的是1999年6月公布的 RFC 2616，定义了HTTP协议中现今广泛使用的一个版本——HTTP 1.1。

**概述**

HTTP 是一个客户端终端（用户）和服务器端（网站）请求和应答的标准（TCP）。通过使用网页浏览器、网络爬虫或者其它的工具，客户端发起一个HTTP请求到服务器上指定端口（默认端口为80）。我们称这个客户端为用户代理程序（user agent）。应答的服务器上存储着一些资源，比如 HTML 文件和图像。我们称这个应答服务器为源服务器（origin server）。在用户代理和源服务器中间可能存在多个“中 间层”，比如代理服务器、网关或者隧道（tunnel）。

尽管 TCP/IP 协议是互联网上最流行的应用，HTTP 协议中，并没有规定必须使用它或它支持的层。事实 上，HTTP可以在任何互联网协议上，或其他网络上实现。HTTP 假定其下层协议提供可靠的传输。因此，任何能够提供这种保证的协议都可以被其使用。因此也就是其在 TCP/IP 协议族使用 TCP 作为其传输层。

通常，由HTTP客户端发起一个请求，创建一个到服务器指定端口（默认是80端口）的 TCP 连接。HTTP 服务器则在那个端口监听客户端的请求。一旦收到请求，服务器会向客户端返回一个状态，比如"HTTP/1.1 200 OK"，以及返回的内容，如请求的文件、错误消息、或者其它信息。

**工作原理**

HTTP 协议定义 Web 客户端如何从 Web 服务器请求 Web 页面，以及服务器如何把 Web 页面传送给客户端。HTTP 协议采用了请求/响应模型。客户端向服务器发送一个请求报文，请求报文包含请求的方法、URL、协议版本、请求头部和请求数据。服务器以一个状态行作为响应，响应的内容包括协议的版本、成功或者错误代码、服务器信息、响应头部和响应数据。

以下是 HTTP 请求/响应的步骤：

1. 客户端连接到 Web 服务器 

   一个HTTP客户端，通常是浏览器，与 Web 服务器的 HTTP 端口（默认为 80 ）建立一个 TCP 套接字连接。例如，http://www.baidu.com。（URL）

2. 发送 HTTP 请求 

   通过 TCP 套接字，客户端向 Web 服务器发送一个文本的请求报文，一个请求报文由请求行、请求头部、空行和请求数据 4 部分组成。

3. 服务器接受请求并返回 HTTP 响应 

   Web 服务器解析请求，定位请求资源。服务器将资源复本写到 TCP 套接字，由客户端读取。一个响应由状态行、响应头部、空行和响应数据 4 部分组成。

4. 释放连接 TCP 连接 

  若 connection 模式为 close，则服务器主动关闭 TCP连接，客户端被动关闭连接，释放 TCP 连接；若connection 模式为 keepalive，则该连接会保持一段时间，在该时间内可以继续接收请求; 

5. 客户端浏览器解析 HTML 内容
    客户端浏览器首先解析状态行，查看表明请求是否成功的状态代码。然后解析每一个响应头，响应 头告知以下为若干字节的 HTML 文档和文档的字符集。客户端浏览器读取响应数据 HTML，根据HTML 的语法对其进行格式化，并在浏览器窗口中显示。

例如：在浏览器地址栏键入URL，按下回车之后会经历以下流程：

1. 浏览器向 DNS 服务器请求解析该 URL 中的域名所对应的 IP 地址; 
2. 解析出 IP 地址后，根据该 IP 地址和默认端口 80，和服务器建立 TCP 连接; 
3. 浏览器发出读取文件（ URL 中域名后面部分对应的文件）的 HTTP 请求，该请求报文作为 TCP 三 次握手的第三个报文的数据发送给服务器;
4. 服务器对浏览器请求作出响应，并把对应的 HTML 文本发送给浏览器; 
5. 释放 TCP 连接;
6. 浏览器将该 HTML 文本并显示内容。

![image-20210409101443981](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409101443981.png)

HTTP 协议是基于 TCP/IP 协议之上的应用层协议，基于 请求-响应 的模式。HTTP 协议规定，请求从客户端发出，最后服务器端响应该请求并返回。换句话说，肯定是先从客户端开始建立通信的，服务器端在没有接收到请求之前不会发送响应。

**HTTP 请求报文格式**

![image-20210409101524518](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409101524518.png)

GET / HTTP/1.1
Host: www.baidu.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Cookie: BAIDUID=6267A15A8E84CD0FF837D5BE9B4C6DAA:FG=1; BIDUPSID=6267A15A8E84CD0F6BC2627A67B439A6; PSTM=1617937250; BDRCVFR[Fc9oatPmwxn]=aeXf-1x8UdYcs; BD_HOME=1; H_PS_PSSID=33796_33241_33749_33272_31660_33714_26350; BD_UPN=13314752; BA_HECTOR=0580848h018h2ha5mp1g6vgr40r
Upgrade-Insecure-Requests: 1
Cache-Control: max-age=0

**HTTP响应报文格式**

![image-20210409101716935](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409101716935.png)

HTTP/1.1 200 OK
Bdpagetype: 1
Bdqid: 0xa19b40770000849a
Cache-Control: private
Connection: keep-alive
Content-Encoding: gzip
Content-Type: text/html;charset=utf-8
Date: Fri, 09 Apr 2021 03:01:03 GMT
Expires: Fri, 09 Apr 2021 03:01:03 GMT
Server: BWS/1.1
Set-Cookie: BDSVRTM=12; path=/
Set-Cookie: BD_HOME=1; path=/
Set-Cookie: H_PS_PSSID=33796_33241_33749_33272_31660_33714_26350; path=/; domain=.baidu.com
Strict-Transport-Security: max-age=172800
Traceid: 1617937263062388225011644972141341869210
X-Ua-Compatible: IE=Edge,chrome=1
Transfer-Encoding: chunked



**HTTP请求方法**

HTTP/1.1 协议中共定义了八种方法（也叫“动作”）来以不同方式操作指定的资源：

1. GET：向指定的资源发出“显示”请求。使用 GET 方法应该只用在读取数据，而不应当被用于产生“副作用”的操作中，例如在 Web Application 中。其中一个原因是 GET 可能会被网络蜘蛛等随意访问。
2. HEAD：与 GET 方法一样，都是向服务器发出指定资源的请求。只不过服务器将不传回资源的本文部分。它的好处在于，使用这个方法可以在不必传输全部内容的情况下，就可以获取其中“关于该资源的信息”（元信息或称元数据）。
3. POST：向指定资源提交数据，请求服务器进行处理（例如提交表单或者上传文件）。数据被包含在请求本文中。这个请求可能会创建新的资源或修改现有资源，或二者皆有。
4. PUT：向指定资源位置上传其最新内容。 
5. DELETE：请求服务器删除 Request-URI 所标识的资源。 
6. TRACE：回显服务器收到的请求，主要用于测试或诊断。 
7. OPTIONS：这个方法可使服务器传回该资源所支持的所有 HTTP 请求方法。用'*'来代替资源名称， 向 Web 服务器发送 OPTIONS 请求，可以测试服务器功能是否正常运作。
8. CONNECT：HTTP/1.1 协议中预留给能够将连接改为管道方式的代理服务器。通常用于SSL加密服务器的链接（经由非加密的 HTTP 代理服务器）。

**HTTP状态码**

所有HTTP响应的第一行都是状态行，依次是当前HTTP版本号，3位数字组成的状态代码，以及描述状态的短语，彼此由空格分隔。
状态代码的第一个数字代表当前响应的类型：

- 1xx消息——请求已被服务器接收，继续处理 
- 2xx成功——请求已成功被服务器接收、理解、并接受 
- 3xx重定向——需要后续操作才能完成这一请求 
- 4xx请求错误——请求含有词法错误或者无法被执行
- 5xx服务器错误——服务器在处理某个正确请求时发生错误

虽然 RFC 2616 中已经推荐了描述状态的短语，例如"200 OK"，"404 Not Found"，但是WEB开发者仍然能够自行决定采用何种短语，用以显示本地化的状态描述或者自定义信息。

![image-20210409112203048](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409112203048.png)

更多状态码：https://baike.baidu.com/item/HTTP%E7%8A%B6%E6%80%81%E7%A0%81/5053660?fr=aladdin

### 5.5 服务器编程的基本框架

虽然服务器程序种类繁多，但其基本框架都一样，不同之处在于逻辑处理。

![image-20210409113037957](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409113037957.png)

|     模块     |            功能            |
| :----------: | :------------------------: |
| I/O 处理单元 | 处理客户连接，读写网络数据 |
|   逻辑单元   |       业务进程或线程       |
| 网络存储单元 |     数据库、文件或缓存     |
|   请求队列   |    各单元之间的通信方式    |

I/O 处理单元是服务器管理客户连接的模块。它通常要完成以下工作：等待并接受新的客户连接，接收客户数据，将服务器响应数据返回给客户端。但是数据的收发不一定在 I/O 处理单元中执行，也可能在逻辑单元中执行，具体在何处执行取决于事件处理模式。

一个逻辑单元通常是一个进程或线程。它分析并处理客户数据，然后将结果传递给 I/O 处理单元或者直接发送给客户端（具体使用哪种方式取决于事件处理模式）。服务器通常拥有多个逻辑单元，以实现对多个客户任务的并发处理。

网络存储单元可以是数据库、缓存和文件，但不是必须的。

请求队列是各单元之间的通信方式的抽象。I/O 处理单元接收到客户请求时，需要以某种方式通知一个逻辑单元来处理该请求。同样，多个逻辑单元同时访问一个存储单元时，也需要采用某种机制来协调处理竞态条件。请求队列通常被实现为池的一部分。

**两种高效的事件处理模式**

服务器程序通常需要处理三类事件：I/O 事件、信号及定时事件。有两种高效的事件处理模式Reactor 和Proactor，同步 I/O 模型通常用于实现 Reactor 模式，异步 I/O 模型通常用于实现 Proactor 模式。

**Reactor模式**

要求主线程（I/O处理单元）只负责监听文件描述符上是否有事件发生，有的话就立即将该事件通知工作线程（逻辑单元），将 socket 可读可写事件放入请求队列，交给工作线程处理。除此之外，主线程不做任何其他实质性的工作。读写数据，接受新的连接，以及处理客户请求均在工作线程中完成。

使用同步 I/O（以 epoll_wait 为例）实现的 Reactor 模式的工作流程是：

1. 主线程往 epoll 内核事件表中注册 socket 上的读就绪事件。 
2. 主线程调用 epoll_wait 等待 socket 上有数据可读。
3. 当 socket 上有数据可读时， epoll_wait 通知主线程。主线程则将 socket 可读事件放入请求队列。
4. 睡眠在请求队列上的某个工作线程被唤醒，它从 socket 读取数据，并处理客户请求，然后往 epoll 内核事件表中注册该 socket 上的写就绪事件。
5. 当主线程调用 epoll_wait 等待 socket 可写。 
6. 当 socket 可写时，epoll_wait 通知主线程。主线程将 socket 可写事件放入请求队列。
7. 睡眠在请求队列上的某个工作线程被唤醒，它往 socket 上写入服务器处理客户请求的结果。

![image-20210409131059303](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409131059303.png)

**Proactor模式**

Proactor 模式将所有 I/O 操作都交给主线程和内核来处理（进行读、写），工作线程仅仅负责业务逻辑。使用异步 I/O 模型（以 aio_read 和 aio_write 为例）实现的 Proactor 模式的工作流程是：

1. 主线程调用 aio_read 函数向内核注册 socket 上的读完成事件，并告诉内核用户读缓冲区的位置，以及读操作完成时如何通知应用程序（这里以信号为例）。
2. 主线程继续处理其他逻辑。 
3. 当 socket 上的数据被读入用户缓冲区后，内核将向应用程序发送一个信号，以通知应用程序数据已经可用。
4. 应用程序预先定义好的信号处理函数选择一个工作线程来处理客户请求。工作线程处理完客户请求 后，调用 aio_write 函数向内核注册 socket 上的写完成事件，并告诉内核用户写缓冲区的位置，以及写操作完成时如何通知应用程序。
5. 主线程继续处理其他逻辑。 
6. 当用户缓冲区的数据被写入 socket 之后，内核将向应用程序发送一个信号，以通知应用程序数据 已经发送完毕。
7. 应用程序预先定义好的信号处理函数选择一个工作线程来做善后处理，比如决定是否关闭 socket。

![image-20210409131355069](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409131355069.png)

**模拟 Proactor 模式**

使用同步 I/O 方式模拟出 Proactor 模式。原理是：主线程执行数据读写操作，读写完成之后，主线程向工作线程通知这一”完成事件“。那么从工作线程的角度来看，它们就直接获得了数据读写的结果，接下来要做的只是对读写的结果进行逻辑处理。

使用同步 I/O 模型（以 epoll_wait为例）模拟出的 Proactor 模式的工作流程如下：

1. 主线程往 epoll 内核事件表中注册 socket 上的读就绪事件。 
2. 主线程调用 epoll_wait 等待 socket 上有数据可读。 
3. 当 socket 上有数据可读时，epoll_wait 通知主线程。主线程从 socket 循环读取数据，直到没有更 多数据可读，然后将读取到的数据封装成一个请求对象并插入请求队列。
4. 睡眠在请求队列上的某个工作线程被唤醒，它获得请求对象并处理客户请求，然后往 epoll 内核事 件表中注册 socket 上的写就绪事件。
5. 主线程调用 epoll_wait 等待 socket 可写。
6. 当 socket 可写时，epoll_wait 通知主线程。主线程往 socket 上写入服务器处理客户请求的结果。

![image-20210409132036931](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409132036931.png)

### 5.6 线程的同步机制类封装以及线程池的实现

#### 5.6.1 线程池

线程池是由服务器预先创建的一组子线程，线程池中的线程数量应该和 CPU 数量差不多。线程池中的所有子线程都运行着相同的代码。当有新的任务到来时，主线程将通过某种方式选择线程池中的某一个子线程来为之服务。相比与动态的创建子线程，选择一个已经存在的子线程的代价显然要小得多。至于主线程选择哪个子线程来为新任务服务，则有多种方式：

- 主线程使用某种算法来主动选择子线程。最简单、最常用的算法是随机算法和 Round Robin（轮流选取）算法，但更优秀、更智能的算法将使任务在各个工作线程中更均匀地分配，从而减轻服务器的整体压力。
- 主线程和所有子线程通过一个共享的工作队列来同步，子线程都睡眠在该工作队列上。当有新的任务到来时，主线程将任务添加到工作队列中。这将唤醒正在等待任务的子线程，不过只有一个子线程将获得新任务的”接管权“，它可以从工作队列中取出任务并执行之，而其他子线程将继续睡眠在工作队列上。

![image-20210409154248210](https://raw.githubusercontent.com/wenchenwan/CloudPic/master/img/image-20210409154248210.png)

> 线程池中的线程数量最直接的限制因素是中央处理器(CPU)的处理器(processors/cores)的数量 N：如果你的CPU是4-cores的，对于CPU密集型的任务(如视频剪辑等消耗CPU计算资源的任务)来 说，那线程池中的线程数量最好也设置为4（或者+1防止其他因素造成的线程阻塞）；对于IO密集 型的任务，一般要多于CPU的核数，因为线程间竞争的不是CPU的计算资源而是IO，IO的处理一 般较慢，多于cores数的线程将为CPU争取更多的任务，不至在线程处理IO的过程造成CPU空闲导
> 致资源浪费。

- 空间换时间，浪费服务器的硬件资源，换取运行效率。 
- 池是一组资源的集合，这组资源在服务器启动之初就被完全创建好并初始化，这称为静态资源。 
- 当服务器进入正式运行阶段，开始处理客户请求的时候，如果它需要相关的资源，可以直接从池中获取，无需动态分配。
- 当服务器处理完一个客户连接后，可以把相关的资源放回池中，无需执行系统调用释放资源。

#### 5.6.2 有限状态机

逻辑单元内部的一种高效编程方法：有限状态机（finite state machine）。 

有的应用层协议头部包含数据包类型字段，每种类型可以映射为逻辑单元的一种执行状态，服务器可以
根据它来编写相应的处理逻辑。如下是一种状态独立的有限状态机：

```c
STATE_MACHINE( Package _pack ) {
PackageType _type = _pack.GetType(); 
switch( _type ) {
case type_A: 
	process_package_A( _pack ); 
	break;
case type_B: 
    process_package_B( _pack ); 
    break;
	}
}
```

这是一个简单的有限状态机，只不过该状态机的每个状态都是相互独立的，即状态之间没有相互转移。 状态之间的转移是需要状态机内部驱动，如下代码：

```c++
STATE_MACHINE() 
{
	State cur_State = type_A;
    while( cur_State != type_C ) {
	Package _pack = getNewPackage(); 
    switch( cur_State ) {
		case type_A: process_package_state_A( _pack ); 
            cur_State = type_B; 
            break;
		case type_B: process_package_state_B( _pack ); 
            cur_State = type_C; 
            break;
		} 	
    }
}
```

该状态机包含三种状态：type_A、type_B 和 type_C，其中 type_A 是状态机的开始状态，type_C 是状态机的结束状态。状态机的当前状态记录在 cur_State 变量中。在一趟循环过程中，状态机先通过getNewPackage 方法获得一个新的数据包，然后根据 cur_State 变量的值判断如何处理该数据包。数据包处理完之后，状态机通过给 cur_State 变量传递目标状态值来实现状态转移。那么当状态机进入下一趟循环时，它将执行新的状态对应的逻辑。

#### 5.6.3 EPOLLONESHOT事件

即使可以使用ET模式，一个socket 上的某个事件还是可能被触发多次。这在并发程序中就会引起一个问题。比如一个线程在读取完某个 socket 上的数据后开始处理这些数据，而在数据的处理过程中该 socket上又有新数据可读（EPOLLIN 再次被触发），此时另外一个线程被唤醒来读取这些新的数据。于是就出现了两个线程同时操作一个 socket 的局面。一个socket连接在任一时刻都只被一个线程处理，可以使用 epoll 的 EPOLLONESHOT 事件实现。

#### 5.6.4 服务器压力测试

Webbench 是 Linux 上一款知名的、优秀的 web 性能压力测试工具。它是由Lionbridge公司开发。

> - 测试处在相同硬件上，不同服务的性能以及不同硬件上同一个服务的运行状况。
> -  展示服务器的两项内容：每秒钟响应请求数和每秒钟传输数据量。

基本原理：Webbench 首先 fork 出多个子进程，每个子进程都循环做 web 访问测试。子进程把访问的结果通过pipe 告诉父进程，父进程做最终的统计结果。

测试示例

```c
webbench -c 1000 -t 30  http://192.168.110.129:10000/index.html

参数： 
	-c 表示客户端数
	-t 表示时间
```

