#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

static SOCKET Udp;
static int TotalGVSPPackSize;
static int OnePackLen;
static int FristPackSize = 44;
static int LastPackSize = 18;
static int W = 3072;
static int H = 2048;
int udp_init(char *ip, int port,int onePackLen)
{
    OnePackLen = onePackLen;
    int packCount = (W*H)/onePackLen;
    if((W*H)%onePackLen != 0)
        packCount += 1;
    TotalGVSPPackSize = (W*H) + (packCount*8) +FristPackSize + LastPackSize;
    struct sockaddr_in ser; //服务器端地址
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock.\n"); //Winsock 初始化错误
        return -1;
    }
    ser.sin_family = AF_INET;                       //初始化服务器地址信息
    ser.sin_port = htons(port);                     //端口转换为网络字节序
    ser.sin_addr.s_addr = inet_addr(ip);            //IP 地址转换为网络字节序
    Udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //创建UDP套接字
    if (Udp == INVALID_SOCKET)
    {
        printf("socket() Failed: %d\n", WSAGetLastError());
        return -1;
    }
    if (bind(Udp, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)
    {
        printf("绑定IP和端口\n");
        return 0;
    }
    printf("udp init ok\n");
}

int udp_rcv( char *buff)
{
    struct sockaddr_in s;
    int ret,addrlen;
    int totalLen = 0;
    addrlen = sizeof(s);
    struct sockaddr_in Addr;
    int iAddrLength;
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
    //    Addr.sin_port = htons(3956);
    Addr.sin_port = htons(54199);
    iAddrLength = sizeof(Addr);
    ret = recvfrom(Udp, ((char*)buff) , 44, 0, (SOCKADDR *)&Addr, &iAddrLength);
    totalLen = totalLen+ret;
    int packIndex;
    while(totalLen < TotalGVSPPackSize){
        ret = recvfrom(Udp, ((char*)buff)+ totalLen, 1472, 0, (SOCKADDR *)&Addr, &iAddrLength);
        packIndex = (int)((unsigned char)(buff+ totalLen)[7]  |
                (unsigned char)(buff+ totalLen)[6] << 8 |(unsigned char)(buff+ totalLen)[5] << 16);
        totalLen = totalLen+ret;
        if(packIndex == 4297)
            break;
    }
    ret = recvfrom(Udp, ((char*)buff)+ totalLen, 656, 0, (SOCKADDR *)&Addr, &iAddrLength);
    totalLen = totalLen+ret;
    ret = recvfrom(Udp, ((char*)buff)+ totalLen, 18, 0, (SOCKADDR *)&Addr, &iAddrLength);
    totalLen = totalLen+ret;
    return totalLen;
}

int close_udp()
{
    closesocket(Udp); //关闭 socket
    WSACleanup();
    return 1;
}

#endif // UDPCLIENT_H
