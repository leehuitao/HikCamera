#ifndef CAMERABASE_H
#define CAMERABASE_H

#include <Windows.h>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QVector>
#include <QMutex>

class CameraInfo
{
public:
	QString vendor;
	QString model;
	QString sn;
	QString mac;
	QString ip;
};

enum FloatParamName
{
    ExposureTime = 0,            //曝光时间
    Gain,                       //增益值
    TriggerDelay,               //触发延时
    ResultingFrameRate,         //相机的实际采集帧率
    AutoGainLowerLimit,         //自动增益值下限
    AutoGainupperLimit,         //自动增益值上限
    BlackLevel,                 //黑电平调节
    Gamma,                      //伽马调节
    BalanceRatio,                   //白平衡数值

};

enum IntParamName
{
    AcquisitionLineRate = 0,       //行频（线扫）

};

enum EnumParamName
{
    BalanceWhiteAuto = 0,          //自动白平衡，0为Off，1为Continuous，2为Once
    BalanceRatioSelector = 0,      //白平衡分量，0为R，1为G，2为B
    TriggerMode = 0,
    TriggerSource,
    TriggerActivation,
    ExposureAuto,
    GainAuto,
};

enum AcquisitionModeParam
{
    SingleFrame = 0,
    MultiFrame,
    Continuous
};

enum BoolParamName
{
    LUTEnable,
};


class CameraBase : public QObject
{
    Q_OBJECT
public:
    CameraBase()
	{
        m_workerThread = new QThread;
	}

    virtual ~CameraBase()
	{
		if (m_workerThread != nullptr)
		{
			delete m_workerThread;
		}
	}

    virtual QString name() = 0;
    QString lastError() {return m_lastError;}

protected:
    QString m_lastError;
    QThread *m_workerThread = nullptr;


public:
    bool m_isOpened = false;

Q_SIGNALS:
    void SendPhoto2GUI(LPBYTE, int);
    void SendPhoto2TestGUI(LPBYTE);
    void CamError(const QString);

public Q_SLOTS:
    virtual bool Init();
    virtual bool UnInit();

    virtual bool StartGenImg() = 0;
    virtual bool OneGenImg() = 0;
    virtual bool OpenDevice() = 0;
    virtual bool OpenDeviceForFrame() = 0;
    virtual bool CloseDevice() = 0;
    virtual bool SetExposureTime(float ExposureTime) = 0;
	virtual bool GetCameraInfo(QVector<CameraInfo> &infos) = 0;
    virtual bool GetCurrentCameraInfo(CameraInfo &info) = 0;
    virtual bool GetImageFormat(int &Width, int &Height) = 0;
    virtual bool GetImageFormatForFrame(int &Width, int &Height) = 0;
    virtual bool SetGain(float Gain) = 0;
    virtual bool GetImg() = 0;
    virtual bool StartImg() = 0;
    virtual bool EndImg() = 0;
    virtual bool SetTriggerMode(const QString &mode) = 0;
    virtual bool TriggerSoftwareExecute() = 0;
    virtual bool OpenDeviceRGB() = 0;
    virtual bool SetExposureTimeRGB(float ExposureTime) = 0;
    virtual bool GetImageFormatRGB(int &Width, int &Height) = 0;
    virtual bool SetGainRGB(float Gain) = 0;
    virtual bool SetFloatParam(FloatParamName name, float param) = 0;
    virtual bool SetIntParam(IntParamName name, unsigned int param) = 0;
    virtual bool SetEnumParam(EnumParamName name, unsigned int param) = 0;
    virtual bool SetTriggerOnOrOff(bool isOn) = 0;
    virtual bool SetAcquisitionMode(AcquisitionModeParam mode) = 0;
    virtual bool SetBoolParam(BoolParamName name, bool tf) = 0;
    virtual bool isCameraOpen() = 0;
    virtual bool setPixelFormat(const unsigned int type) = 0;
    virtual bool setGrabbing(const bool start) = 0;
};
#endif // CAMERABASE_H
