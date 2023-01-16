#include "CameraBase.h"

bool CameraBase::Init()
{
    if (m_workerThread != nullptr)
    {
		this->moveToThread(m_workerThread);
		m_workerThread->start();
    }

    return true;
}

bool CameraBase::UnInit()
{
    if (m_isOpened)
    {
        CloseDevice();
    }

	if (m_workerThread != nullptr && m_workerThread->isRunning())
	{
		m_workerThread->quit();
		m_workerThread->wait();
	}

    if (m_workerThread != nullptr)
    {
        delete m_workerThread;
        m_workerThread = nullptr;
    }

    return true;
}
