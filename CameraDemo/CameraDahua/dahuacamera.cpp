#include "dahuacamera.h"
#include "QsLog.h"

DahuaCamera::DahuaCamera(QObject *parent)
{

}

QString DahuaCamera::name()
{
    return "";
}

// 取流回调函数
// get frame callback function
void DahuaCamera::FrameCallback(const CFrame &frame)
{
    CFrameInfo frameInfo;
    //    frameInfo.m_nWidth = frame.getImageWidth();
    //    frameInfo.m_nHeight = frame.getImageHeight();
    //    frameInfo.m_nBufferSize = frame.getImageSize();
    //    frameInfo.m_nPaddingX = frame.getImagePadddingX();
    //    frameInfo.m_nPaddingY = frame.getImagePadddingY();
    //    frameInfo.m_ePixelType = frame.getImagePixelFormat();
    //    frameInfo.m_pImageBuf = (BYTE *)malloc(sizeof(BYTE) * frameInfo.m_nBufferSize);
    //    frameInfo.m_nTimeStamp = frame.getImageTimeStamp();
    //    m_width = frameInfo.m_nWidth;
    //    m_height = frameInfo.m_nHeight;

    //    memcpy(frameInfo.m_pImageBuf, frame.getImage(), frame.getImageSize());
    recvNewFrame(frame, frameInfo);
}



bool DahuaCamera::CameraChangeTrig(DahuaCamera::ETrigType trigType)
{
    if (NULL == m_pCamera)
    {
        printf("Change Trig fail. No camera or camera is not connected.\n");
        return false;
    }

    if (trigContinous == trigType)
    {
        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("Off"))
        {
            printf("set TriggerMode value = Off fail.\n");
            return false;
        }
    }
    else if (trigSoftware == trigType)
    {
        // 设置触发器
        // set trigger
        CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
        if (false == nodeTriggerSelector.isValid())
        {
            printf("get TriggerSelector node fail.\n");
            return false;
        }
        if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
        {
            printf("set TriggerSelector value = FrameStart fail.\n");
            return false;
        }

        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("On"))
        {
            printf("set TriggerMode value = On fail.\n");
            return false;
        }

        // 设置触发源为软触发
        // set triggerSource as software trigger
        CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
        if (false == nodeTriggerSource.isValid())
        {
            printf("get TriggerSource node fail.\n");
            return false;
        }
        if (false == nodeTriggerSource.setValueBySymbol("Software"))
        {
            printf("set TriggerSource value = Software fail.\n");
            return false;
        }
    }
    else if (trigLine == trigType)
    {
        // 设置触发器
        // set trigger
        CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
        if (false == nodeTriggerSelector.isValid())
        {
            printf("get TriggerSelector node fail.\n");
            return false;
        }
        if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
        {
            printf("set TriggerSelector value = FrameStart fail.\n");
            return false;
        }

        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("On"))
        {
            printf("set TriggerMode value = On fail.\n");
            return false;
        }

        // 设置触发源为Line1触发
        // set trigggerSource as Line1 trigger
        CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
        if (false == nodeTriggerSource.isValid())
        {
            printf("get TriggerSource node fail.\n");
            return false;
        }
        if (false == nodeTriggerSource.setValueBySymbol("Line1"))
        {
            printf("set TriggerSource value = Line1 fail.\n");
            return false;
        }
    }

    return true;
}

bool DahuaCamera::SetExecuteSoftTrig()
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CCmdNode nodeTriggerSoftware(m_pCamera, "TriggerSoftware");
    if (false == nodeTriggerSoftware.isValid())
    {
        QLOG_ERROR() << QString("Get TriggerSoftware node fail.");
        printf("get TriggerSoftware node fail.\n");
        return false;
    }
    if (false == nodeTriggerSoftware.execute())
    {
        QLOG_ERROR() << QString("Set TriggerSoftware fail.");
        return false;
    }

    return true;
}

