#include "CameraHik.h"
#include <QDebug>
#include <QImage>
#include "Log/QsLog.h"
using namespace MvCamCtrl;
#define MAX_BUF_SIZE    (3072*2048*3)


void __stdcall ImageCallBackEx(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
    if (pFrameInfo)
    {
        CameraHik *pThis = static_cast<CameraHik *>(pUser);

        if (pThis)
        {
            //qDebug() << __FUNCTION__ << pThis->cameraName();
            pThis->OnImageCallback(pData,pFrameInfo);
        }
    }
}

bool CameraHik::OpenDevice()
{
    if (m_isOpened)
    {
        QLOG_WARN() << "CameraHik::OpenDevice called, and camera is already opened.";
        return true;
    }

    int nRet = -1;

    //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO_LIST m_stDevList;

    //枚举子网内所有设备
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (0x%1)").arg(nRet  & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    // 选择查找到的第一台在线设备创建设备实例
    nRet = MV_CC_CreateHandle(&m_hHandle, m_stDevList.pDeviceInfo[m_index]);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when CreateHandle of device: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    //打开设备
    nRet = MV_CC_OpenDevice(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when OpenDevice: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        MV_CC_CloseDevice(m_hHandle);
        nRet = MV_CC_OpenDevice(m_hHandle);
        if (nRet != MV_OK)
        {
            return false;
        }
    }
    MV_CC_RegisterImageCallBackEx(m_hHandle, ImageCallBackEx, this);
    int nPacketSize = MV_CC_GetOptimalPacketSize(m_hHandle);
    if (nPacketSize > 0)
    {
        nRet = MV_CC_SetIntValue(m_hHandle,"GevSCPSPacketSize",nPacketSize);
        if(nRet != MV_OK)
        {
            printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
        }
    }
    else
    {
        printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
    }

    nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
    if (MV_OK != nRet)
    {
        printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
    }

    QLOG_INFO() << QString("[CameraHik] Camera-%1 create success!").arg(m_index);
    m_isOpened = true;
    return true;
}

bool CameraHik::OpenDeviceBySN(const QString cameraName)
{
    if (m_isOpened)
    {
        QLOG_WARN() << "CameraHik::OpenDevice called, and camera is already opened.";
        return true;
    }
    if(cameraName.split("&").size()!=3)
        return false;
    m_cameraName = cameraName;
    m_sn = cameraName.split("&")[2];
    m_index = CheckCameraIndex(m_sn);
    if (m_index == -1)
    {
        //QLOG_ERROR() << "The SN is not Hik";
        return false;
    }

    m_sn = m_sn;
    int nRet = -1;

    //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO_LIST m_stDevList;

    //枚举子网内所有设备
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (0x%1)").arg(nRet  & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    // 选择查找到的第一台在线设备创建设备实例
    nRet = MV_CC_CreateHandle(&m_hHandle, m_stDevList.pDeviceInfo[m_index]);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when CreateHandle of device: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    //打开设备
    nRet = MV_CC_OpenDevice(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when OpenDevice: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        MV_CC_CloseDevice(m_hHandle);
        nRet = MV_CC_OpenDevice(m_hHandle);
        if (nRet != MV_OK)
        {
            return false;
        }
    }
    MV_CC_RegisterImageCallBackEx(m_hHandle, ImageCallBackEx, this);
    int nPacketSize = MV_CC_GetOptimalPacketSize(m_hHandle);
    if (nPacketSize > 0)
    {
        nRet = MV_CC_SetIntValue(m_hHandle,"GevSCPSPacketSize",nPacketSize);
        if(nRet != MV_OK)
        {
            printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
        }
    }
    else
    {
        printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
    }

    QLOG_INFO() << QString("[CameraHik] Camera-%1 加载成功!").arg(m_index);
    m_isOpened = true;
    return true;
}

