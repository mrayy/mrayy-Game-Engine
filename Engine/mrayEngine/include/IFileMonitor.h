

/********************************************************************
	created:	2014/01/29
	created:	29:1:2014   13:14
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\IFileMonitor.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine
	file base:	IFileMonitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IFileMonitor__
#define __IFileMonitor__


#include "ListenerContainer.h"
#include "IMonitorFileInformation.h"

namespace mray
{
namespace OS
{
	class IFileMonitor;
	class IFileMonitorListener
	{
	public:
		virtual void OnMonitorFiles(IFileMonitor* sender, IMonitorFileInformation* f,EMonitorAction action) = 0;
	};

class IFileMonitor:public ListenerContainer<IFileMonitorListener*>
{
protected:
	DECLARE_FIRE_METHOD(OnMonitorFiles, (IFileMonitor* sender, IMonitorFileInformation* f, EMonitorAction action), (sender, f,action));
public:
	IFileMonitor(){}
	virtual~IFileMonitor(){}

	virtual bool StartMonitoring() = 0;
	virtual void StopMonitoring() = 0;
	virtual bool IsMonitoring() = 0;

	virtual void SetDirectory(const core::string& path) = 0;
	virtual const core::string& GetDirectory() = 0;

};

}
}


#endif
