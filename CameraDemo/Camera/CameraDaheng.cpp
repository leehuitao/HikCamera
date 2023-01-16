#include "CameraDaheng.h"

bool CameraDaheng::Init()
{
	CameraBase::Init();

    bool ok = true;
    try
    {
        IGXFactory::GetInstance().Init();
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::UnInit()
{
	CameraBase::UnInit();

    bool ok = true;
    try
    {
        IGXFactory::GetInstance().Uninit();
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::OpenDevice()
{
    if (m_isOpened)
    {
        qDebug() << "CameraHik::CameraDaheng called, and camera is already opened.";
        return true;
    }

    bool ok = true;
    try
    {
        GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
        IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
        m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceByIP(vectorDeviceInfo[0].GetIP(), GX_ACCESS_EXCLUSIVE);
        m_isOpened = true;
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::CloseDevice()
{
    if (!m_isOpened)
    {
        qDebug() << "CameraDaheng::CloseDevice called, but camera is already closed.";
        return true;
    }

    bool ok = false;
    try
    {
        m_objDevicePtr->Close();
        m_isOpened = false;
        ok = true;
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
    }

    return ok;
}

bool CameraDaheng::StartGenImg()
{
    if (!m_isOpened)
    {
        qDebug() << "CameraDaheng::StartGenImg called, but camera is not opened.";
        OpenDevice();
    }

    bool ok = true;
    try
    {
        CGXStreamPointer objStreamPtr = m_objDevicePtr->OpenStream(0);
        //开启流通道
        objStreamPtr->StartGrab();
        //发送开采命令
        CGXFeatureControlPointer objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();
        objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
        //采单帧
        CImageDataPointer objImageDataPtr = objStreamPtr->GetImage(500);

        if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
        {
            LPBYTE din = (LPBYTE)(objImageDataPtr->GetBuffer());
            emit SendPhoto2GUI(din);
//          int strTime = time.elapsed();
//          qDebug()<<QString::number(strTime);
        }

        objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        objStreamPtr->StopGrab();
        objStreamPtr->Close();
//      objDevicePtr11->Close();
//      m_IsDevOpen = false;
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::OneGenImg()
{
    if (!m_isOpened)
    {
        qDebug() << "CameraDaheng::OneGenImg called, but camera is not opened.";
        OpenDevice();
    }

    bool ok = true;
    try
    {
        CGXStreamPointer objStreamPtr = m_objDevicePtr->OpenStream(0);
        //开启流通道
        objStreamPtr->StartGrab();
        //发送开采命令
        CGXFeatureControlPointer objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();
        objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
        //采单帧
        CImageDataPointer objImageDataPtr = objStreamPtr->GetImage(500);

        if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
        {
            LPBYTE din_test = (LPBYTE)(objImageDataPtr->GetBuffer());
            emit SendPhoto2TestGUI(din_test);
        }
        objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        objStreamPtr->StopGrab();
        objStreamPtr->Close();
//      objDevicePtr11->Close();
//      m_IsDevOpen = false;
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::SetExposureTime(float ExposureTime)
{
    if (!m_isOpened)
    {
        qDebug() << "CameraDaheng::SetExposureTime called, but camera is not opened.";
        OpenDevice();
    }

    bool ok = true;
    try
    {
        CGXFeatureControlPointer objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();
        CFloatFeaturePointer objFloatPtr = objFeatureControlPtr->GetFloatFeature("ExposureTime");
        objFloatPtr->SetValue(ExposureTime);
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}

bool CameraDaheng::GetCameraInfo(QVector<CameraInfo> &infos)
{
	bool ok = true;
	try
	{
		IGXFactory::GetInstance().Init();

		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

        for (auto &i : vectorDeviceInfo)
		{
			CameraInfo info;
			info.vendor = i.GetVendorName();
			info.model = i.GetModelName();
			info.sn = i.GetSN();
			info.mac = i.GetMAC();
			info.ip = i.GetIP();

			infos.push_back(info);
		}
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException &e)
	{
		m_lastError = e.what();
		emit CamError(m_lastError);
		ok = false;
	}

	return ok;
}

bool CameraDaheng::GetImageFormat(int &Width, int &Height)
{
    if (!m_isOpened)
    {
        qDebug() << "CameraHik::CameraDaheng called, but camera is not opened.";
        OpenDevice();
    }

    bool ok = true;
    try
    {
        CGXFeatureControlPointer objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();
        CIntFeaturePointer objIntPtr = objFeatureControlPtr->GetIntFeature("Width");
        Width = objIntPtr->GetValue();
        objIntPtr = objFeatureControlPtr->GetIntFeature("Height");
        Height = objIntPtr->GetValue();
    }
    catch (CGalaxyException &e)
    {
        m_lastError = e.what();
        emit CamError(m_lastError);
        ok = false;
    }

    return ok;
}
