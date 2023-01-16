#ifndef HDVPENGINE_H
#define HDVPENGINE_H

#include <QObject>
#include "Halcon.h"
#include "HalconCpp.h"
#include "hdevengine/HDevEngineCpp.h"
#include <QMap>

using namespace HalconCpp;
using namespace HDevEngineCpp;

class HDvpEngine : public QObject
{
    Q_OBJECT
public:
    explicit HDvpEngine(QObject *parent = nullptr);
    bool openHdvpFile(QString path);
    bool runOnce(QMap<QString, HTuple> inputTuples, QMap<QString, HObject> inputObjects,
                 QMap<QString, HTuple> &outputTuples, QMap<QString, HObject> &outputObjects);

signals:

protected:
    HDevEngine *m_engine;
    HDevProcedureCall *m_procCall = nullptr;
    QStringList m_listOutputTupleNames;
    QStringList m_listOutputObjectNames;

};

Q_DECLARE_METATYPE(HObject);
Q_DECLARE_METATYPE(HTuple);
#endif // HDVPENGINE_H