QStringList CameraHik::GetCameraList()
{
    //海康相机列表
    QStringList list;

    //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO_LIST m_stDevList;

    //枚举子网内所有设备
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (0x%1)").arg(nRet  & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return list;
    }

    for (int i = 0; i < int(m_stDevList.nDeviceNum); ++i)
    {
        if (m_stDevList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
        {
            if (checkCameraVender(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerName)) == true)
            {
                QString modleName = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName;
                QString sn = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;
                QString info = QString("%1&%2&%3").arg(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerName), modleName, sn);
                list.append(info);
            }
        }
        else if (m_stDevList.pDeviceInfo[i]->nTLayerType == MV_USB_DEVICE)
        {
            if (checkCameraVender(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chManufacturerName)) == true)
            {
                QString modleName = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chModelName;
                QString sn = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chSerialNumber;
                QString info = QString("%1&%2&%3").arg(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chManufacturerName), modleName, sn);
                list.append(info);
            }
        }
        else
        {

        }
    }

    return list;
}

int CameraHik::CheckCameraIndex(const QString SerialNumber)
{
    int index = -1;

    //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO_LIST m_stDevList;

    //枚举子网内所有设备
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (0x%1)").arg(nRet  & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return -1;
    }

    for (int i = 0; i < int(m_stDevList.nDeviceNum); ++i)
    {
        if (m_stDevList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
        {
            if (checkCameraVender(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerName)) == true)
            {
                QString sn = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;
                if (SerialNumber == sn)
                {
                    index = i;
                    break;
                }
            }
        }
        else if (m_stDevList.pDeviceInfo[i]->nTLayerType == MV_USB_DEVICE)
        {
            if (checkCameraVender(QString((char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chManufacturerName)) == true)
            {
                QString sn = (char *)m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chSerialNumber;
                if (SerialNumber == sn)
                {
                    index = i;
                    break;
                }
            }
        }
        else
        {

        }
    }

    return index;
}

void CameraHik::initTriggerSource()
{
    m_mapTriggerSource.insert(0, "Line0");
    m_mapTriggerSource.insert(1, "Line1");
    m_mapTriggerSource.insert(2, "Line2");
    m_mapTriggerSource.insert(3, "Line3");
    m_mapTriggerSource.insert(4, "Counter0");
    m_mapTriggerSource.insert(7, "Software");
    m_mapTriggerSource.insert(8, "FrequencyConverter");
    m_mapTriggerSource.insert(9, "Action1");
    m_mapTriggerSource.insert(13, "Anyway");
}

QMap<int, QString> CameraHik::GetTriggerSourceList()
{
    return m_mapTriggerSource;
}

QVector<int> CameraHik::GetSupportTriggerSource()
{
    return m_supportTriggerSource;
}

