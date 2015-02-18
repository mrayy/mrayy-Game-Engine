

/********************************************************************
	created:	2014/01/29
	created:	29:1:2014   13:46
	filename: 	C:\Development\mrayEngine\Engine\mrayWin32\WinFileMonitor.h
	file path:	C:\Development\mrayEngine\Engine\mrayWin32
	file base:	WinFileMonitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __WinFileMonitor__
#define __WinFileMonitor__

#include "IFileMonitor.h"
#include "IThread.h"


namespace mray
{
namespace OS
{

	class FileMonitorThreadFunction;
class WinFileMonitor:public IFileMonitor
{
protected:
	//DECLARE_FIRE_METHOD()
	core::string m_directory;
	bool m_running;
	GCPtr<IThread> m_thread;
	friend class FileMonitorThreadFunction;
public:
	WinFileMonitor();
	virtual~WinFileMonitor();

	virtual bool StartMonitoring();
	virtual void StopMonitoring() ;
	virtual bool IsMonitoring() ;

	virtual void SetDirectory(const core::string& path) ;
	virtual const core::string& GetDirectory() ;


	//////////////////////////////////////////////////////////////////////////
	void _FailedCreateThread();
};

}
}


#endif
