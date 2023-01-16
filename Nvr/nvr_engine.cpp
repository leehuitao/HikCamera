#include "nvr_engine.h"
#include <Windows.h>
#include <QDebug>
#include <QFile>
NvrEngine::NvrEngine(QObject *parent) : QObject(parent)
{
    initSdk();
    login("192.168.105.108",37777,"admin","admin123");
}

// 设备断线回调函数
// 当设备出现断线时，SDK 会调用该函数，在 CLIENT_Init 中设置该回调函数
void  CALLBACK DisConnectFunc(LLONG lLoginID, char *pchDVRIP,LONG nDVRPort, LDWORD dwUser)
{
    qDebug()<<"Call DisConnectFunc\n";
    qDebug()<<"lLoginID[0x%x]"<< lLoginID;
    if (nullptr != pchDVRIP)
    {
        qDebug()<<"pchDVRIP[%s]\n", pchDVRIP;
    }
    qDebug()<<"nDVRPort[%d]\n"<< nDVRPort;
    qDebug()<<"dwUser[%p]\n"<< dwUser;
}

void NvrEngine::initSdk()
{
    // 初始化 SDK
    BOOL g_bNetSDKInitFlag = CLIENT_Init(DisConnectFunc, 0);
    if (FALSE == g_bNetSDKInitFlag)
    {
        qDebug()<<"Initialize client SDK failed;";
        return;
    }
    else
    {
        qDebug()<<"Initialize client SDK done;";
    }
}

void NvrEngine::login(QString ip, int port, QString username, QString password)
{
    // 登录设备
    NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY stInparam;
    memset(&stInparam, 0, sizeof(stInparam));
    stInparam.dwSize = sizeof(stInparam);
    strncpy(stInparam.szIP, ip.toLocal8Bit().data(), sizeof(stInparam.szIP) - 1);
    strncpy(stInparam.szPassword, password.toLocal8Bit().data(),sizeof(stInparam.szPassword) - 1);
    strncpy(stInparam.szUserName, username.toLocal8Bit().data(),sizeof(stInparam.szUserName) - 1);
    stInparam.nPort = port;
    stInparam.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
    NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY stOutparam;
    memset(&stOutparam, 0, sizeof(stOutparam));
    stOutparam.dwSize = sizeof(stOutparam);
    m_loginID =CLIENT_LoginWithHighLevelSecurity(&stInparam, &stOutparam);
    qDebug()<<"lLoginID = "<<m_loginID;
    qDebug()<<"Last Error[%x]\n" << CLIENT_GetLastError();
}

void NvrEngine::startRealPlay(HWND handle)
{
    // 获取控制台窗口句柄
    HWND hWnd = handle;
    //开启实时监视
    int nChannelID = 0; // 预览通道号
    DH_RealPlayType emRealPlayType = DH_RType_Realplay;
    g_lRealHandle = CLIENT_RealPlayEx(m_loginID, nChannelID,hWnd, emRealPlayType);
    if (g_lRealHandle == 0)
    {
        qDebug()<<"CLIENT_RealPlayEx: failed! Error code: %x.\n"<<CLIENT_GetLastError();
    }
}

void NvrEngine::stopRealPlay()
{
    if (!CLIENT_StopRealPlayEx(g_lRealHandle)) {
        qDebug()<<"CLIENT_StopRealPlayEx Failed, g_lRealHandle[%x]!Last Error[%x]\n" , g_lRealHandle, CLIENT_GetLastError();
    }
}

void NvrEngine::playBackByTime(HWND handle)
{
    // 获取控制台窗口句柄
    HWND hWnd = handle;
    int nChannelID = 0; // 通道号
    NET_TIME stuStartTime = {0};
    stuStartTime.dwYear = 2022;
    stuStartTime.dwMonth = 1;
    stuStartTime.dwDay = 25;
    stuStartTime.dwHour = 15;
    stuStartTime.dwMinute = 30;
    NET_TIME stuStopTime = {0};
    stuStopTime.dwYear = 2022;
    stuStopTime.dwMonth = 1;
    stuStopTime.dwDay = 25;
    stuStopTime.dwHour = 16;
    stuStopTime.dwMinute = 0;
    LLONG g_lPlayHandle = CLIENT_PlayBackByTimeEx(m_loginID, nChannelID, &stuStartTime, &stuStopTime, hWnd, nullptr, NULL, nullptr, NULL);
    if (g_lPlayHandle == 0)
    {
        qDebug()<<"CLIENT_PlayBackByTimeEx: failed! Error code: 0x%x.\n"<<CLIENT_GetLastError();
    }
}