bool CameraHik::checkCameraVender(QString vender)
{
    QStringList list;
    list.append("Hik");
    list.append("GEV");
    for (int i = 0; i < list.size(); ++i)
    {
        if (vender.contains(list[i], Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

bool CameraHik::OpenDeviceForFrame()
{
    if (m_isOpened)
    {
        QLOG_WARN() << "CameraHik::OpenDevice called, and camera is already opened.";
        return true;
    }

    int nRet = -1;

    // 获取工厂单件
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    //设备信息列表结构体变量，用来存储设备列表
    MV_CC_DEVICE_INFO_LIST m_stDevList;

    //枚举子网内所有设备
    nRet = tlFactory.EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, m_stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    // 选择查找到的第一台在线设备创建设备实例
    nRet = MV_CC_CreateHandle(&m_hHandle, m_stDevList.pDeviceInfo[m_index]);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when CreateHandle of device: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_RegisterImageCallBackEx(m_hHandle, ImageCallBackEx, this);
    if (MV_OK != nRet)
    {
        printf("Register Image CallBack fail! nRet [0x%x]\n", nRet);
        return false;
    }

    //打开设备
    nRet = MV_CC_OpenDevice(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when OpenDevice: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        MV_CC_CloseDevice(m_hHandle);
        nRet = MV_CC_OpenDevice(m_hHandle);
        if (nRet != MV_OK)
        {
            return false;
        }
    }

    MV_IMAGE_BASIC_INFO mstruBasicInfo;
    memset(&mstruBasicInfo, 0, sizeof(MV_IMAGE_BASIC_INFO));
    nRet = MV_CC_GetImageInfo(m_hHandle, &mstruBasicInfo);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageInfo: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", MV_TRIGGER_MODE_OFF);
    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    m_isOpened = true;
    return true;
}

bool CameraHik::CloseDevice()
{

    if (!m_isOpened)
    {
        QLOG_WARN() << "CameraHik::CloseDevice called, but camera is already closed.";
        return true;
    }

    int nRet = -1;
    nRet = MV_CC_CloseDevice(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when CloseDevice: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_DestroyHandle(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when DestroyHandle: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    QLOG_INFO() << QString("[CameraHik] Camera-%1 close success!").arg(m_index);
    m_isOpened = false;
    return true;
}

bool CameraHik::StartGenImg()
{
    unsigned char *pDataForRGB = NULL;
    unsigned char * pData = NULL;
    int nRet;
    QLOG_INFO()<<"StartGenImg";
    // ch:获取数据包大小 | en:Get payload size
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(m_hHandle, "PayloadSize", &stParam);
    if (MV_OK != nRet)
    {
        printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
    }
    unsigned int nPayloadSize = stParam.nCurValue;

    // ch:开始取流 | en:Start grab image
    nRet = MV_CC_StartGrabbing(m_hHandle);
    if (MV_OK != nRet)
    {
        QLOG_ERROR()<<"Start Grabbing fail! nRet [0x%x]\n"<< nRet;
    }


    return true;
}

void CameraHik::stopGenImg()
{
    // ch:停止取流 | en:Stop grab image
    bool nRet = MV_CC_StopGrabbing(m_hHandle);
    if (MV_OK != nRet)
    {
        printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
    }
}

bool CameraHik::OneGenImg()
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::OneGenImg called, but camera is not opened.";
        OpenDevice();
    }

    // 开始采集图像
    int nRet = MV_CC_StartGrabbing(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Grabbing: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    MVCC_INTVALUE stIntvalue;
    memset(&stIntvalue, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(m_hHandle, "PayloadSize", &stIntvalue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Get PayloadSize: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    unsigned int nBufSize = stIntvalue.nCurValue; //一帧数据大小

    LPBYTE pFrameBuf = (unsigned char *)malloc(MAX_BUF_SIZE);

    MV_FRAME_OUT_INFO_EX stInfo;
    memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO));

    nRet = MV_CC_GetOneFrameTimeout(m_hHandle, pFrameBuf, nBufSize, &stInfo, 500);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetFrame: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        free(pFrameBuf);
        return false;
    }

    nRet = MV_CC_StopGrabbing(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when StopGrabbing: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        free(pFrameBuf);
        return false;
    }

    SendPhoto2TestGUI(pFrameBuf);

    free(pFrameBuf);

    return true;
}

bool CameraHik::SetExposureTime(float ExposureTime)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::SetExposureTime called, but camera is not opened.";
        return false;
    }

    int nRet = MV_CC_SetFloatValue(m_hHandle, "ExposureTime", ExposureTime);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Setting ExposureTime: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::SetGain(float Gain)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::SetGain called, but camera is not opened.";
        return false;
    }

    int nRet = MV_CC_SetFloatValue(m_hHandle, "Gain", Gain);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Setting Gain: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::GetCameraInfo(QVector<CameraInfo> &infos)
{
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    MV_CC_DEVICE_INFO_LIST stDevList;
    memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    if (MV_OK != tlFactory.EnumDevices(MV_GIGE_DEVICE, stDevList))
    {
        m_lastError = "Error when Enum Devices";
        CamError(m_lastError);
        return false;
    }

    if (m_index >= stDevList.nDeviceNum)
    {
        m_lastError = "Invalid device index";
        CamError(m_lastError);
        return false;
    }

    for (unsigned int i = 0; i < stDevList.nDeviceNum; i++)
    {
        CameraInfo info;
        info.vendor = QString::fromStdString((char *)stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerName);
        info.model = QString::fromStdString((char *)stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName);
        info.sn = QString::fromStdString((char *)stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber);
        info.mac = "";

        unsigned int ip = stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp;
        int ip1 = (ip >> 24) & 0x000000FF;
        int ip2 = (ip >> 16) & 0x000000FF;
        int ip3 = (ip >> 8) & 0x000000FF;
        int ip4 = ip & 0x000000FF;
        info.ip = QString("%1.%2.%3.%4").arg(QString::number(ip1), QString::number(ip2), QString::number(ip3), QString::number(ip4));

        infos.push_back(info);
    }
    return true;
}

bool CameraHik::GetCurrentCameraInfo(CameraInfo &info)
{
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    MV_CC_DEVICE_INFO_LIST stDevList;
    memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    if (MV_OK != tlFactory.EnumDevices(MV_GIGE_DEVICE, stDevList))
    {
        m_lastError = "Error when Enum Devices";
        CamError(m_lastError);
        return false;
    }

    if (m_index >= stDevList.nDeviceNum)
    {
        m_lastError = "Invalid device index";
        CamError(m_lastError);
        return false;
    }

    info.vendor = QString::fromStdString((char *)stDevList.pDeviceInfo[m_index]->SpecialInfo.stGigEInfo.chManufacturerName);
    info.model = QString::fromStdString((char *)stDevList.pDeviceInfo[m_index]->SpecialInfo.stGigEInfo.chModelName);
    info.sn = QString::fromStdString((char *)stDevList.pDeviceInfo[m_index]->SpecialInfo.stGigEInfo.chSerialNumber);
    info.mac = "";

    unsigned int ip = stDevList.pDeviceInfo[m_index]->SpecialInfo.stGigEInfo.nCurrentIp;
    int ip1 = (ip >> 24) & 0x000000FF;
    int ip2 = (ip >> 16) & 0x000000FF;
    int ip3 = (ip >> 8) & 0x000000FF;
    int ip4 = ip & 0x000000FF;
    info.ip = QString("%1.%2.%3.%4").arg(QString::number(ip1), QString::number(ip2), QString::number(ip3), QString::number(ip4));

    return true;
}

bool CameraHik::GetImageFormat(int &Width, int &Height)
{
    if (!m_isOpened)
    {
        //qDebug() << "CameraHik::GetImageFormat called, but camera is not opened.";
        OpenDevice();
    }

    MVCC_INTVALUE struIntValue;
    int nRet = MV_CC_GetIntValue(m_hHandle, "Width", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageWidth: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Width = struIntValue.nCurValue;
    nRet = MV_CC_GetIntValue(m_hHandle, "Height", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageHeight: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Height = struIntValue.nCurValue;

    return true;
}

bool CameraHik::GetImageFormatForFrame(int &Width, int &Height)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::GetImageFormat called, but camera is not opened.";
        OpenDeviceForFrame();
    }

    MVCC_INTVALUE struIntValue;
    int nRet = MV_CC_GetIntValue(m_hHandle, "Width", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageWidth: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Width = struIntValue.nCurValue;
    nRet = MV_CC_GetIntValue(m_hHandle, "Height", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageHeight: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Height = struIntValue.nCurValue;

    return true;
}

bool CameraHik::StartImg()
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::StartGenImg called, but camera is not opened.";
        OpenDeviceForFrame();
    }

    // 开始采集图像
    int nRet = MV_CC_StartGrabbing(m_hHandle);

    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Grabbing: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    MVCC_INTVALUE stIntvalue;
    memset(&stIntvalue, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(m_hHandle, "PayloadSize", &stIntvalue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Get PayloadSize: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nBufSize = stIntvalue.nCurValue; //一帧数据大小

    return true;
}

bool CameraHik::EndImg()
{
    int nRet = MV_CC_StopGrabbing(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when StopGrabbing: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::GetImg()
{
    // LPBYTE pFrameBuf = (unsigned char *)malloc(MAX_BUF_SIZE);

    // MV_FRAME_OUT_INFO_EX stInfo;
    // memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO));

    // int nRet = MV_CC_GetOneFrameTimeout(m_hHandle, pFrameBuf, nBufSize, &stInfo, 1000);
    //    if (nRet != MV_OK)
    //    {
    //        m_lastError = QString("Error when GetFrame: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
    //        CamError(m_lastError);
    //        free(pFrameBuf);
    //        return false;
    //    }

    // emit SendPhoto2GUI(pFrameBuf, m_index);

    //  free(pFrameBuf);
    return true;
}

CameraHik::~CameraHik()
{
    MV_CC_CloseDevice(m_hHandle);
}

void CameraHik::OnImageCallback(unsigned char *pData,MV_FRAME_OUT_INFO_EX * imageInfo)
{
    //    emit SendPhoto2GUI(pData, m_index);
    //    emit SigSendImageData(pData, m_cameraName);
    QImage image;
    if(startGrabImage(image,imageInfo,pData)){
            //signSendQIamge(image);
    }
}

bool CameraHik::SetTriggerMode(const QString &mode)
{
    int nRet = MV_OK;
    if (mode == "line0")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0);
    }
    else if (mode == "line1")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE1);
    }
    else if (mode == "line2")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE2);
    }
    else if (mode == "line3")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE3);
    }
    else if (mode == "counter0")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_COUNTER0);
    }
    else if (mode == "software")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
    }
    else if (mode == "freq")
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_FrequencyConverter);
    }
    else
    {
        nRet = MV_E_SUPPORT;
    }

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        QLOG_ERROR() << m_lastError;
        CamError(m_lastError);
        return false;
    }
    QLOG_INFO() << "Set trigger mode success!";

    return true;
}

