#include "camera_opt.h"
#include <QImage>

CameraOPT::CameraOPT()
{

}

int32_t CameraOPT::GENICAM_CreateStreamSource(GENICAM_Camera *pGetCamera, GENICAM_StreamSource **ppStreamSource){
    int32_t isCreateStreamSource;
    GENICAM_StreamSourceInfo stStreamSourceInfo;
    stStreamSourceInfo.channelId = 0;
    stStreamSourceInfo.pCamera = pGetCamera;
    isCreateStreamSource = GENICAM_createStreamSource(&stStreamSourceInfo, ppStreamSource);
    if( isCreateStreamSource != 0)    {
        qDebug()<<"create stream obj  fail.\r\n";
        return -1;
    }
    return 0;
}

int32_t CameraOPT::GENICAM_connect(GENICAM_Camera *pGetCamera){
    int32_t isConnectSuccess;
    isConnectSuccess = pGetCamera->connect(pGetCamera, accessPermissionControl);
    if( isConnectSuccess != 0)    {
        qDebug()<<"connect camera failed.\n";
        return -1;
    }
    return 0;
}
int32_t isGrabbingFlag=0;

void onGetFrame(GENICAM_Frame* pFrame ,void * pUser){
    int32_t ret = -1;
    uint64_t blockId = 0;    // 标准输出换行
    ret = pFrame->valid(pFrame);
    if(0 == ret)    {

        blockId = pFrame->getBlockId(pFrame);
        qDebug()<<"blockId = %d.\r\n"<<blockId;
        CameraOPT * callback = static_cast<CameraOPT *>(pUser);
        callback->OnImageCallback(pFrame);
        isGrabbingFlag=0;
    }    else    {
        qDebug()<<"Frame is invalid!\n";

    }
    //Caution：release the frame after using it      //注意：使用该帧后需要显示释政
    ret = pFrame->release(pFrame);
    return;
}
void mat2QImage(const cv::Mat &mat, QImage &image, bool resize_)
{

    IplImage  iplImage = cvIplImage(mat);
    uchar *qImageBuffer = NULL;
    int width = iplImage.width;

    int widthStep = iplImage.widthStep;
    int height = iplImage.height;
    qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
    uchar *QImagePtr = qImageBuffer;
    const uchar *iplImagePtr = (const uchar *)iplImage.imageData;
    for (int y = 0; y < height; y++)
    {
        // Copy line by line
        memcpy(QImagePtr, iplImagePtr, width);
        QImagePtr += width;
        iplImagePtr += widthStep;
    }
    QVector<QRgb> colorTable;
    for (int i = 0; i < 256; i++)
    {
        colorTable.push_back(qRgb(i, i, i));
    }

    QImage resultimage_(qImageBuffer, width, height, QImage::Format_Indexed8);
    resultimage_.setColorTable(colorTable);
    image = resultimage_.copy();
    delete qImageBuffer;
}
QImage	CameraOPT::MatToQImage(cv::Mat mtx)
{
    switch (mtx.type())
    {
    case CV_8UC1:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
        return img;
    }
        break;
    case CV_8UC3:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 3, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
        break;
    case CV_8UC4:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
        return img;
    }
        break;
    default:
    {
        QImage img;
        return img;
    }
        break;
    }
}

QString CameraOPT::name()
{
    return "";
}

bool CameraOPT::OpenDevice()
{
    return 1;
}

bool CameraOPT::OpenDeviceForFrame()
{
    return 1;
}

bool CameraOPT::CloseDevice()
{
    return 1;
}

bool CameraOPT::SetExposureTime(float ExposureTime)
{
    return 1;
}

bool CameraOPT::GetCameraInfo(QVector<CameraInfo> &infos)
{
    return 1;
}

bool CameraOPT::GetCurrentCameraInfo(CameraInfo &info)
{
    return 1;
}

bool CameraOPT::GetImageFormat(int &Width, int &Height)
{
    return 1;
}

