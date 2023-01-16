/*
 *
 * Corence is pleased to support the open source community by making
 * GvcpServer available.
 *
 * Copyright (C) 2020 CORENCE AI SENSOR limited,a ningbo company.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "define.h"

char m_szLocalIp[32] = "172.16.10.20";
char m_szLocalMask[32] = "255.255.255.0";
char m_szLocalGateway[32] = "172.16.10.1";
char m_szLocalMac[32];
uint32 m_dwLocalPort = 3956;
uint8 m_LocalMacAddr[ETH_ALEN];
char m_szRemoteIp[32];
char m_szRemoteMask[32];
char m_szRemoteGateway[32];
char m_szRemoteMac[32] ={"34:BD:20:10:E8:20"};
uint32 m_dwRemotePort;
uint8 m_RemoteMacAddr[ETH_ALEN];
uint32 m_dwBroadcast = 0;
int disId = 1,writeId = 1,readId = 12345;
int iFd;

int gvcp_ack_discover(int iFd, char* szIp, char* szMask, char* szGateway, uint16 wReqID, uint32 dwPort, uint8* pMac)
{
    //char rgMessage[128] = "I am sending message to you!";
    //int iFd;
    int iSendbytes;
    int iOptval = 1;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (char*)&iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt failed!");
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    struct gvcp_discover_ack ack_msg;
    memset(&ack_msg, 0, sizeof(struct gvcp_discover_ack));
    ack_msg.header.wStatus = htons(0);
    ack_msg.header.wAck = htons(GVCP_DISCOVERY_ACK);
    ack_msg.header.wLen = htons(sizeof(struct gvcp_ack_payload));
    ack_msg.header.wReqID = htons(1);
    ack_msg.payload.dwSpecVer = htonl(0x010002);;
    ack_msg.payload.dwDevMode = htonl(1);
    //uint8 MyMac[6]={0xc4,0x2f,0x90,0xf1,0x71,0x3e};
    memcpy(&ack_msg.payload.Mac[2], m_LocalMacAddr, 6);
    ack_msg.payload.dwSupIpSet = htonl(0x80000007);
    ack_msg.payload.dwCurIpSet = htonl(0x00005);
    //uint8 unused1[12];
    *((uint32*)&ack_msg.payload.CurIP[12]) = inet_addr(m_szLocalIp);//last 4 byte
    *((uint32*)&ack_msg.payload.SubMask[12]) = inet_addr(m_szLocalMask);//last 4 byte
    *((uint32*)&ack_msg.payload.Gateway[12]) = inet_addr(m_szLocalGateway);//last 4 byte
    strcpy(ack_msg.payload.szFacName, "Hikrobot");//first
    strcpy(ack_msg.payload.szModelName, "MV-CB060-10GM-S");//first
    strcpy(ack_msg.payload.szDevVer, "V3.4.20 201014 513000");
    strcpy(ack_msg.payload.szFacInfo, "Hikrobot");
    strcpy(ack_msg.payload.szSerial, "K75650174");
    strcpy(ack_msg.payload.szUserName, "");
    char* rgMessage = (char*)&ack_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_discover_ack);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno="<< strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_ack_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<<iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}

int gvcp_cmd_discover(int iFd)
{
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    int iOptval = 1;

    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_discover_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_discover_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x11;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_DISCOVERY_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(0);//payload length
    cmd_msg.header.wReqID = htons(1);// request id = 1;READREG id=12345

    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_discover_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}

int gvcp_ask_readreg(int iFd, uint16 wReqID, uint32 dwRegAddr)
{
    //char rgMessage[128] = "I am sending message to you!";
    //int iFd;
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    int iOptval = 1;
#if 1
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt SO_REUSEADDR failed!");
    }
    BOOL	bBroadcast = true;
    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL)) < 0)
    {
        qDebug()<<("setsockopt SO_BROADCAST failed!");
    }
#else
    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt failed!");
    }
#endif
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_readreg_cmd my_msg;
    memset(&my_msg, 0, sizeof(struct gvcp_readreg_cmd));
    my_msg.header.cMsgKeyCode = 0x42;
    my_msg.header.cFlag = 0x11;//0x11 allow broadcast ack;ack required
    my_msg.header.wCmd = htons(GVCP_READREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    my_msg.header.wLen = htons(0);//payload length
    my_msg.header.wReqID = htons(12345);// request id = 1;READREG id=12345
    my_msg.dwRegAddr = dwRegAddr;

    char* rgMessage = (char*)&my_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_readreg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno=%d,%s"<<errno<< strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_ask_readreg=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}

int gvcp_ask_forceip(int iFd, uint16 wReqID, uint32 dwPort)
{
    //char rgMessage[128] = "I am sending message to you!";
    //int iFd;
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    int iOptval = 1;
#if 1
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt SO_REUSEADDR failed!");
    }
    BOOL	bBroadcast = true;
    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL)) < 0)
    {
        qDebug()<<("setsockopt SO_BROADCAST failed!");
    }
#else
    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt failed!");
    }
#endif
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_forceip_ack my_msg;
    memset(&my_msg, 0, sizeof(struct gvcp_forceip_ack));
    my_msg.header.wStatus = htons(0);
    my_msg.header.wAck = htons(GVCP_FORCEIP_ACK);
    my_msg.header.wLen = htons(0);
    my_msg.header.wReqID = htons(wReqID);

    char* rgMessage = (char*)&my_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_forceip_ack);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno=%d,%s"<< errno<< strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_ask_forceip=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}

int gvcp_ask_writereg(int iFd, uint16 wReqID,int d )
{
    //char rgMessage[128] = "I am sending message to you!";
    //int iFd;
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    int iOptval = 1;
#if 1
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt SO_REUSEADDR failed!");
    }
#endif
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x11;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(wReqID);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = uint32(1610810112);
    cmd_msg.dwRegValue = uint32(786432);
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno=%d,%s"<< errno<<strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_ask_writereg=%s, rgMessageLen=%d,iSendbytes=%d"<<rgMessage<<dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}

void recvData(int fd){
    //sleep(200);
    char rgMessage[2014];
    struct sockaddr_in Addr;
    int iAddrLength;
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
//    Addr.sin_port = htons(3956);
    Addr.sin_port = htons(54199);
    iAddrLength = sizeof(Addr);
    int iRecvLen = 0;
    if ((iRecvLen = recvfrom(fd, rgMessage, sizeof(rgMessage), 0, (struct sockaddr*)&Addr, &iAddrLength)) == -1)
    {
        qDebug()<<("recv failed!");
    }
    qDebug()<<"recv len =%d"<< iRecvLen;
    unsigned short* pBuf = (unsigned short*)rgMessage;
    if (iRecvLen >= 8)
    {
        qDebug()<<"recv from "<<inet_ntoa(Addr.sin_addr)<< ntohs(Addr.sin_port)<<ntohs(pBuf[0])<< ntohs(pBuf[1])<< ntohs(pBuf[2])<< ntohs(pBuf[3]);

        struct gvcp_cmd_header* pHeader = (struct gvcp_cmd_header*)rgMessage;
        static struct gvcp_cmd_header m_CmdHeader;
        m_CmdHeader.cMsgKeyCode = pHeader->cMsgKeyCode;// 0x42;
        m_CmdHeader.cFlag = pHeader->cFlag;//0x11 allow broadcast ack;ack required
        m_CmdHeader.wCmd = ntohs(pHeader->wCmd);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
        m_CmdHeader.wLen = ntohs(pHeader->wLen);//payload length
        m_CmdHeader.wReqID = ntohs(pHeader->wReqID);// request id = 1;READREG id=12345
        if(m_CmdHeader.wCmd == GVCP_DISCOVERY_ACK){
            QByteArray arr((char *)pBuf,iRecvLen*2);
            qDebug()<<QString::fromLatin1(arr,iRecvLen*2);
            qDebug()<<arr;
            disId = m_CmdHeader.wReqID;
        }else if(m_CmdHeader.wCmd == GVCP_WRITEREG_ACK){
            QByteArray arr((char *)pBuf,iRecvLen*2);
            qDebug()<<QString::fromLatin1(arr,iRecvLen*2);
            qDebug()<<arr;
            writeId = m_CmdHeader.wReqID;
        }else if(m_CmdHeader.wCmd == GVCP_READREG_ACK){
            QByteArray arr((char *)pBuf,iRecvLen*2);
            int data = 0;
            data = ((char *)pBuf)[10] ;
            data = data |  ((char *)pBuf)[11]<< 8;
            qDebug()<<data;
            qDebug()<<arr;
            readId = m_CmdHeader.wReqID;
        }
    }
}

void  interfaceDiscover(){
    gvcp_cmd_discover(iFd);
    recvData(iFd);
}

void  interfaceReadCtrPri(){
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_readreg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_readreg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_READREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(4);//payload length
    cmd_msg.header.wReqID = htons(12345);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 655360;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_readreg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetCtrPri(){
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0xa0000;
    cmd_msg.dwRegValue = 0x2000000;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetDataChannelIp(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x100b0000;
    cmd_msg.dwRegValue = 0x140a10ac;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetDataChannelPort(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0xb0000;
    cmd_msg.dwRegValue = 0xB8D30000;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetStreamChannelIp(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x180d0000;
    cmd_msg.dwRegValue = 0x140a10ac;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetStreamChannelPort(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x000d0000;
    cmd_msg.dwRegValue = 0xB7D30000;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetStreamChannelSize(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x040d0000;
    cmd_msg.dwRegValue = 0xDC050040;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceCapture(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x80080300;
    cmd_msg.dwRegValue = 0x6000000;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceCapture1(){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x04080300;
    cmd_msg.dwRegValue = 0x80;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void interfaceSetDeviceMode(int mode){
    int iSendbytes;
    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return ;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_writereg_cmd cmd_msg;
    memset(&cmd_msg, 0, sizeof(struct gvcp_writereg_cmd));
    cmd_msg.header.cMsgKeyCode = 0x42;
    cmd_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    cmd_msg.header.wCmd = htons(GVCP_WRITEREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    cmd_msg.header.wLen = htons(8);//payload length
    cmd_msg.header.wReqID = htons(writeId += 1);// request id = 1;READREG id=12345
    cmd_msg.dwRegAddr = 0x40080300;
    if(mode == 0)
        cmd_msg.dwRegValue = 0x40000000;
    else
        cmd_msg.dwRegValue = 0x00000000;
    char* rgMessage = (char*)&cmd_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_writereg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<__FUNCDNAME__<<__LINE__<<"sendto fail, errno=%d,%s"<<strerror(errno);
            return ;
        }
        qDebug()<<"gvcp_cmd_discover=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }
    recvData(iFd);
}

void  interfaceWriteReg(int d){
    gvcp_ask_writereg(iFd,writeId,d);
    recvData(iFd);
}

void  interfaceReadReg(){
    gvcp_ask_readreg(iFd, readId,1610810112);
    recvData(iFd);

}

void  interfaceReadReg1(){
    gvcp_ask_readreg(iFd, readId,268632832);
    recvData(iFd);

}

int GvcpServer()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int iOptval = 1;
    struct sockaddr_in Addr;
    int iAddrLength;
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons(3956);
    iAddrLength = sizeof(Addr);
    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        qDebug()<<("socket fail");
        return -1;
    }
    else
    {
        qDebug()<<("socket ok");
    }
    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, (char*)&iOptval, sizeof(int)) < 0)
    {
        qDebug()<<("setsockopt failed!");
    }
    else
    {
        qDebug()<<("setsockopt ok");
    }


    if (bind(iFd, (struct sockaddr*)&Addr, sizeof(Addr)) == -1)
    {
        qDebug()<<("bind failed!");
    }
    else
    {
        qDebug()<<("bind ok");
    }
}

int interfaceReadReg2(){
    int iSendbytes;

    struct sockaddr_in Addr;
    int bNeedClose = 0;
    if (iFd < 0)
    {
        if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            qDebug()<<("socket fail");
            return -1;
        }
        bNeedClose = 1;
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("172.16.10.19");
    Addr.sin_port = htons(3956);

    static struct gvcp_readreg_cmd my_msg;
    memset(&my_msg, 0, sizeof(struct gvcp_readreg_cmd));
    my_msg.header.cMsgKeyCode = 0x42;
    my_msg.header.cFlag = 0x01;//0x11 allow broadcast ack;ack required
    my_msg.header.wCmd = htons(GVCP_READREG_CMD);//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    my_msg.header.wLen = htons(4);//payload length
    my_msg.header.wReqID = htons(readId+=1);// request id = 1;READREG id=12345
    my_msg.dwRegAddr = 0x44010100;

    char* rgMessage = (char*)&my_msg;
    uint32 dwMsgLen = sizeof(struct gvcp_readreg_cmd);
    //while (1)
    {
        if ((iSendbytes = sendto(iFd, rgMessage, dwMsgLen, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            qDebug()<<"sendto fail, errno=%d,%s"<<errno<< strerror(errno);
            return -1;
        }
        qDebug()<<"gvcp_ask_readreg=%s, rgMessageLen=%d,iSendbytes=%d"<< rgMessage<< dwMsgLen<< iSendbytes;
        //sleep(1);
    }
    if (bNeedClose > 0)
    {
#if 1
        closesocket(iFd);
#else
        close(iFd);
#endif
    }

    return 0;
}