bool CameraHik::SetTriggerSource(const unsigned int source)
{
    int nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", source);

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        QLOG_ERROR() << m_lastError;
        CamError(m_lastError);
        return false;
    }
    QLOG_INFO() << "Set trigger mode success!";

    return true;
}

bool CameraHik::TriggerSoftwareExecute()
{
    int nRet;
    nRet = MV_CC_SetCommandValue(m_hHandle, "TriggerSoftware");

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when TriggerSoftware: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        QLOG_ERROR() << m_lastError;
        CamError(m_lastError);
        return false;
    }
    return true;
}

bool CameraHik::OpenDeviceRGB()
{
    if (m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::OpenDeviceRGB called, and camera is already opened.";
        return true;
    }

    int nRet = -1;

    // 获取工厂单件
    CTlFactory &tlFactory = CTlFactory::GetInstance();

    // 枚举子网内所有GigE设备，返回设备信息列表
    MV_CC_DEVICE_INFO_LIST stDevList;

    memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    nRet = tlFactory.EnumDevices(MV_GIGE_DEVICE, stDevList);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when EnumDevices (%x)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    // 选择查找到的第一台在线设备创建设备实例
    nRet = MV_CC_CreateHandle(&m_hHandle, stDevList.pDeviceInfo[m_index]);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when CreateHandle of device: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    //注册回调函数
    nRet = MV_CC_RegisterImageCallBackForRGB(m_hHandle, ImageCallBackEx, this);
    if (MV_OK != nRet)
    {
        printf("Register Image CallBack fail! nRet [0x%x]\n", nRet);
        return false;
    }

    //打开设备
    nRet = MV_CC_OpenDevice(m_hHandle);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when OpenDeviceRGB: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    MV_IMAGE_BASIC_INFO mstruBasicInfo;
    memset(&mstruBasicInfo, 0, sizeof(MV_IMAGE_BASIC_INFO));
    nRet = MV_CC_GetImageInfo(m_hHandle, &mstruBasicInfo);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageInfo: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    //unsigned int enValue = mstruBasicInfo.enPixelType;
    unsigned int enValue = PixelType_Gvsp_BayerRG8;
    nRet = MV_CC_SetPixelFormat(m_hHandle, enValue);
    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when MV_CC_SetPixelFormat: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", MV_TRIGGER_MODE_ON);
    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0);
    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Hardware Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    nRet = MV_CC_StartGrabbing(m_hHandle);
    if (MV_OK != nRet)
    {
        printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
        return false;
    }

    m_isOpened = true;
    return true;
}