bool DahuaCamera::SetTriggerSource(const QString source)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CEnumNode enumNode(m_pCamera, "TriggerSource");
    if (false == enumNode.isValid())
    {
        QLOG_ERROR() << QString("Get TriggerSource node failed.");
        return false;
    }

    bool bRet = enumNode.setValueBySymbol(source.toLocal8Bit().data());
    if (bRet != true)
    {
        QLOG_ERROR() << QString("Set trigger source failed.");
        return false;
    }

    return true;
}

QStringList DahuaCamera::GetSupportTriggerSourceList()
{
    CEnumNode enumNode(m_pCamera, "TriggerSource");

    QStringList list;
    if (false == enumNode.isValid())
    {
        QLOG_ERROR() << QString("Get TriggerSource node failed.");
        return list;
    }

    TVector<CString> vec = enumNode.getEnumSymbolList();
    TVector<CString>::iterator it = vec.begin();
    while (it != vec.end())
    {
        list.append(it->c_str());
        it++;
    }

    return list;
}


bool DahuaCamera::SetAdjustPlus(double dGainRaw)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CDoubleNode nodeGainRaw(m_pCamera, "GainRaw");

    if (false == nodeGainRaw.isValid())
    {
        QLOG_ERROR() << QString("Get GainRaw node fail.");
        return false;
    }

    if (false == nodeGainRaw.isAvailable())
    {
        QLOG_ERROR() << QString("GainRaw is not available.");
        return false;
    }

    if (false == nodeGainRaw.setValue(dGainRaw))
    {
        QLOG_ERROR() << QString("Set GainRaw value = %1 fail.").arg(dGainRaw);
        return false;
    }

    return true;
}

QStringList DahuaCamera::GetCameraList()
{
    CSystem &systemObj = CSystem::getInstance();
    if (false == systemObj.discovery(m_vCameraPtrList))
    {
        QLOG_ERROR() << "Dahua camera discover camera failed!";
        return m_cameraNameList;
    }

    if (m_vCameraPtrList.size() < 1)
    {
        return m_cameraNameList;
    }
    else
    {
        for (int i = 0; i < int(m_vCameraPtrList.size()); i++)
        {
            if (checkCameraVender(m_vCameraPtrList[i]->getVendorName()) == true)
            {
                QString name = QString("%1&%2&%3").arg(m_vCameraPtrList[i]->getVendorName(), m_vCameraPtrList[i]->getModelName(),
                                                       m_vCameraPtrList[i]->getSerialNumber());
                m_cameraNameList.append(name);
            }
        }
    }

    return m_cameraNameList;
}

bool DahuaCamera::StartGenImg()
{
    if (m_pCamera == NULL)
        return false;

    if (NULL == m_pStreamSource)
    {
        m_pStreamSource = CSystem::getInstance().createStreamSource(m_pCamera);
    }

    if (NULL == m_pStreamSource)
    {
        return false;
    }

    //读取相机像素格式
    CheckCameraPixelFormat();

    if (m_pStreamSource->isGrabbing())
    {
        return true;
    }

    bool bRet = m_pStreamSource->attachGrabbing(IStreamSource::Proc(&DahuaCamera::FrameCallback, this));
    if (!bRet)
    {
        return false;
    }

    if (!m_pStreamSource->startGrabbing())
    {
        return false;
    }

    return true;
}

bool DahuaCamera::StopGenImg()
{
    if (m_pStreamSource != NULL)
    {
        m_pStreamSource->detachGrabbing(IStreamSource::Proc(&DahuaCamera::FrameCallback, this));

        m_pStreamSource->stopGrabbing();
        m_pStreamSource.reset();
    }

    return true;
}

bool DahuaCamera::OneGenImg()
{
    return 1;
}

bool DahuaCamera::OpenDevice()
{
    auto list = GetCameraList();
    if(list.size() > 0 )
        return OpenDevice(list.at(0));
    else
        return 0;
}

bool DahuaCamera::OpenDeviceForFrame()
{
    return 1;
}