bool CameraOPT::GetImageFormatForFrame(int &Width, int &Height)
{
    return 1;
}

bool CameraOPT::SetGain(float Gain)
{
    return 1;
}

bool CameraOPT::GetImg()
{
    return 1;
}

bool CameraOPT::StartImg()
{
    return 1;
}

bool CameraOPT::EndImg()
{
    return 1;
}

bool CameraOPT::SetTriggerMode(const QString &mode)
{
    return 1;
}

bool CameraOPT::TriggerSoftwareExecute()
{
    return 1;
}

bool CameraOPT::OpenDeviceRGB()
{
    return 1;
}

bool CameraOPT::SetExposureTimeRGB(float ExposureTime)
{
    return 1;
}

bool CameraOPT::GetImageFormatRGB(int &Width, int &Height)
{
    return 1;
}

bool CameraOPT::SetGainRGB(float Gain)
{
    return 1;
}

bool CameraOPT::SetFloatParam(FloatParamName name, float param)
{
    return 1;
}

bool CameraOPT::SetIntParam(IntParamName name, unsigned int param)
{
    return 1;
}

bool CameraOPT::SetEnumParam(EnumParamName name, unsigned int param)
{
    return 1;
}

bool CameraOPT::SetTriggerOnOrOff(bool isOn)
{
    return 1;
}

bool CameraOPT::SetAcquisitionMode(AcquisitionModeParam mode)
{
    return 1;
}

bool CameraOPT::SetBoolParam(BoolParamName name, bool tf)
{
    return 1;
}

bool CameraOPT::isCameraOpen()
{
    return 1;
}

bool CameraOPT::setPixelFormat(const unsigned int type)
{
    return 1;
}

bool CameraOPT::setGrabbing(const bool start)
{
    return 1;
}

void CameraOPT::OnImageCallback(GENICAM_Frame *frame)
{
    cv::Mat image;
    auto m_nBufSizeForSaveImage = frame->getImageSize(frame);
    //auto m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
    m_pBufForSaveImage.reset(new unsigned char[m_nBufSizeForSaveImage]);
    memcpy(m_pBufForSaveImage.get(),(char *)frame->getImage(frame),m_nBufSizeForSaveImage);

    bool isMono;//判断是否为黑白图像
    switch (frame->getImagePixelFormat(frame))
    {
    case gvspPixelMono1p:
    case gvspPixelMono2p:
    case gvspPixelMono4p:
    case gvspPixelMono8:
    case gvspPixelMono8S:
    case gvspPixelMono10:
    case gvspPixelMono10Packed:
    case gvspPixelMono12:
    case gvspPixelMono12Packed:
    case gvspPixelMono14:
    case gvspPixelMono16:
        isMono = true;
        break;
    default:
        isMono = false;
        break;
    }
    cv::Mat dstImage1;
    if (isMono)
    {
        image = cv::Mat(frame->getImageHeight(frame), frame->getImageWidth(frame), CV_8UC1, m_pBufForSaveImage.get());
        //cv::resize( image, dstImage1, Size(image.cols/2, image.rows/2), 0, 0, INTER_NEAREST );
    }
    else
    {
        cv::Mat mat16uc3_rgb(frame->getImageHeight(frame),frame->getImageWidth(frame),CV_8UC3);
        cv::Mat mat16uc1_bayer(frame->getImageHeight(frame), frame->getImageWidth(frame), CV_8UC1,m_pBufForSaveImage.get());
        cvtColor(mat16uc1_bayer,mat16uc3_rgb,cv::COLOR_BayerRG2RGB);

        cv::Mat mat8uc3_rgb(frame->getImageHeight(frame),frame->getImageWidth(frame),CV_8UC3);
        mat16uc3_rgb.convertTo(mat8uc3_rgb,CV_8UC3,1);
        image = mat8uc3_rgb;
    }

    if(m_callback_ != nullptr)//回调到control
    {
        m_callback_->hikImageResult(image,m_cameraIndex);
    }

}