bool CameraHik::SetExposureTimeRGB(float ExposureTime)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::SetExposureTime called, but camera is not opened.";
        OpenDeviceRGB();
    }

    int nRet = MV_CC_SetFloatValue(m_hHandle, "ExposureTime", ExposureTime);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Setting ExposureTime: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::SetGainRGB(float Gain)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::SetGain called, but camera is not opened.";
        OpenDeviceRGB();
    }

    int nRet = MV_CC_SetFloatValue(m_hHandle, "Gain", Gain);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when Setting Gain: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::GetImageFormatRGB(int &Width, int &Height)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::GetImageFormat called, but camera is not opened.";
        OpenDeviceRGB();
    }

    MVCC_INTVALUE struIntValue;
    int nRet = MV_CC_GetIntValue(m_hHandle, "Width", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageWidth: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Width = struIntValue.nCurValue;
    nRet = MV_CC_GetIntValue(m_hHandle, "Height", &struIntValue);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when GetImageHeight: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    Height = struIntValue.nCurValue;

    return true;
}

bool CameraHik::SetFloatParam(FloatParamName name, float param)
{
    QString strName;
    if (name == FloatParamName::ExposureTime)
        strName = "ExposureTime";
    else if (name == FloatParamName::Gain)
        strName = "Gain";
    else if (name == FloatParamName::TriggerDelay)
        strName = "TriggerDelay";
    else if (name == FloatParamName::ResultingFrameRate)
        strName = "ResultingFrameRate";
    else if (name == FloatParamName::AutoGainLowerLimit)
        strName = "AutoGainLowerLimit";
    else if (name == FloatParamName::AutoGainupperLimit)
        strName = "AutoGainupperLimit";
    else if (name == FloatParamName::BlackLevel)
        strName = "BlackLevel";
    else if (name == FloatParamName::Gamma)
        strName = "Gamma";
    else if (name == FloatParamName::BalanceRatio)
        strName = "BalanceRatio";
    else
    {
        QLOG_ERROR() << "[CameraHik] Float param name is invaild!";
        return false;
    }

    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::SetFloatParam called, but camera is not opened!";
        return false;
    }

    int nRet = MV_CC_SetFloatValue(m_hHandle, strName.toLatin1().data(), param);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when SetFloatParam[%2]: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16).arg(strName);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::SetIntParam(IntParamName name, unsigned int param)
{
    QString strName;
    if (name == IntParamName::AcquisitionLineRate)
        strName = "AcquisitionLineRate";
    else
    {
        QLOG_ERROR() << "[CameraHik] Int param name is invaild!";
        return false;
    }

    int nRet = MV_CC_SetIntValue(m_hHandle, strName.toLatin1().data(), param);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when MV_CC_SetIntValue[%2]: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16).arg(strName);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::SetEnumParam(EnumParamName name, unsigned int param)
{
    QString strName;
    if (name == EnumParamName::BalanceRatioSelector)
        strName = "BalanceRatioSelector";
    else if (name == EnumParamName::BalanceWhiteAuto)
        strName = "BalanceWhiteAuto";
    else
    {
        QLOG_ERROR() << "[CameraHik] Enum param name is invaild!";
        return false;
    }

    int nRet = MV_CC_SetEnumValue(m_hHandle, strName.toLatin1().data(), param);
    if (nRet != MV_OK)
    {
        m_lastError = QString("Error when MV_CC_SetEnumValue[%2]: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16).arg(strName);
        CamError(m_lastError);
        return false;
    }

    return true;
}

bool CameraHik::SetTriggerOnOrOff(bool isOn)
{
    int nRet;
    if (isOn == true)
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", MV_TRIGGER_MODE_ON);

    }
    else
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", MV_TRIGGER_MODE_OFF);
    }

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Trigger Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    return true;
}

