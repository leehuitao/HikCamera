#ifndef NVRENGINE_H
#define NVRENGINE_H

#include <QObject>
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#include "avglobal.h"
#include <opencv2/opencv.hpp>
class NvrEngine : public QObject
{
    Q_OBJECT
public:
    explicit NvrEngine(QObject *parent = nullptr);
    void initSdk();
    void login(QString ip ,int port , QString username , QString password);
    void startRealPlay(HWND handle);
    void stopRealPlay();
    void playBackByTime(HWND handle);
    void getBackByTime();
    void downloadByTime();
    void EndTest();
    void openDownloadFile(std::string url);
signals:

public slots:

private:
    LLONG m_loginID;
    LLONG g_lRealHandle;
    LLONG g_lDownloadHandle;
    LLONG g_lPlayHandle;


    cv::VideoCapture            m_cap;
};

#endif // NVRENGINE_H