bool DahuaCamera::SetExposureTime(float ExposureTime)
{
    return 1;
}

bool DahuaCamera::GetCameraInfo(QVector<CameraInfo> &infos)
{
    return 1;
}

bool DahuaCamera::GetCurrentCameraInfo(CameraInfo &info)
{
    return 1;
}

bool DahuaCamera::GetImageFormat(int &Width, int &Height)
{
    return 1;
}

bool DahuaCamera::GetImageFormatForFrame(int &Width, int &Height)
{
    return 1;
}

bool DahuaCamera::SetGain(float Gain)
{
    return 1;
}

bool DahuaCamera::StartImg()
{
    return 1;
}

bool DahuaCamera::GetImg()
{
    return 1;
}

bool DahuaCamera::EndImg()
{
    return 1;
}

bool DahuaCamera::SetTriggerMode(ETrigType trigType)
{
    if (NULL == m_pCamera)
    {
        printf("Change Trig fail. No camera or camera is not connected.\n");
        return false;
    }

    if (trigContinous == trigType)
    {
        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("Off"))
        {
            printf("set TriggerMode value = Off fail.\n");
            return false;
        }
    }
    else if (trigSoftware == trigType)
    {
        // 设置触发器
        // set trigger
        CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
        if (false == nodeTriggerSelector.isValid())
        {
            printf("get TriggerSelector node fail.\n");
            return false;
        }
        if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
        {
            printf("set TriggerSelector value = FrameStart fail.\n");
            return false;
        }

        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("On"))
        {
            printf("set TriggerMode value = On fail.\n");
            return false;
        }

        // 设置触发源为软触发
        // set triggerSource as software trigger
        CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
        if (false == nodeTriggerSource.isValid())
        {
            printf("get TriggerSource node fail.\n");
            return false;
        }
        if (false == nodeTriggerSource.setValueBySymbol("Software"))
        {
            printf("set TriggerSource value = Software fail.\n");
            return false;
        }
    }
    else if (trigLine == trigType)
    {
        // 设置触发器
        // set trigger
        CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
        if (false == nodeTriggerSelector.isValid())
        {
            printf("get TriggerSelector node fail.\n");
            return false;
        }
        if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
        {
            printf("set TriggerSelector value = FrameStart fail.\n");
            return false;
        }

        // 设置触发模式
        // set trigger mode
        CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
        if (false == nodeTriggerMode.isValid())
        {
            printf("get TriggerMode node fail.\n");
            return false;
        }
        if (false == nodeTriggerMode.setValueBySymbol("On"))
        {
            printf("set TriggerMode value = On fail.\n");
            return false;
        }

        // 设置触发源为Line1触发
        // set trigggerSource as Line1 trigger
        CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
        if (false == nodeTriggerSource.isValid())
        {
            printf("get TriggerSource node fail.\n");
            return false;
        }
        if (false == nodeTriggerSource.setValueBySymbol("Line1"))
        {
            printf("set TriggerSource value = Line1 fail.\n");
            return false;
        }
    }

    return true;

}

bool DahuaCamera::SetTriggerSource(const unsigned int source)
{
    return 1;
}

bool DahuaCamera::TriggerSoftwareExecute()
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CCmdNode nodeTriggerSoftware(m_pCamera, "TriggerSoftware");
    if (false == nodeTriggerSoftware.isValid())
    {
        QLOG_ERROR() << QString("Get TriggerSoftware node fail.");
        printf("get TriggerSoftware node fail.\n");
        return false;
    }
    if (false == nodeTriggerSoftware.execute())
    {
        QLOG_ERROR() << QString("Set TriggerSoftware fail.");
        return false;
    }

    return true;
}

bool DahuaCamera::OpenDeviceRGB()
{
    return 1;
}

bool DahuaCamera::SetExposureTimeRGB(float ExposureTime)
{
    return 1;
}

