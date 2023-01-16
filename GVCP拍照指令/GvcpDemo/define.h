#ifndef DEFINE_H
#define DEFINE_H
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>
#include <QDebug>
#ifndef ETH_ALEN
#define ETH_ALEN       6              // 以太网地址大小
#define ETH_HLEN       14             // 以太网头部大小
#define ETH_DATA_LEN   1500           // 最大帧负载数据大小
#define ETH_FRAME_LEN  1514           // 最大帧大小，头部+负载数据
#endif

#include <errno.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define GVCP_DISCOVERY_CMD	2
#define GVCP_DISCOVERY_ACK	3
#define GVCP_FORCEIP_CMD	4
#define GVCP_FORCEIP_ACK	5
#define GVCP_READREG_CMD	0x80
#define GVCP_READREG_ACK	0x81
#define GVCP_WRITEREG_CMD	0x82
#define GVCP_WRITEREG_ACK	0x83
#define GVCP_READMEM_CMD	0x84
#define GVCP_READMEM_ACK	0x85

struct gvcp_cmd_header {
    uint8 cMsgKeyCode;//0x42
    uint8 cFlag;//0x11 allow broadcast ack;ack required
    uint16 wCmd;//discovery_cmd=2;FORCEIP_CMD = 4;READREG_CMD=0x80
    uint16 wLen;//payload length
    uint16 wReqID;// request id = 1;READREG id=12345
};

struct gvcp_forceip_payload {
    uint8 Mac[8];//last 6 byte
    uint8 CurIP[16];//last 4 byte
    uint8 SubMask[16];//last 4 byte
    uint8 Gateway[16];//last 4 byte
};

struct gvcp_ack_header {
    uint16 wStatus;//success=0;
    uint16 wAck;//discover_ack=3;forceip_ack=5;READREG_ACK=0x81
    uint16 wLen;
    uint16 wReqID;
};



struct gvcp_ack_payload {
    uint32 dwSpecVer;
    uint32 dwDevMode;
    uint8 Mac[8];//last 6 byte
    uint32 dwSupIpSet;
    uint32 dwCurIpSet;
    //uint8 unused1[12];
    uint8 CurIP[16];//last 4 byte
    uint8 SubMask[16];//last 4 byte
    uint8 Gateway[16];//last 4 byte
    char szFacName[32];//first
    char szModelName[32];//first
    char szDevVer[32];
    char szFacInfo[48];
    char szSerial[16];
    char szUserName[16];
};

struct gvcp_discover_cmd {
    struct gvcp_cmd_header header;
};
struct gvcp_discover_ack {
    struct gvcp_ack_header header;
    struct gvcp_ack_payload payload;
};
struct gvcp_forceip_cmd {
    struct gvcp_cmd_header header;
    struct gvcp_forceip_payload payload;
};
struct gvcp_forceip_ack {
    struct gvcp_ack_header header;
};
struct gvcp_readreg_cmd {
    struct gvcp_cmd_header header;
    uint32 dwRegAddr;
};
struct gvcp_readreg_ack {
    struct gvcp_ack_header header;
    uint32 dwRegValue;
};
struct gvcp_writereg_cmd {
    struct gvcp_cmd_header header;
    uint32 dwRegAddr;
    uint32 dwRegValue;
};

struct gvcp_writereg_ack {
    struct gvcp_ack_header header;
};

struct gvcp_readmem_cmd {
    struct gvcp_cmd_header header;
    uint32 dwMemAddr;
    uint32 dwMemCount;//last 2 byte
};
struct gvcp_readmem_ack {
    struct gvcp_ack_header header;
    uint32 dwMemAddr;
    char* pMemBuf;
};

#if 1//most hardware
#define MY_DEV_NAME "eth3"
#else
#define MY_DEV_NAME "ens33"
#endif
struct gvsp_frist_ack_header {
    uint16      wStatus;//success=0;
    uint16      wBlockId;//discover_ack=3;forceip_ack=5;READREG_ACK=0x81
    uint8       wFormat;
    char        wPackId[3];
    uint8       wFieldInfo;
    uint16      wPayloadType;
    char        wTimeStamp[8];
    uint32      wPixelFormat;
    uint32      wW;
    uint32      wH;
    uint32      wOffsetX;
    uint32      wOffsety;
    uint16      wPaddingX;
    uint16      wPaddingY;
};

struct gvsp_normal_ack_header {
    uint16      wStatus;//success=0;
    uint16      wBlockId;//discover_ack=3;forceip_ack=5;READREG_ACK=0x81
    uint8       wFormat;
    char        wPackId[3];
    char *      wPayloadData;
};

struct gvsp_last_ack_header {
    uint16      wStatus;//success=0;
    uint16      wBlockId;//discover_ack=3;forceip_ack=5;READREG_ACK=0x81
    uint8       wFormat;
    char        wPackId[3];
    uint16      wPayloadType;
    uint32      wH;
};

#endif // DEFINE_H
