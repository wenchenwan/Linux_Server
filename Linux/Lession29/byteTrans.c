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