#include "xld_find_points.h"

XldFindPoints::XldFindPoints()
{

}

void XldFindPoints::loadHdvpEngine(QString hdvpPath)
{
    m_hdvpengine = new HDvpEngine();
    bool ret = m_hdvpengine->openHdvpFile(hdvpPath);
    if(!ret){
        qDebug()<<"load XldFindPoints HdvpEngine load error";
    }else{
        qDebug()<<"load XldFindPoints HdvpEngine load success";
    }
}

void XldFindPoints::findPointsDetection(const QImage &image, QString &code, QImage &resimg)
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
    QStringList list = code.split("|");
    code = "坐标:";
    if(list.size() == 2){
        auto list1 = list.at(0).split(",");
        auto list2 = list.at(1).split(",");
        for(int i=0 ;i<list1.size();i++){
            if(!list2.at(i).isEmpty())
                code +=  QString("(")+list2.at(i)+","+list1.at(i)+QString(")");
        }
    }
    qDebug()<<"result = "<<code;
}

bool XldFindPoints::doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result, QImage &image)
{
    //输出tuple
    QMap<QString, HTuple> outTuples;
    //输出object
    QMap<QString, HObject> outObjects;

    bool ret = m_hdvpengine->runOnce(inTuples, inObjects, outTuples, outObjects);

    if (ret == true)
    {
        //结果判断
        auto len = outTuples.find("result").value().Length();
        auto value = outTuples.find("result").value();
        QString x;
        QString y;
        for(int i = 0 ; i < len/2 ;i++){
            x += QString::number(value[i].D()) + ",";
            y += QString::number(value[i+(len/2)].D())+ ",";
        }
        result = x +"|"+y ;
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
