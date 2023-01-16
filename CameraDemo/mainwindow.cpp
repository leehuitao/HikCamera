#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initLog();
    m_camera1 = new CameraOPT;
    m_camera1->setCameraIndex(1);
    m_camera1->setCallBack(this);
    m_camera1->Init();
    m_camera2 = new CameraHik;
    m_camera2->setCameraIndex(1);
    m_camera2->setCallBack(this);
    m_camera2->Init();
    m_camera3 = new DahuaCamera;
    m_camera3->setCameraIndex(1);
    m_camera3->setCallBack(this);
    m_camera3->Init();
    bool status = m_camera1->OpenCamera();
    bool status1 = m_camera2->OpenDevice();
    bool status2 = m_camera3->OpenDevice();
    if(status){
        m_camera1->StartGenImg();
    }
    if(status1){
        m_camera2->StartGenImg();
    }
    if(status2){
        m_camera3->StartGenImg();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
using namespace QsLogging;
void MainWindow::hikImageResult(cv::Mat frame, int cameraindex)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"cameraindex = "<<cameraindex;
}
void logFunction(const QString &message, QsLogging::Level level)
{

}

#define LogMaxSizeMultiple  1024*1024

void MainWindow::initLog()
{

    QString logPath        = "./";
    int logLevel       = 0;
    int logMaxSize     = 100;
    int logMaxOldLog   = 10;

    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(Level(logLevel));
    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                       logPath+"log.txt", EnableLogRotation,
                                       MaxSizeBytes(logMaxSize*LogMaxSizeMultiple),
                                       MaxOldLogCount(logMaxOldLog)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    logger.addDestination(functorDestination);
}