bool CameraOPT::OpenCamera()
{
    int32_t ret;
    int32_t i;
    GENICAM_System *pSystem = nullptr;
    GENICAM_Camera *pCameraList = nullptr;
    GENICAM_AcquisitionControl *pAcquisitionCtrl = nullptr;
    GENICAM_AcquisitionControlInfo acquisitionControlInfo = {0};
    uint32_t cameraCnt = 0;    // discover camera    //发现设备
    ret = GENICAM_getSystemInstance(&pSystem);
    if (-1 == ret)    {
        qDebug()<<"pSystem is null.";
        return 0;
    }
    ret = pSystem->discovery(pSystem, &pCameraList, &cameraCnt, typeAll);
    if (-1 == ret)    {
        qDebug()<<"discovery device fail.";
        return 0;
    }
    if(cameraCnt < 1)    {
        qDebug()<<"there is no device.";
        return 0;
    }
    // print camera info (key, vendor name, model, serial number)    // 打印相机基本信息（key, 制造商信息, 型号, 序列号）
    for (i = 0; i < cameraCnt; i++)    {
        pCamera = &pCameraList[i];
        qDebug()<<"Camera Info :   "<< i;
        qDebug()<<"    key           = "<< pCamera->getKey(pCamera);
        qDebug()<<"    vendor name   = "<< pCamera->getVendorName(pCamera);
        qDebug()<<"    model         = "<< pCamera->getModelName(pCamera);
        qDebug()<<"    serial number = "<< pCamera->getSerialNumber(pCamera);
    }
    pCamera = &pCameraList[0];    // connect to camera    //连接设备
    ret = GENICAM_connect(pCamera);
    if(ret != 0)    {
        qDebug()<<"connect camera failed.";
        return 0;
    }
    //创建属性节点 AcquisitionControl
    acquisitionControlInfo.pCamera = pCamera;
    ret=GENICAM_createAcquisitionControl(&acquisitionControlInfo, &pAcquisitionCtrl);
    if ((ret != 0) || (NULL == pAcquisitionCtrl))    {
        qDebug()<<"Create Acquisition Control Fail.";
        return 0;
    }
    // create stream source instance
    //创建流对象
    ret = GENICAM_CreateStreamSource(pCamera, &pStreamSource);
    if((ret != 0) || (NULL == pStreamSource))    {
        qDebug()<<"create stream obj  fail.";
        return 0;
    }
    //注册回调函数
    ret = pStreamSource->attachGrabbingEx(pStreamSource, &onGetFrame,this);
    if (ret != 0)    {
        qDebug()<<"attch Grabbing fail!";
        pStreamSource->release(pStreamSource);
        return 0;
    }
}
int checkCameraSN(QString sn,GENICAM_Camera *pCameraList , int CameraSize){
    for(int index = 0 ; index < CameraSize ; index++){
        auto pCamera = &pCameraList[index];
        auto camerakey = QString(pCamera->getKey(pCamera));
        if(camerakey == sn){
            return index;
        }
    }
    return -1;
}