bool DahuaCamera::SetGainRGB(float Gain)
{
    return 1;
}

bool DahuaCamera::GetImageFormatRGB(int &Width, int &Height)
{
    return 1;
}

bool DahuaCamera::SetFloatParam(FloatParamName name, float param)
{
    return 1;
}

bool DahuaCamera::SetIntParam(IntParamName name, unsigned int param)
{
    return 1;
}

bool DahuaCamera::SetEnumParam(EnumParamName name, unsigned int param)
{
    return 1;
}

bool DahuaCamera::SetTriggerOnOrOff(bool on)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
    if (false == nodeTriggerMode.isValid())
    {
        QLOG_ERROR() << "Get TriggerMode node failed.";
        return false;
    }

    bool ret = true;
    if (on == true)
    {
        ret = nodeTriggerMode.setValueBySymbol("On");
    }
    else
    {
        ret = nodeTriggerMode.setValueBySymbol("Off");
    }

    if (ret == false)
    {
        QLOG_ERROR() << "Set TriggerMode failed.";
        return false;
    }

    return true;
}

bool DahuaCamera::SetAcquisitionMode(AcquisitionModeParam mode)
{
    return 1;
}

bool DahuaCamera::SetBoolParam(BoolParamName name, bool tf)
{
    return 1;
}

bool DahuaCamera::isCameraOpen()
{
    return 1;
}

bool DahuaCamera::setPixelFormat(const unsigned int type)
{
    return 1;
}

bool DahuaCamera::setGrabbing(const bool start)
{
    return 1;
}

QString DahuaCamera::GetCameraKey(const QString camera)
{
    if (m_vCameraPtrList.size() < 1)
    {
        GetCameraList();
    }
    QStringList list = camera.split("&");
    if (list.size() != 3)
        return "";

    QString key = "";
    for (int i = 0; i < int(m_vCameraPtrList.size()); ++i)
    {
        if (m_vCameraPtrList[i]->getSerialNumber() == list.at(2))
        {
            key = m_vCameraPtrList[i]->getKey();
        }
    }

    return key;
}

void DahuaCamera::CheckCameraPixelFormat()
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return;
    }

    CEnumNode node(m_pCamera, "PixelFormat");
    if (false == node.isValid())
    {
        QLOG_ERROR() << "Get PixelFormat node fail.";
        return;
    }

    uint64_t pxiel;
    CString symbol;
    node.getValue(pxiel);
    node.getValueSymbol(symbol);
    m_pixelFormat = pxiel;
    m_pixelFormatStr = symbol.c_str();
}

uint64_t DahuaCamera::GetPixelPormat()
{
    if (m_pixelFormat == uint64_t(-1))
    {
        CheckCameraPixelFormat();
    }

    return m_pixelFormat;
}

QString DahuaCamera::GetPixelPormatName()
{
    if (m_pixelFormatStr.length() == 0)
    {
        CheckCameraPixelFormat();
    }

    return m_pixelFormatStr;
}

bool DahuaCamera::SetTriggerMode(const bool on)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
    if (false == nodeTriggerMode.isValid())
    {
        QLOG_ERROR() << "Get TriggerMode node failed.";
        return false;
    }

    bool ret = true;
    if (on == true)
    {
        ret = nodeTriggerMode.setValueBySymbol("On");
    }
    else
    {
        ret = nodeTriggerMode.setValueBySymbol("Off");
    }

    if (ret == false)
    {
        QLOG_ERROR() << "Set TriggerMode failed.";
        return false;
    }

    return true;
}

bool DahuaCamera::SetTriggerDelay(const double delayTime)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CDoubleNode nodeGainRaw(m_pCamera, "TriggerDelay");

    if (false == nodeGainRaw.isValid())
    {
        QLOG_ERROR() << QString("Get TriggerDelay node fail.");
        return false;
    }

    if (false == nodeGainRaw.isAvailable())
    {
        QLOG_ERROR() << QString("TriggerDelay is not available.");
        return false;
    }

    if (false == nodeGainRaw.setValue(delayTime))
    {
        QLOG_ERROR() << QString("Set TriggerDelay value = %1 fail.").arg(delayTime);
        return false;
    }

    return true;
}

