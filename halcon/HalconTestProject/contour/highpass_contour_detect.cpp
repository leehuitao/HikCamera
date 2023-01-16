#include "highpass_contour_detect.h"

HighpassContourDetect::HighpassContourDetect()
{

}
void HighpassContourDetect::loadHdvpEngine(QString number)
{
    m_highpassHdvpengine = new HDvpEngine();
    bool ret = m_highpassHdvpengine->openHdvpFile(number);
    if(!ret){
        qDebug()<<"load highpassHdvpengine load error";
    }else{
        qDebug()<<"load highpassHdvpengine load success";
    }

}

void HighpassContourDetect::highpassDetection(const QImage &image, QString &code, QImage &resimg)
{
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
            doFuncCodeDetection(inTuples,inObjects,str,resimg,m_highpassHdvpengine);

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

bool HighpassContourDetect::doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result, QImage &image,HDvpEngine * engine)
{
    //输出tuple
    QMap<QString, HTuple> outTuples;
    //输出object
    QMap<QString, HObject> outObjects;

    bool ret = engine->runOnce(inTuples, inObjects, outTuples, outObjects);

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