bool CameraOPT::OpenCamera(QString sn)
{
    int32_t ret;
    int32_t i;
    GENICAM_System *pSystem = nullptr;
    GENICAM_Camera *pCameraList = nullptr;
    GENICAM_AcquisitionControl *pAcquisitionCtrl = nullptr;
    GENICAM_AcquisitionControlInfo acquisitionControlInfo = {0};
    uint32_t cameraCnt = 0;    // discover camera    //发现设备
    ret = GENICAM_getSystemInstance(&pSystem);
    if (-1 == ret)    {
        qDebug()<<"pSystem is null.";
        return 0;
    }
    ret = pSystem->discovery(pSystem, &pCameraList, &cameraCnt, typeAll);
    if (-1 == ret)    {
        qDebug()<<"discovery device fail.";
        return 0;
    }
    if(cameraCnt < 1)    {
        qDebug()<<"there is no device.";
        return 0;
    }
    int index = checkCameraSN(sn,pCameraList,cameraCnt);
    if(index == -1){
        qDebug()<<"do not find Camera.\n";
        return 0;
    }
    pCamera = &pCameraList[index];    // connect to camera    //连接设备
    ret = GENICAM_connect(pCamera);
    if(ret != 0)    {
        qDebug()<<"connect camera failed.";
        return 0;
    }
    //创建属性节点 AcquisitionControl
    acquisitionControlInfo.pCamera = pCamera;
    ret=GENICAM_createAcquisitionControl(&acquisitionControlInfo, &pAcquisitionCtrl);
    if ((ret != 0) || (NULL == pAcquisitionCtrl))    {
        qDebug()<<"Create Acquisition Control Fail.";
        return 0;
    }
    // create stream source instance
    //创建流对象
    ret = GENICAM_CreateStreamSource(pCamera, &pStreamSource);
    if((ret != 0) || (NULL == pStreamSource))    {
        qDebug()<<"create stream obj  fail.";
        return 0;
    }
    //注册回调函数
    ret = pStreamSource->attachGrabbingEx(pStreamSource, &onGetFrame,this);
    if (ret != 0)    {
        qDebug()<<"attch Grabbing fail!";
        pStreamSource->release(pStreamSource);
        return 0;
    }
}

bool CameraOPT::CloseCamera()
{
    if(pCamera==nullptr)
        return 0;
    int32_t isDisconnectSuccess;
    isDisconnectSuccess = pCamera->disConnect(pCamera);
    if( isDisconnectSuccess != 0)    {
        qDebug()<<"disconnect fail.";
        return -1;
    }
    return 0;
}

bool CameraOPT::SetExposure(int num)
{
    int32_t isExposureTimeSuccess;
    GENICAM_DoubleNode doubleNode;
    double exposureTimeValue;
    GENICAM_AcquisitionControl *pAcquisitionCtrl = NULL;
    GENICAM_AcquisitionControlInfo acquisitionControlInfo = {0};
    acquisitionControlInfo.pCamera = pCamera;
    isExposureTimeSuccess = GENICAM_createAcquisitionControl(&acquisitionControlInfo, &pAcquisitionCtrl);
    if( isExposureTimeSuccess != 0)
    {
        qDebug()<<"ExposureTime  fail.\n";
        return -1;
    }
    exposureTimeValue = 0.0;
    doubleNode = pAcquisitionCtrl->exposureTime(pAcquisitionCtrl);
    isExposureTimeSuccess = doubleNode.getValue(&doubleNode, &exposureTimeValue);
    if( isExposureTimeSuccess != 0)
    {
        qDebug()<<"get exposureTime fail.\n";
        //注意：需要释放pAcquisitionCtrl内部对象内存
        pAcquisitionCtrl->release(pAcquisitionCtrl);
        //注意：需要释放doubleNode内部对象内存
        doubleNode.release(&doubleNode);
        return -1;
    }
    else
    {
        //注意：需要释放pAcquisitionCtrl内部对象内存
        pAcquisitionCtrl->release(pAcquisitionCtrl);
        qDebug()<<"before change ,exposureTime is %f\n",exposureTimeValue;
    }
    doubleNode.setValue(&doubleNode, (num));
    if( isExposureTimeSuccess != 0)
    {
        qDebug()<<"set exposureTime fail.\n";
        //注意：需要释放doubleNode内部对象内存
        doubleNode.release(&doubleNode);
        return -1;
    }
    doubleNode.getValue(&doubleNode, &exposureTimeValue);
    if( isExposureTimeSuccess != 0)
    {
        qDebug()<<"get exposureTime fail.\n";
        //注意：需要释放doubleNode内部对象内存
        doubleNode.release(&doubleNode);
        return -1;
    }
    else
    {
        qDebug()<<"after change ,exposureTime is ",exposureTimeValue;
        //注意：需要释放doubleNode内部对象内存
        doubleNode.release(&doubleNode);
    }
    return 0;
}

