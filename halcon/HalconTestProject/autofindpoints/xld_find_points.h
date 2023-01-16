#ifndef XLDFINDPOINTS_H
#define XLDFINDPOINTS_H

#include "image_format.h"
struct Points{
    QString x;
    QString y;
};
class XldFindPoints
{
public:
    XldFindPoints();
    void loadHdvpEngine(QString );

    void findPointsDetection(const QImage & image, QString &code,QImage & resimg);

    bool doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image);

private:
    HDvpEngine *                m_hdvpengine;
    QList<Points>               m_points;
};

#endif // XLDFINDPOINTS_H