bool CameraHik::SetAcquisitionMode(AcquisitionModeParam mode)
{
    int nRet;
    if (mode == AcquisitionModeParam::SingleFrame)
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "AcquisitionMode", MV_ACQ_MODE_SINGLE);
    }
    else if (mode == AcquisitionModeParam::MultiFrame)
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "AcquisitionMode", MV_ACQ_MODE_MUTLI);
    }
    else if (mode == AcquisitionModeParam::Continuous)
    {
        nRet = MV_CC_SetEnumValue(m_hHandle, "AcquisitionMode", MV_ACQ_MODE_CONTINUOUS);
    }
    else
    {
        nRet = MV_E_UNKNOW;
    }

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set Acquisition Mode: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16);
        CamError(m_lastError);
        return false;
    }
    return true;
}

bool CameraHik::SetBoolParam(BoolParamName name, bool tf)
{
    int nRet;
    QString strName;
    if (name == BoolParamName::LUTEnable)
    {
        strName = "LUTEnable";
        nRet = MV_CC_SetBoolValue(m_hHandle, "LUTEnable", tf);
    }
    else
    {
        nRet = MV_E_UNKNOW;
    }

    if (MV_OK != nRet)
    {
        m_lastError = QString("Error when Set %2: (0x%1)").arg(nRet & 0xFFFFFFFF, 0, 16).arg(strName);
        CamError(m_lastError);
        return false;
    }
    return true;
}