bool DahuaCamera::SetBalanceWhiteAuto(const int mode)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CEnumNode node(m_pCamera, "BalanceWhiteAuto");

    if (false == node.isValid())
    {
        QLOG_ERROR() << QString("Get BalanceWhiteAuto node fail.");
        return false;
    }

    if (false == node.isAvailable())
    {
        QLOG_ERROR() << QString("BalanceWhiteAuto is not available.");
        return false;
    }

    if (false == node.setValue(mode))
    {
        QLOG_ERROR() << QString("Set BalanceWhiteAuto value = %1 fail.").arg(mode);
        return false;
    }

    return true;
}

bool DahuaCamera::IsCameraOpen()
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << __FUNCTION__ << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    return m_pCamera->isConnected();
}

void DahuaCamera::recvNewFrame(const CFrame &frame, CFrameInfo &info)
{
    int ConvertType = 0; //0:无需转换 1:转为RGB24 2:转为mono8
    switch (info.m_ePixelType)
    {
    case Dahua::GenICam::gvspPixelMono8:
        ConvertType = 0;
        break;
    case Dahua::GenICam::gvspPixelMono10:
        ConvertType = 2;
        break;
    case Dahua::GenICam::gvspPixelMono10Packed:
        ConvertType = 2;
        break;
    case Dahua::GenICam::gvspPixelMono12:
        ConvertType = 2;
        break;
    case Dahua::GenICam::gvspPixelMono12Packed:
        ConvertType = 2;
        break;
    case Dahua::GenICam::gvspPixelBayRG8:
        ConvertType = 1;
        break;
    case Dahua::GenICam::gvspPixelBayRG10:
        ConvertType = 1;
        break;
    case Dahua::GenICam::gvspPixelBayRG12:
        ConvertType = 1;
        break;
    case Dahua::GenICam::gvspPixelBayRG10Packed:
        ConvertType = 1;
        break;
    case Dahua::GenICam::gvspPixelBayRG12Packed:
        ConvertType = 1;
        break;
    case Dahua::GenICam::gvspPixelYUV422_8_UYVY:
        ConvertType = 1;
        break;
    default:
        ConvertType = 1;
        break;
    }

    if (ConvertType == 0)
    {
        cv::Mat image = cv::Mat(frame.getImageHeight(),
                                frame.getImageWidth(),
                                CV_8U,
                                (uint8_t*)frame.getImage());
        signSendCVMat(image);
        if(m_callback_!= nullptr){
            m_callback_->hikImageResult(image,m_cameraIndex);
        }
    }
    else if (ConvertType == 1)
    {
        // 转码
        // transcoding
        uint8_t *pRGBbuffer = NULL;
        int nRgbBufferSize = 0;
        nRgbBufferSize = info.m_nWidth * info.m_nHeight * 3;
        pRGBbuffer = (uint8_t *)malloc(nRgbBufferSize);
        if (pRGBbuffer == NULL)
        {
            // 释放内存
            // release memory
            free(info.m_pImageBuf);
            printf("RGBbuffer malloc failed.\n");
        }

        IMGCNV_SOpenParam openParam;
        openParam.width = info.m_nWidth;
        openParam.height = info.m_nHeight;
        openParam.paddingX = info.m_nPaddingX;
        openParam.paddingY = info.m_nPaddingY;
        openParam.dataSize = info.m_nBufferSize;
        openParam.pixelForamt = info.m_ePixelType;

        IMGCNV_EErr status = IMGCNV_ConvertToRGB24(info.m_pImageBuf, &openParam, pRGBbuffer, &nRgbBufferSize);
        if (IMGCNV_SUCCESS != status)
        {
            // 释放内存
            // release memory
            printf("IMGCNV_ConvertToRGB24 failed.\n");
            free(info.m_pImageBuf);
            free(pRGBbuffer);
            return;
        }

        // 释放内存
        // release memory
        free(info.m_pImageBuf);
        sigSendImage(pRGBbuffer, m_cameraName);
    }
    else
    {
        // 转码
        // transcoding
        uint8_t *pRGBbuffer = NULL;
        int nRgbBufferSize = 0;
        nRgbBufferSize = info.m_nWidth * info.m_nHeight * 3;
        pRGBbuffer = (uint8_t *)malloc(nRgbBufferSize);
        if (pRGBbuffer == NULL)
        {
            // 释放内存
            // release memory
            free(info.m_pImageBuf);
            printf("RGBbuffer malloc failed.\n");
        }

        IMGCNV_SOpenParam openParam;
        openParam.width = info.m_nWidth;
        openParam.height = info.m_nHeight;
        openParam.paddingX = info.m_nPaddingX;
        openParam.paddingY = info.m_nPaddingY;
        openParam.dataSize = info.m_nBufferSize;
        openParam.pixelForamt = info.m_ePixelType;

        IMGCNV_EErr status = IMGCNV_ConvertToMono8(info.m_pImageBuf, &openParam, pRGBbuffer, &nRgbBufferSize);
        if (IMGCNV_SUCCESS != status)
        {
            // 释放内存
            // release memory
            printf("IMGCNV_ConvertToMono8 failed.\n");
            free(info.m_pImageBuf);
            free(pRGBbuffer);
            return;
        }

        // 释放内存
        // release memory
        free(info.m_pImageBuf);
        sigSendImage(pRGBbuffer, m_cameraName);
    }
}

