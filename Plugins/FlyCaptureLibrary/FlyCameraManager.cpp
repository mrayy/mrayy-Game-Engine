
#include "stdafx.h"
#include "FlyCameraManager.h"
#include "ILogManager.h"



namespace mray
{
namespace video
{



FlyCameraManager::FlyCameraManager()
{
	m_busManager = new FlyCapture2::BusManager();
	m_refCount=0;
}
FlyCameraManager::~FlyCameraManager()
{
	delete m_busManager;
}

void FlyCameraManager::AddRef()
{
	if(m_refCount==0)
	{
	}
	++m_refCount;
}
void FlyCameraManager::SubRef()
{
	--m_refCount;
	if(m_refCount==0)
	{
	}
}

int FlyCameraManager::GetCamerasCount()
{
	uint cCount=0;
	FlyCapture2::Error e;
	e=m_busManager->GetNumOfCameras(&cCount);
	if(e!=FlyCapture2::PGRERROR_OK)
	{
		LogError(e);
		return 0;
	}
	return cCount;
}
bool FlyCameraManager::GetCamera(int index,FlyCapture2::PGRGuid& out)
{
	FlyCapture2::Error e = m_busManager->GetCameraFromIndex(index, &out);
	if (e != FlyCapture2::PGRERROR_OK)
	{
		LogError( e );
		return false;
	}
	return true;

}
bool FlyCameraManager::GetCameraSerialNumber(int index, unsigned int& pSerialNumber)
{
	FlyCapture2::Error e = m_busManager->GetCameraSerialNumberFromIndex(index, &pSerialNumber);
	if (e != FlyCapture2::PGRERROR_OK)
	{
		LogError(e);
		return false;
	}
	return true;

}


void FlyCameraManager::LogError(FlyCapture2::Error e)
{
	core::string eStr= e.GetDescription();
	gLogManager.log("FlyCamera: "+eStr,ELL_WARNING);
}
	
}
}