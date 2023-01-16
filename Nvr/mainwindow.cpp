#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    NvrEngine nvr;
    auto handle = this->winId();
    //nvr.startRealPlay(HWND(handle));//实时播放
    //nvr.playBackByTime(HWND(handle));//回放
    //nvr.downloadByTime();// 按时间下载视频
    std::string url = "D:/LHT/TestProject/NVR/build-Nvr-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug/test.dav";
    nvr.openDownloadFile(url);
}

MainWindow::~MainWindow()
{
    delete ui;
}

