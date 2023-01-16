#include "auto_template_match.h"

AutoTemplateMatch::AutoTemplateMatch()
{

}

void AutoTemplateMatch::loadHdvpEngine(QString shapModel)
{
    m_highpassHdvpengine = new HDvpEngine();
    bool ret = m_highpassHdvpengine->openHdvpFile(shapModel);
    if(!ret){
        qDebug()<<"load shapModelHdvpengine load error";
    }else{
        qDebug()<<"load shapModelHdvpengine load success";
    }
}

void AutoTemplateMatch::createShapModel(const QImage &image)
{
    HImage himage; //直接输入图像数据
    qtimage2HImage(image,himage);
    try
    {
        HObject grayImage;
        // Local iconic variables
        HObject  ho_Image, ho_GrayImage, ho_ROI_0, ho_ImageReduced;
        HObject  ho_ImageMedian, ho_ImageEmphasize, ho_Highpass;
        HObject  ho_Region, ho_ConnectedRegions, ho_ObjectSelected;
        HObject  ho_SelectedRegions, ho_ObjectSelected1, ho_RegionFillUp;
        HObject  ho_ImageReduced1, ho_ImageR, ho_ImageG, ho_ImageB;
        HObject  ho_resultImage;

        // Local control variables
        HTuple  hv_Width, hv_Height, hv_Area, hv_Row;
        HTuple  hv_Column, hv_Area1, hv_Indices, hv_Length, hv_Area2;
        HTuple  hv_Row1, hv_Column1, hv_Number, hv_result, hv_ModelID;
        HTuple  hv_Row2, hv_Column2, hv_Angle, hv_Scale, hv_Score;

        Rgb1ToGray(himage,&grayImage);
        qDebug()<<__FUNCTION__<<__LINE__;
        MedianImage(grayImage, &ho_ImageMedian, "circle", 21, "mirrored");

        Emphasize(ho_ImageMedian, &ho_ImageEmphasize, 10, 10, 5);

        HighpassImage(ho_ImageEmphasize, &ho_Highpass, 21, 21);

        Threshold(ho_Highpass, &ho_Region, 132, 255);

        Connection(ho_Region, &ho_ConnectedRegions);

        AreaCenter(ho_ConnectedRegions, &hv_Area1, &hv_Row, &hv_Column);

        TupleSortIndex(hv_Area1, &hv_Indices);

        TupleLength(hv_Area1, &hv_Length);

        SelectObj(ho_ConnectedRegions, &ho_ObjectSelected, HTuple(hv_Indices[hv_Length-2])+1);

        SelectObj (ho_ObjectSelected, &ho_ObjectSelected1, 1);

        FillUp(ho_ObjectSelected1, &ho_RegionFillUp);

        ReduceDomain(grayImage, ho_RegionFillUp, &ho_ImageReduced1);
        qDebug()<<__FUNCTION__<<__LINE__;
        CreateScaledShapeModel(ho_ImageReduced1, "auto", 0, 6.28, "auto", 0.8, 1.2, "auto",
              "auto", "ignore_local_polarity", "auto", "auto", &hv_ModelID);
        qDebug()<<__FUNCTION__<<__LINE__;
        WriteShapeModel(hv_ModelID,"D:/test.shm");

    }
    catch (HalconCpp::HException ex)
    {
        qDebug()<<__FUNCTION__<<"error "<<ex.ErrorMessage().Text();
    }
}

void AutoTemplateMatch::shapModelDetection(const QImage &image, QString &code, QImage &resimg)
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

bool AutoTemplateMatch::doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result, QImage &image, HDvpEngine *engine)
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
