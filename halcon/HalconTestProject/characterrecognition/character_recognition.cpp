#include "character_recognition.h"

CharacterRecognition::CharacterRecognition()
{

}

void CharacterRecognition::loadHdvpEngine(QString number, QString english)
{
    m_numberHdvpengine = new HDvpEngine();
    bool ret = m_numberHdvpengine->openHdvpFile(number);
    m_englishHdvpengine= new HDvpEngine();
    bool ret1 = m_englishHdvpengine->openHdvpFile(english);
    if(!ret){
        qDebug()<<"load numberHdvpengine load error";
    }else{
        qDebug()<<"load numberHdvpengine load success";
    }
    if(!ret1){
        qDebug()<<"load englishHdvpengine load error";
    }else{
        qDebug()<<"load englishHdvpengine load success";
    }
}

void CharacterRecognition::numberDetection(const QImage &image, QString &code, QImage &resimg)
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
            doFuncCodeDetection(inTuples,inObjects,str,resimg,m_numberHdvpengine);

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

void CharacterRecognition::englishDetection(const QImage &image, QString &code, QImage &resimg)
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
            doFuncCodeDetection(inTuples,inObjects,str,resimg,m_englishHdvpengine);

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

bool CharacterRecognition::doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result, QImage &image,HDvpEngine * engine)
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