bool CameraOPT::SetTrigger(int type)
{
    int32_t isEnumNodeSuccess;
    uint32_t maxCnt;
    GENICAM_EnumNode *pEnumNode = NULL;
    GENICAM_EnumNodeInfo enumNodeInfo = {0};
    memcpy(enumNodeInfo.attrName, "TriggerMode", sizeof("TriggerMode"));
    isEnumNodeSuccess = GENICAM_createEnumNode(&enumNodeInfo, &pEnumNode);
    if (0 != isEnumNodeSuccess)    {
        qDebug()<<"GENICAM_createEnumNode fail.";
        return -1;
    }

    maxCnt = 256;

    isEnumNodeSuccess = pEnumNode->setValueBySymbol(pEnumNode, type ? "On" : "Off");
    if (0 != isEnumNodeSuccess)    {
        qDebug()<<"set TriggerMode fail.";        //注意：需要释放pEnumNode内部对象内存
        pEnumNode->release(pEnumNode);
        return -1;
    }

    return 0;
}

QStringList CameraOPT::GetCameraList()
{
    int32_t ret;
    int32_t i;
    QStringList retlist;
    GENICAM_System *pSystem = nullptr;
    GENICAM_Camera *pCameraList = nullptr;
    uint32_t cameraCnt = 0;    // discover camera    //发现设备
    ret = GENICAM_getSystemInstance(&pSystem);
    if (-1 == ret)    {
        qDebug()<<"pSystem is null.";
        return QStringList();
    }
    ret = pSystem->discovery(pSystem, &pCameraList, &cameraCnt, typeAll);
    for (i = 0; i < cameraCnt; i++)    {
        auto pCamera = &pCameraList[i];
        retlist.append(QString(pCamera->getKey(pCamera)));
    }
    return retlist;
}

bool CameraOPT::StartGenImg()
{
    // start grabbing from camera    //开始抓流
    if(pStreamSource == nullptr)
        return 0;
    int32_t isStartGrabbingSuccess;
    GENICAM_EGrabStrategy eGrabStrategy;
    eGrabStrategy = grabStrartegySequential;
    isStartGrabbingSuccess = pStreamSource->startGrabbing(pStreamSource, 0, eGrabStrategy);
    if( isStartGrabbingSuccess != 0)    {
        qDebug()<<"StartGrabbing  fail.";
        return 0;
    }
    return 1;
}

bool CameraOPT::StopGenImg()
{
    if(pStreamSource == nullptr)
        return 0;
    int32_t isStopGrabbingSuccess ,isdetachGrabbingEx;
    isdetachGrabbingEx = pStreamSource->detachGrabbingEx(pStreamSource, &onGetFrame,this);
    if(isdetachGrabbingEx != 0){
        qDebug()<<"detachGrabbingEx  fail.";
        return 0;
    }
    isStopGrabbingSuccess = pStreamSource->stopGrabbing(pStreamSource);
    if( isStopGrabbingSuccess != 0)    {
        qDebug()<<"StopGrabbing  fail.";
        return 0;
    }
    return 1;
}

bool CameraOPT::OneGenImg()
{
    if(pStreamSource == nullptr){
        qDebug()<<"pStreamSource  fail.";
        return 0;
    }
    int32_t isStopGrabbingSuccess;
    GENICAM_Frame *ppFrame;
    isStopGrabbingSuccess = pStreamSource->getFrame(pStreamSource, &ppFrame, 500);
    if( isStopGrabbingSuccess != 0)    {
        qDebug()<<"StopGrabbing  fail.";
        return 0;
    }
    onGetFrame(ppFrame,this);
    return 1;
}

