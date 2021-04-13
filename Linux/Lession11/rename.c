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