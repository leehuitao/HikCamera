#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <QDebug>
using namespace std;
using namespace cv;

const string filename_eyes = "./etc\\haarcascades\\haarcascade_eye.xml";
const string filename = "./etc\\haarcascades\\haarcascade_frontalface_alt.xml";
const string filename_face_lbp = "./etc\\lbpcascades\\lbpcascade_frontalface.xml";
const string filename_cat_face_lbp = "./etc\\lbpcascades\\lbpcascade_frontalcatface.xml";

CascadeClassifier imgFaceFier;
CascadeClassifier videoFaceFier;
CascadeClassifier videoFaceEyeFier;
CascadeClassifier camareFaceFier;
CascadeClassifier camareFaceEyeFier;
CascadeClassifier catFaceFier;
CascadeClassifier catFaceEyeFier;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //imgRead();
    //videoRead();
    //catRead();
    camereRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::imgRead()
{
    if(!imgFaceFier.load(filename)){
        qDebug()<<"load file error";
    }
    Mat src = imread("./1.jpg");
    if(src.empty()){
        qDebug()<<"load image error";
    }
    Mat gray;
    cvtColor(src,gray,COLOR_BGR2GRAY);
    equalizeHist(gray,gray);
    vector<Rect> faces;

    imgFaceFier.detectMultiScale(gray,faces,1.1,3,0);
    qDebug()<<"faces size "<<faces.size();
    for(int i=0;i<faces.size();i++){
        rectangle(src,faces[static_cast<int>(i)],Scalar(0,0,255),2,8,0);
    }
    namedWindow("123",WINDOW_NORMAL);
    imshow("123",src);
}

void MainWindow::videoRead()
{

}

void MainWindow::camereRead()
{
    if(!camareFaceFier.load(filename)){
        qDebug()<<"load file error";
    }
    if(!camareFaceEyeFier.load(filename_eyes)){
        qDebug()<<"load file error";
    }
    namedWindow("camera-demo",WINDOW_AUTOSIZE);

    VideoCapture capture;
    auto openstatus = capture.open(0);
    Mat frame;
    Mat gray;
    while(capture.read(frame)){
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        equalizeHist(gray,gray);
        vector<Rect> faces;
        vector<Rect> eyes;
        //camareFaceFier.detectMultiScale(gray,faces,1.1,3,0);
        qDebug()<<"faces size "<<faces.size();
        for(int i=0;i<faces.size();i++){
            //rectangle(frame,faces[static_cast<int>(i)],Scalar(0,0,255),2,8,0);
//            //眼部获取
//            Rect roi;//扫描眼睛的时候 只扫描上半部分
//            roi.x = faces[static_cast<int>(i)].x;
//            roi.y = faces[static_cast<int>(i)].y;
//            roi.width = faces[static_cast<int>(i)].width;
//            roi.height = faces[static_cast<int>(i)].height/2;

//            Mat faceROI = frame(roi);
//            camareFaceEyeFier.detectMultiScale(faceROI,eyes,1.2,3,0);
//             for(int j=0;j<eyes.size();j++){
//                 Rect r ;// 画框的时候原来的X位置是相对 相框的位置所以需要加上原来人员相框的xy
//                 r.x = faces[static_cast<int>(i)].x + eyes[j].x;
//                 r.y = faces[static_cast<int>(i)].y + eyes[j].y;
//                 r.width = eyes[j].width;
//                 r.height = eyes[j].height;
//                 rectangle(frame,r,Scalar(0,0,255),2,8,0);
//             }

        }
        imshow("camera-demo",frame);
        char c = waitKey(30);
        if(c == 27){
            break;
        }
    }
    waitKey(0);
}

void MainWindow::catRead()
{
    if(!catFaceFier.load(filename_cat_face_lbp)){
        qDebug()<<__FUNCTION__<<"load file error";
    }
    Mat src = imread("./2.jpg");
    if(src.empty()){
        qDebug()<<"load image error";
    }
    Mat gray;
    cvtColor(src,gray,COLOR_BGR2GRAY);
    equalizeHist(gray,gray);
    vector<Rect> faces;

    catFaceFier.detectMultiScale(gray,faces,1.3,3,0);
    qDebug()<<"faces size "<<faces.size();
    for(int i=0;i<faces.size();i++){
        rectangle(src,faces[static_cast<int>(i)],Scalar(0,0,255),2,8,0);
    }
    namedWindow("123",WINDOW_NORMAL);
    imshow("123",src);
}

