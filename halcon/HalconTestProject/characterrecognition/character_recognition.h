#ifndef CHARACTERRECOGNITION_H
#define CHARACTERRECOGNITION_H

#include "image_format.h"

class CharacterRecognition
{
public:
    CharacterRecognition();

    void loadHdvpEngine(QString number,QString english);

    void numberDetection(const QImage & image, QString &code,QImage & resimg);

    void englishDetection(const QImage & image, QString &code,QImage & resimg);

    bool doFuncCodeDetection(QMap<QString, HTuple> inTuples, QMap<QString, HObject> inObjects, QString &result,QImage & image,HDvpEngine * engine);

private:
    HDvpEngine *                m_numberHdvpengine;
    HDvpEngine *                m_englishHdvpengine;
};

#endif // CHARACTERRECOGNITION_H
