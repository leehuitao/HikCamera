#ifndef CAMHIK_H
#define CAMHIK_H

#include "CameraBase.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "TlFactory.h"
#include "CameraParams.h"
#include "MvGigEDevice.h"
#include "MvCameraControl.h"
#include "MvUsb3VDevice.h"
#include <QImage>
#include <iostream>

class HikImageCallBack{

public:
    HikImageCallBack(){}
    virtual ~HikImageCallBack(){}
    virtual void hikImageResult(cv::Mat , int cameraindex) = 0 ;
};
class CameraHik : public CameraBase
{
    Q_OBJECT
public:
    ~CameraHik() override;
    QString name() override {return "Hik";}
    void setIndex(int index) {m_index = index;}
    void OnImageCallback(unsigned char *pData,MV_FRAME_OUT_INFO_EX *);
    QString cameraType() {return m_cameraType;}


    bool OpenDeviceBySN(const QString cameraName);
    QStringList GetCameraList();
    int CheckCameraIndex(const QString SerialNumber);//确定对应的sn的相机的序号
    void initTriggerSource();
    QMap<int, QString> GetTriggerSourceList();
    QVector<int> GetSupportTriggerSource();
    QString cameraName() {return m_cameraName;}
    static bool checkCameraVender(QString vender);//判断相机是否属于海康，属于返回true
    void setCallBack(HikImageCallBack * callbaclk){m_callback_ = callbaclk;}
    void setCameraIndex(int index){m_cameraIndex = index;}
protected:
    void *m_hHandle = nullptr;
    int m_index = 0;
    unsigned int nBufSize;
    QString m_cameraType;//相机类型
    unsigned int m_pixelFormat;//图像像素类型
    QString m_sn = "";
    QString m_cameraName = "";
    QMap<int, QString> m_mapTriggerSource;
    QVector<int> m_supportTriggerSource;
    int m_cameraIndex;

protected:




public Q_SLOTS:
    bool StartGenImg() override;
    void stopGenImg();
    bool OneGenImg() override;
    bool OpenDevice() override;
    bool OpenDeviceForFrame() override;
    bool CloseDevice() override;
    bool SetExposureTime(float ExposureTime) override;
	bool GetCameraInfo(QVector<CameraInfo> &infos) override;
    bool GetCurrentCameraInfo(CameraInfo &info) override;
    bool GetImageFormat(int &Width, int &Height) override;
    bool GetImageFormatForFrame(int &Width, int &Height) override;
    bool SetGain(float Gain) override;
    bool StartImg() override;
    bool GetImg() override;
    bool EndImg() override;
    bool SetTriggerMode(const QString &mode) override;//设置触发源
    bool SetTriggerSource(const unsigned int source);
    bool TriggerSoftwareExecute() override;//软触发一次
    bool OpenDeviceRGB() override;
    bool SetExposureTimeRGB(float ExposureTime) override;
    bool SetGainRGB(float Gain) override;
    bool GetImageFormatRGB(int &Width, int &Height) override;
    bool SetFloatParam(FloatParamName name, float param) override;
    bool SetIntParam(IntParamName name, unsigned int param) override;
    bool SetEnumParam(EnumParamName name, unsigned int param) override;
    bool SetTriggerOnOrOff(bool isOn) override;//设置触发模式
    bool SetAcquisitionMode(AcquisitionModeParam mode) override;//设置采集模式
    bool SetBoolParam(BoolParamName name, bool tf) override;
    bool isCameraOpen() override;
    bool setPixelFormat(const unsigned int type) override;
    bool setGrabbing(const bool start) override;


private:
    bool startGrabImage(QImage &image,MV_FRAME_OUT_INFO_EX *stImageInfo,unsigned char* data);
    std::unique_ptr<unsigned char[]> m_pBufForSaveImage;
    HikImageCallBack * m_callback_ = nullptr;
Q_SIGNALS:
    QVector<int> SigSendImageData(LPBYTE img, QString sn);
    void signSendCVMat(cv::Mat);
    void signSendQIamge(QImage);


};

#endif // CAMHIK_H
