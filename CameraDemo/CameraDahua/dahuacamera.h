#ifndef DAHUACAMERA_H
#define DAHUACAMERA_H

#include <QObject>
#include <QThread>
#include "GenICam/System.h"
#include "Media/VideoRender.h"
#include "Media/ImageConvert.h"
#include "GenICam/Camera.h"
#include "GenICam/StreamSource.h"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include "GenICam/ImageFormatControl.h"
#include "Camera/CameraHik.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#define printf qDebug

using namespace Dahua::GenICam;
using namespace Dahua::Infra;
// 帧信息
// frame imformation
class CFrameInfo : public Dahua::Memory::CBlock
{
public:
    CFrameInfo()
    {
        m_pImageBuf = NULL;
        m_nBufferSize = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_ePixelType = Dahua::GenICam::gvspPixelMono8;
        m_nPaddingX = 0;
        m_nPaddingY = 0;
        m_nTimeStamp = 0;
    }

    ~CFrameInfo()
    {
    }

public:
    BYTE		*m_pImageBuf;
    int			m_nBufferSize;
    int			m_nWidth;
    int			m_nHeight;
    Dahua::GenICam::EPixelType	m_ePixelType;
    int			m_nPaddingX;
    int			m_nPaddingY;
    uint64_t	m_nTimeStamp;
};

class DahuaCamera : public CameraBase
{
    Q_OBJECT
public:
    DahuaCamera(QObject *parent = nullptr);

    enum ETrigType
    {
        trigContinous = 0,	// 连续拉流 | continue grabbing
        trigSoftware = 1,	// 软件触发 | software trigger
        trigLine = 2,		// 外部触发	| external trigger
    };
    QString name() override;
    // 取流回调函数
    // get frame callback function
    void FrameCallback(const Dahua::GenICam::CFrame &frame);
    // 切换采集方式、触发方式 （连续采集、外部触发、软件触发）
    // Switch acquisition mode and triggering mode (continuous acquisition, external triggering and software triggering)
    bool CameraChangeTrig(ETrigType trigType = trigContinous);
    // 执行一次软触发
    // Execute a soft trigger
    bool SetExecuteSoftTrig();
    // 设置触发源
    bool SetTriggerSource(const QString source);
    // 获取支持的触发源列表
    QStringList GetSupportTriggerSourceList();
    // 设置增益
    // set gain
    bool SetAdjustPlus(double dGainRaw);
    //  获取图像宽
    int GetWidth() {return m_width;}
    //  获取图像高
    int GetHeight() {return m_height;}
    // 获取相机的key
    QString GetCameraKey(const QString camera);
    // 检查相机格式
    void CheckCameraPixelFormat();
    // 获取相机像素格式
    uint64_t GetPixelPormat();
    // 获取相机像素格式名称
    QString GetPixelPormatName();
    // 设置触发模式 on off
    bool SetTriggerMode(const bool on);
    // 设置触发延时
    bool SetTriggerDelay(const double delayTime);
    // 设置自动白平衡 0:off 1:once 2:continue
    bool SetBalanceWhiteAuto(const int mode);

    bool IsCameraOpen();

    static bool checkCameraVender(QString vender);//判断相机是否属于大华，属于返回true

Q_SIGNALS:
    void sigSendImage(uint8_t *image, const QString camera);
public Q_SLOTS:
public:

    bool OpenDevice(IN const QString & sn);
    // 设置曝光
    // set exposuse time
    bool SetExposureTime(IN int num);
    bool SetTriggerMode(const QString &mode) override;//设置触发源
    bool SetTriggerMode(ETrigType trigType = trigContinous);//设置触发源
    bool SetTriggerSource(const unsigned int source);
    bool TriggerSoftwareExecute() override;//软触发一次
    // 获取支持的触发源列表
    QStringList GetCameraList();
    // 开始采集
    // start grabbing
    bool StartGenImg()override;
    // 停止采集
    // stop grabbing
    bool StopGenImg();

    bool OneGenImg()override;
    void stopGenImg();
    void setCallBack(HikImageCallBack * callbaclk){m_callback_ = callbaclk;}
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
    void setCameraIndex(int index){m_cameraIndex = index;}
private:
    // 状态栏统计信息
    // Status bar statistics
    void recvNewFrame(const CFrame &pBuf, CFrameInfo &info);
    void updateStatistic();
private:
    Dahua::GenICam::ICameraPtr m_pCamera;							// 当前相机 | current camera
    Dahua::GenICam::IStreamSourcePtr m_pStreamSource;				// 流对象   |  stream object
    int m_width;
    int m_height;
    QStringList m_cameraNameList;
    Dahua::Infra::TVector<Dahua::GenICam::ICameraPtr> m_vCameraPtrList;
    QString m_cameraName;
    uint64_t m_pixelFormat = 0;
    QString m_pixelFormatStr = "";
    HikImageCallBack *  m_callback_;
    int m_cameraIndex;
private:
    std::unique_ptr<unsigned char[]> m_pBufForSaveImage;
Q_SIGNALS:
    QVector<int> SigSendImageData(LPBYTE img, QString sn);
    void signSendCVMat(cv::Mat);
    void signSendQIamge(QImage);


};

#endif // DAHUACAMERA_H
