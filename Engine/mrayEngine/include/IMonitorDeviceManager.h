

/********************************************************************
	created:	2012/09/26
	created:	26:9:2012   17:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IMonitorDeviceManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IMonitorDeviceManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IMonitorDeviceManager___
#define ___IMonitorDeviceManager___

#include "ISingleton.h"


namespace mray
{
namespace video
{
	class IMonitorDevice;

class IMonitorDeviceManager:public ISingleton<IMonitorDeviceManager>
{
protected:

public:
	IMonitorDeviceManager(){}
	virtual~IMonitorDeviceManager(){}
	

	virtual void Clear()=0;
	virtual void Refresh(bool clearCheck=true)=0;

	virtual IMonitorDevice* GetMonitorFromPoint(const math::vector2di& pos)=0;
	virtual const std::vector<IMonitorDevice*>& GetMonitors()=0;
	virtual IMonitorDevice* GetMonitor(int index)=0;

};

}
}

#endif
