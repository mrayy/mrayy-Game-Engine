

/********************************************************************
	created:	2011/06/12
	created:	12:6:2011   9:07
	filename: 	d:\Development\mrayEngine\Engine\mrayWin32\WinProcess.h
	file path:	d:\Development\mrayEngine\Engine\mrayWin32
	file base:	WinProcess
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __WinProcess__
#define __WinProcess__

#include "CompileConfig.h"
#include "ISystemProcess.h"

#include <windows.h>

namespace mray
{
namespace OS
{


class MRAY_Win32_DLL WinProcess:public ISystemProcess
{
private:
protected:

	core::string m_currentDirectory;
	core::string m_application;

	std::vector<core::string> m_arguments;

	PROCESS_INFORMATION m_pinfo;
	ProcessStartupInfo m_startupInfo;

	IStreamWriterListener* m_writerListener;

	HANDLE m_child_stdIn_Read;
	HANDLE m_child_stdIn_Write;
	HANDLE m_child_stdOut_Read;
	HANDLE m_child_stdOut_Write;
public:
	WinProcess();
	virtual~WinProcess();

	virtual void Close();

	virtual void SetApplicationName(const core::string& name);
	virtual const core::string& GetApplicationName();

	virtual void SetCurrentDirectory(const core::string& name);
	virtual const core::string& GetCurrentDirectory();

	virtual std::vector<core::string>& GetArguments();

	virtual ProcessStartupInfo& GetProcessStartupInfo();

	virtual bool Run();

	void OnWriteData(const void*data,size_t count);
};

}
}

#endif
