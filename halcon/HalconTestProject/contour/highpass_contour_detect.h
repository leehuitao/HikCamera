#ifndef HIGHPASSCONTOURDETECT_H
#define HIGHPASSCONTOURDETECT_H

#include "image_format.h"

class HighpassContourDetect
{
public:
    HighpassContourDetect();

    void loadHdvpEngine(QString number);

    void highpassDetection(const QImage & image, QString &code,QImage & resimg);
private:
    bool doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image,HDvpEngine * engine);

private:
    HDvpEngine *                m_highpassHdvpengine;
};

#endif // HIGHPASSCONTOURDETECT_H