void NvrEngine::getBackByTime()
{
    NET_TIME StartTime = {0};
    NET_TIME StopTime = {0};
    StartTime.dwYear = 2022;
    StartTime.dwMonth = 1;
    StartTime.dwDay = 25;
    StartTime.dwHour = 0;
    StartTime.dwMinute = 0;
    StopTime.dwYear = 2022;
    StopTime.dwMonth = 1;
    StopTime.dwDay = 26;
    StopTime.dwHour = 8;
    NET_RECORDFILE_INFO netFileInfo[30] = {0};
    int nFileCount = 0;
    // 获取录像查询句柄
    int nChannelID = 0; // 预览通道号
    LLONG lFindHandle = CLIENT_FindFile (m_loginID, nChannelID,(int)EM_RECORD_TYPE_ALL, nullptr, &StartTime, &StopTime, FALSE,5000);
    if(!lFindHandle)
    {
        qDebug()<<"CLIENT_FindFile: failed! Error code: %x.\n"<<CLIENT_GetLastError();
    }
    NET_RECORDFILE_INFO struFileData = {0};
    int result = CLIENT_FindNextFile(lFindHandle, &struFileData);
    if(result == 1)//取回一条录像文件信息
    {
        //存储录像文件
        bool status = QFile::copy(struFileData.filename,"./"+QString(struFileData.filename).split("/").last());
        qDebug()<<struFileData.framenum<<"|"<<struFileData.size<<"|"<<struFileData.filename<<"save status = "<<status;
    } else if (result == 0) {
        //录像文件信息数据取完
        //QFile file("./"+QString(struFileData.filename).split("/").last());
        bool status = QFile::copy(struFileData.filename,"./"+QString(struFileData.filename).split("/").last());

        qDebug()<<"录像文件信息数据取完 save status = "<<status;
    } else {
        //参数出错
        qDebug()<<"CLIENT_FindNextFile: failed! Error code:0x%x.\n"<<CLIENT_GetLastError();

    }
}
void CALLBACK TimeDownLoadPosCallBack(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, LDWORD dwUser){
    qDebug()<<"dwTotalSize = "<<dwTotalSize<<"dwDownLoadSize = "<<dwDownLoadSize<<"index = "<<index;
}

int CALLBACK DataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser){
    return 1;
}

void NvrEngine::downloadByTime()
{
    // 录像文件查询
    // 设置查询时的录像码流类型
    int nStreamType = 0; // 0-主辅码流,1-主码流,2-辅码流
    CLIENT_SetDeviceMode(m_loginID, DH_RECORD_STREAM_TYPE,
                         &nStreamType);
    int nChannelID = 0; // 通道号
    NET_TIME stuStartTime = {0};
    stuStartTime.dwYear = 2022;
    stuStartTime.dwMonth = 1;
    stuStartTime.dwDay = 25;
    stuStartTime.dwHour = 15;
    stuStartTime.dwMinute = 30;
    NET_TIME stuStopTime = {0};
    stuStopTime.dwYear = 2022;
    stuStopTime.dwMonth = 1;
    stuStopTime.dwDay = 25;
    stuStopTime.dwHour = 16;
    stuStopTime.dwMinute = 0;
    // 录像下载功能业务实现处
    // 开启录像下载
    // 函数形参 sSavedFileName 和 fDownLoadDataCallBack 需至少有一个为有效值，否则入参有误
    g_lDownloadHandle = CLIENT_DownloadByTimeEx(m_loginID, nChannelID, EM_RECORD_TYPE_ALL, &stuStartTime, &stuStopTime, QString("test.dav").toLocal8Bit().data(),
                                                TimeDownLoadPosCallBack, NULL, nullptr, NULL);
    if (g_lDownloadHandle == 0)
    {
        qDebug()<<"CLIENT_DownloadByTimeEx: failed! Error code: "<<CLIENT_GetLastError();
    }
}

void NvrEngine::EndTest()
{
    qDebug()<<"input any key to quit!";

    getchar();
    // 关闭下载，可在下载结束后调用，也可在下载中调用
    if (0 != g_lDownloadHandle)
    {
        if (FALSE == CLIENT_StopDownload(g_lDownloadHandle))
        {
            qDebug()<<"CLIENT_StopDownload Failed, g_lDownloadHandle[%x]!Last " << g_lDownloadHandle<< CLIENT_GetLastError();
        }
        else
        {
            g_lDownloadHandle = 0;
        }
    }
    return;
}

void NvrEngine::openDownloadFile(std::string url)
{
    auto openstatus = m_cap.open(url);
    double rate = m_cap.get(CV_CAP_PROP_FPS);
    m_cap.set(CV_CAP_PROP_FPS, rate);//帧率 帧/秒
    qDebug()<<"openstatus = "<<openstatus;
    cv::namedWindow("1",cv::WINDOW_FREERATIO);
    cv::Mat frame;
    if(openstatus){
        while(m_cap.read(frame)){
            if(frame.empty())
                break;
            cv::flip(frame,frame,-1);
            cv::imshow("1",frame);
            int key = cv::waitKey(1);
            if(key == 27)
                return;
        }
    }
}


