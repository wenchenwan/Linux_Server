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

    printf("val : %d, byte : %d  %d \n",test.val,test.byte[0],test.byte[1]);
    return 0;
}