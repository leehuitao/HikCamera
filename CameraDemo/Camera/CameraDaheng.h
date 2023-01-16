#ifndef CAMDAHENG_H
#define CAMDAHENG_H

#include "CameraBase.h"
#include <GalaxyIncludes.h>

class CameraDaheng : public CameraBase
{
    Q_OBJECT
public:
    QString name() override {return "Daheng";}

protected:
    CGXDevicePointer m_objDevicePtr;

public slots:
    bool StartGenImg() override;
    bool OneGenImg() override;
    bool OpenDevice() override;
    bool CloseDevice() override;
    bool Init() override;
    bool UnInit() override;
    bool SetExposureTime(float ExposureTime) override;
	bool GetCameraInfo(QVector<CameraInfo> &infos) override;
    bool GetImageFormat(int &Width, int &Height) override;
};

#endif // CAMDAHENG_H
