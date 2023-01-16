#ifndef CAMERAOPT_H
#define CAMERAOPT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <QImage>
#include "GenICam/CAPI/SDK.h"
#include <QObject>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Camera/CameraHik.h"

class CameraOPT: public CameraBase
{
    Q_OBJECT
public:
    CameraOPT();
public Q_SLOTS:
    void clearIsFristPic(){
        m_cam4IsFristPic = 1;
    }
    bool OpenCamera();
    bool OpenCamera(QString sn);
    bool CloseCamera();
    bool SetExposure(int num);
    bool SetTrigger(int type);
    QStringList GetCameraList();
    bool StartGenImg();
    bool StopGenImg();
    bool OneGenImg();
    void OnImageCallback(GENICAM_Frame * frame);
    void setCallBack(HikImageCallBack * callbaclk){m_callback_ = callbaclk;}
    void setCameraIndex(int index){m_cameraIndex = index;}
Q_SIGNALS:
    void signOnImage(QImage );
private:
    std::unique_ptr<unsigned char[]>    m_pBufForSaveImage;
    GENICAM_Camera *                    pCamera = nullptr;
    GENICAM_StreamSource *              pStreamSource = nullptr;
    HikImageCallBack *                  m_callback_ = nullptr;
    int                                 m_cameraIndex;
    bool                                m_cam4IsFristPic = 1;
private:

    int32_t GENICAM_CreateStreamSource(GENICAM_Camera *pGetCamera, GENICAM_StreamSource **ppStreamSource);
    int32_t GENICAM_connect(GENICAM_Camera *pGetCamera);
    QImage MatToQImage(cv::Mat mtx);

    // CameraBase interface
public:
    QString name();

public Q_SLOTS:
    bool OpenDevice();
    bool OpenDeviceForFrame();
    bool CloseDevice();
    bool SetExposureTime(float ExposureTime);
    bool GetCameraInfo(QVector<CameraInfo> &infos);
    bool GetCurrentCameraInfo(CameraInfo &info);
    bool GetImageFormat(int &Width, int &Height);
    bool GetImageFormatForFrame(int &Width, int &Height);
    bool SetGain(float Gain);
    bool GetImg();
    bool StartImg();
    bool EndImg();
    bool SetTriggerMode(const QString &mode);
    bool TriggerSoftwareExecute();
    bool OpenDeviceRGB();
    bool SetExposureTimeRGB(float ExposureTime);
    bool GetImageFormatRGB(int &Width, int &Height);
    bool SetGainRGB(float Gain);
    bool SetFloatParam(FloatParamName name, float param);
    bool SetIntParam(IntParamName name, unsigned int param);
    bool SetEnumParam(EnumParamName name, unsigned int param);
    bool SetTriggerOnOrOff(bool isOn);
    bool SetAcquisitionMode(AcquisitionModeParam mode);
    bool SetBoolParam(BoolParamName name, bool tf);
    bool isCameraOpen();
    bool setPixelFormat(const unsigned int type);
    bool setGrabbing(const bool start);
};

#endif // CAMERAOPT_H
