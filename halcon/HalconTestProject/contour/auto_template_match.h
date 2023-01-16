#ifndef AUTOTEMPLATEMATCH_H
#define AUTOTEMPLATEMATCH_H

#include "image_format.h"

class AutoTemplateMatch
{
public:
    AutoTemplateMatch();

    void loadHdvpEngine(QString shapModel);

    void createShapModel(const QImage & image);

    void shapModelDetection(const QImage & image, QString &code,QImage & resimg);
private:
    bool doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image,HDvpEngine * engine);

private:
    HDvpEngine *                m_highpassHdvpengine;
};

#endif // AUTOTEMPLATEMATCH_H
