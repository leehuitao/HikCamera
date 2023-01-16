#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMutexLocker>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Camera/CameraHik.h"
#include <QFuture>
#include <QtConcurrent>
#include <QObject>
#include <QVector>
#include <QMap>
#include "Log/QsLog.h"
#include "Log/QsLogDest.h"
#include <QUuid>
#include "CameraOPT/camera_opt.h"
#include "CameraDahua/dahuacamera.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow,public HikImageCallBack
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief hikImageResult
     * @param Mat  回调的图像数据
     * @return
     */
    virtual void hikImageResult(cv::Mat, int cameraindex );
    void initLog();
private:
    Ui::MainWindow *ui;
    CameraOPT *                 m_camera1 = nullptr;
    CameraHik *                 m_camera2 = nullptr;
    DahuaCamera*                m_camera3 = nullptr;
};
#endif // MAINWINDOW_H
