#include "qr_code.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent>
QRCode::QRCode()
{

}

void QRCode::loadHdvpEngine(QString hdvpPath)
{
    m_hdvpengine = new HDvpEngine();
    bool ret = m_hdvpengine->openHdvpFile(hdvpPath);
    if(!ret){
        qDebug()<<"loadHdvpEngine load error";
    }else{
        qDebug()<<"loadHdvpEngine load success";
    }
}

void QRCode::codeDetection(QString path, QString &code,QImage & resimg)
{
    QFuture<QString> res =  QtConcurrent::run([this,&path,&resimg]()
    {

        QMap<QString, HTuple> inTuples;
        QMap<QString, HObject> inObjects;
        HObject img;
        QString str;
        try
        {
            ReadImage(&img,path.toLocal8Bit().data());
            inObjects.insert("image",img);
            doFuncCodeDetection(inTuples,inObjects,str,resimg);

        }
        catch (HalconCpp::HException ex)
        {
            qDebug() << "Error when codeDetection:" << ex.ErrorMessage().Text();
            return QString();
        }
        return str;
    });
    while(!res.isFinished()) {
        QApplication::processEvents(QEventLoop::AllEvents, 200);
    }
    code = res.result();
    qDebug()<<"result = "<<code;
}

void QRCode::codeDetection(const QImage & image, QString & code,QImage & resimg)
{
    //    QString tmppath = "./imgtemp/tmp.jpg";
    //    image.save(tmppath);
    HImage himage; //直接输入图像数据
    qtimage2HImage(image,himage);
    QFuture<QString> res =  QtConcurrent::run([this,&himage,&resimg]()
    {

        QMap<QString, HTuple> inTuples;
        QMap<QString, HObject> inObjects;
        HObject img;
        QString str;
        try
        {
            //ReadImage(&img,tmppath.toLocal8Bit().data());
            inObjects.insert("image",himage);
            doFuncCodeDetection(inTuples,inObjects,str,resimg);

        }
        catch (HalconCpp::HException ex)
        {
            qDebug() << "Error when codeDetection:" << ex.ErrorMessage().Text();
            return QString();
        }
        return str;
    });
    while(!res.isFinished()) {
        QApplication::processEvents(QEventLoop::AllEvents, 200);
    }
    code = res.result();
    qDebug()<<"result = "<<code;
}


bool QRCode::doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image)
{

    //输出tuple
    QMap<QString, HTuple> outTuples;
    //输出object
    QMap<QString, HObject> outObjects;
    bool ret = m_hdvpengine->runOnce(inTuples, inObjects, outTuples, outObjects);

    if (ret == true)
    {
        //结果判断
        result = outTuples.find("result").value().S().Text();

        HObject res = outObjects.find("resultImage").value().Clone();
        if(res.IsInitialized()){
            HImage img(res);
            if(img.IsInitialized())
                halconimage2QImage(img,image);
        }
    }
    else
    {
        result = "";
    }

    return true;
}
