


/********************************************************************
	created:	2012/10/15
	created:	15:10:2012   18:00
	filename: 	d:\Development\mrayEngine\Plugins\DirectShowVideo\FlyCameraManager.h
	file path:	d:\Development\mrayEngine\Plugins\DirectShowVideo
	file base:	FlyCameraManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___FlyCameraManager___
#define ___FlyCameraManager___

#include "FlyCapture2.h"


namespace mray
{
namespace video
{

class FlyCameraManager
{
protected:


	int m_refCount;

	FlyCapture2::BusManager m_busManager;
public:
	FlyCameraManager();
	virtual~FlyCameraManager();

	void AddRef();
	void SubRef();

	int GetCamerasCount();
	bool GetCamera(int index,FlyCapture2::PGRGuid& out);


	void LogError(FlyCapture2::Error e);

	static FlyCameraManager instance;
};

}
}

#endif