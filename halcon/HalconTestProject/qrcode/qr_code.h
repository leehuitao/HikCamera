#ifndef QRCODE_H
#define QRCODE_H

#include "image_format.h"

class QRCode
{
public:
    QRCode();

    void loadHdvpEngine(QString );

    void codeDetection(QString path, QString &code,QImage & resimg);

    void codeDetection(const QImage & image, QString &code,QImage & resimg);

    bool doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image);

private:
    HDvpEngine *                m_hdvpengine;
    Hlong                       m_imgWinID;
};

#endif // QRCODE_H