bool DahuaCamera::checkCameraVender(QString vender)
{
    QStringList list;
    list.append("Dahua");
    list.append("Huaray");
    for (int i = 0; i < list.size(); ++i)
    {
        if (vender.contains(list[i], Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

bool DahuaCamera::OpenDevice(const QString &sn)
{
    CSystem &systemObj = CSystem::getInstance();
    m_pCamera = systemObj.getCameraPtr(GetCameraKey(sn).toStdString().c_str());

    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Connect camera failed! No camera %1.").arg(sn);
        return false;
    }

    if (true == m_pCamera->isConnected())
    {
        QLOG_ERROR() << QString("%1 camera is already connected.").arg(sn);
        return true;
    }

    if (false == m_pCamera->connect())
    {
        QLOG_ERROR() << QString("Connect camera %1 failed!").arg(sn);
        return false;
    }

    m_cameraName = sn;
    return true;
}

bool DahuaCamera::SetExposureTime(int dExposureTime)
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Camera %1 is not connected.").arg(m_cameraName);
        return false;
    }

    CDoubleNode nodeExposureTime(m_pCamera, "ExposureTime");

    if (false == nodeExposureTime.isValid())
    {
        QLOG_ERROR() << QString("Get ExposureTime node fail.");
        return false;
    }

    if (false == nodeExposureTime.isAvailable())
    {
        QLOG_ERROR() << QString("ExposureTime is not available.");
        return false;
    }

    if (false == nodeExposureTime.setValue(dExposureTime))
    {
        QLOG_ERROR() << QString("Set ExposureTime value = %1 fail.").arg(dExposureTime);
        return false;
    }

    return true;
}

bool DahuaCamera::SetTriggerMode(const QString &mode)
{
    return 1;
}

bool DahuaCamera::CloseDevice()
{
    if (NULL == m_pCamera)
    {
        QLOG_ERROR() << QString("Connect camera failed! No camera.");
        return false;
    }

    if (false == m_pCamera->isConnected())
    {
        QLOG_ERROR() << QString("camera is already disconnected.");
        return true;
    }

    if (false == m_pCamera->disConnect())
    {
        QLOG_ERROR() << QString("Disconnect camera fail.");
        return false;
    }

    return true;
}



