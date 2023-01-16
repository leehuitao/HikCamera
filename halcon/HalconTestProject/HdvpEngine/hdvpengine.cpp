#include "hdvpengine.h"
#include <QFileInfo>
#include <QVariant>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
HDvpEngine::HDvpEngine(QObject *parent) : QObject(parent)
{
    /* 如果本地安装的版本不是18.0需要
     * 在mian中添加 "qputenv("HALCONROOT", QByteArray(QCoreApplication::applicationDirPath().toUtf8()))"
     * */
    m_engine = new HDevEngine;
}

bool HDvpEngine::openHdvpFile(QString path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.suffix() != "hdvp")
    {
        qDebug() << __FUNCTION__ << "file type is not hdvp";
        return false;
    }
    QString dir = fileInfo.absoluteDir().path();
    QString fileName = fileInfo.fileName().remove(".hdvp");

    try
    {
        //设置程序文件夹
        m_engine->AddProcedurePath((char *)dir.toUtf8().data());
        //实例化程序
        HDevProcedure proc;
        proc.LoadProcedure((char *)fileName.toUtf8().data());
        //绑定程序到触发器
        if (m_procCall != nullptr)
        {
            qDebug() << __FUNCTION__ << fileName;
            delete m_procCall;
            m_procCall = nullptr;
        }

        m_listOutputTupleNames.clear();
        for (int i = 0; i < proc.GetOutputCtrlParamCount(); ++i)
        {
            m_listOutputTupleNames.append(proc.GetOutputCtrlParamName(i + 1));
        }

        m_listOutputObjectNames.clear();
        for (int i = 0; i < proc.GetOutputIconicParamCount(); ++i)
        {
            m_listOutputObjectNames.append(proc.GetOutputIconicParamName(i + 1));
        }

        m_procCall = new HDevProcedureCall(proc);
    }
    catch (HDevEngineException ex)
    {
        qDebug() << __FUNCTION__ << ex.CategoryText();
        return false;
    }

    return true;
}

bool HDvpEngine::runOnce(QMap<QString, HTuple> inputTuples, QMap<QString, HObject> inputObjects, QMap<QString, HTuple> &outputTuples,
                         QMap<QString, HObject> &outputObjects)
{
    if (m_procCall == nullptr)
    {
        qDebug() << __FUNCTION__ << "HDevProcedureCall is null";
        return false;
    }

    try
    {
        //设置tuple
        QMap<QString, HTuple>::iterator it_tuple = inputTuples.begin();
        while (it_tuple != inputTuples.end())
        {
            try
            {
                m_procCall->SetInputCtrlParamTuple(it_tuple.key().toUtf8().data(), it_tuple.value());
            }
            catch (HDevEngineException ex)
            {
                qDebug() << __FUNCTION__ << __LINE__ << it_tuple.key() << ex.CategoryText();
            }
            it_tuple++;
        }

        //设置object
        QMap<QString, HObject>::iterator it_object = inputObjects.begin();
        while (it_object != inputObjects.end())
        {
            try
            {
                m_procCall->SetInputIconicParamObject(it_object.key().toUtf8().data(), it_object.value());
            }
            catch (HDevEngineException ex)
            {
                qDebug() << __FUNCTION__ << __LINE__ << it_object.key() << ex.CategoryText();
            }
            it_object++;
        }


        //运行
        qDebug() << __FUNCTION__ << __LINE__ << "RUN";
        m_procCall->Execute();

        //获取结果
        outputTuples.clear();
        outputTuples.clear();
        for (int i = 0; i < m_listOutputTupleNames.size(); ++i)
        {
            auto result = m_procCall->GetOutputCtrlParamTuple(m_listOutputTupleNames[i].toLatin1().data());
            outputTuples.insert(m_listOutputTupleNames[i], m_procCall->GetOutputCtrlParamTuple(m_listOutputTupleNames[i].toLatin1().data()));
        }
        for (int i = 0; i < m_listOutputObjectNames.size(); ++i)
        {
            outputObjects.insert(m_listOutputObjectNames[i], m_procCall->GetOutputIconicParamObject(m_listOutputObjectNames[i].toLatin1().data()));
        }
    }
    catch (HDevEngineException ex)
    {
        //halcon奔溃时打印在哪一行出的问题，和报错信息方便改错
        qDebug() << __FUNCTION__  << ex.CategoryText()<<" line = "<<ex.ProgLineNum()<<" message = "<<ex.Message();
        return false;
    }

    return true;
}