bool CameraHik::isCameraOpen()
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::Camera not opened!";
        return false;
    }

    MVCC_INTVALUE struIntValue;
    int nRet = MV_CC_GetIntValue(m_hHandle, "Width", &struIntValue);
    nRet = MV_CC_GetWidth(m_hHandle, &struIntValue);
    if (nRet != MV_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CameraHik::setPixelFormat(const unsigned int type)
{
    m_pixelFormat = type;
    return true;
}

bool CameraHik::setGrabbing(const bool start)
{
    if (!m_isOpened)
    {
        QLOG_ERROR() << "CameraHik::Camera not opened!";
        return false;
    }

    if (start == true)
    {
        bool nRet = MV_CC_StartGrabbing(m_hHandle);
        if (MV_OK != nRet)
        {
            printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
            return false;
        }
    }
    else
    {
        bool nRet = MV_CC_StopGrabbing(m_hHandle);
        if (MV_OK != nRet)
        {
            printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
            return false;
        }
    }
    return true;
}

bool CameraHik::startGrabImage(QImage &retImage, MV_FRAME_OUT_INFO_EX *stImageInfo, unsigned char *data)
{
    cv::Mat image;
    auto m_nBufSizeForSaveImage = stImageInfo->nWidth * stImageInfo->nHeight * 3 + 2048;
    //auto m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
    m_pBufForSaveImage.reset(new unsigned char[m_nBufSizeForSaveImage]);
    bool isMono;//判断是否为黑白图像
    switch (stImageInfo->enPixelType)
    {
    case PixelType_Gvsp_Mono8:
    case PixelType_Gvsp_Mono10:
    case PixelType_Gvsp_Mono10_Packed:
    case PixelType_Gvsp_Mono12:
    case PixelType_Gvsp_Mono12_Packed:
        isMono = true;
        break;
    default:
        isMono = false;
        break;
    }
    cv::Mat dstImage1;
    if (isMono)
    {
        image = cv::Mat(stImageInfo->nHeight, stImageInfo->nWidth, CV_8UC1, data);
        //cv::resize( image, dstImage1, Size(image.cols/2, image.rows/2), 0, 0, INTER_NEAREST );
    }
    else
    {
        //转换图像格式为BGR8
        MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
        memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
        stConvertParam.nWidth = stImageInfo->nWidth;
        stConvertParam.nHeight = stImageInfo->nHeight;
        stConvertParam.pSrcData = data;
        stConvertParam.nSrcDataLen = stImageInfo->nFrameLen;
        stConvertParam.enSrcPixelType = stImageInfo->enPixelType;
        stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
        //stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;
        stConvertParam.pDstBuffer = m_pBufForSaveImage.get();
        stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;
        MV_CC_ConvertPixelType(m_hHandle, &stConvertParam);
        image = cv::Mat(stImageInfo->nHeight, stImageInfo->nWidth, CV_8UC3, m_pBufForSaveImage.get());

    }

    if(m_callback_ != nullptr)//回调到control
        m_callback_->hikImageResult(image, m_cameraIndex);
#ifdef DEBUG //返回QImage
    switch (image.type())
    {
    case CV_8UC1:
    {
        QImage img((const unsigned char *)(resizeMat.data), resizeMat.cols, resizeMat.rows, resizeMat.cols, QImage::Format_Grayscale8);
        retImage = img;
    }
        break;
    case CV_8UC3:
    {
        QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 3, QImage::Format_RGB888);
        retImage = img.rgbSwapped();
    }
        break;
    case CV_8UC4:
    {
        QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 4, QImage::Format_ARGB32);
        retImage = img;
    }
        break;
    default:
    {
        QImage img;
        retImage = img;
    }
        break;
    }
#endif
    //free m_pBufForSaveImage;
    return 1;
}
