#include "stdafx.h"

#include "WinProcess.h"
#include "OSLoggerSystem.h"
#include "StreamWriter.h"


namespace mray
{
namespace OS
{

	class WinProcessStreamWriterListener:public IStreamWriterListener
	{
		WinProcess* m_owner;
	public:
		WinProcessStreamWriterListener(WinProcess* owner):m_owner(owner)
		{
		}
		void OnWriteData(const void*data,size_t count)
		{
			m_owner->OnWriteData(data,count);
		}
	};

WinProcess::WinProcess():m_child_stdIn_Read(0),m_child_stdIn_Write(0),
		m_child_stdOut_Read(0),m_child_stdOut_Write(0),m_writerListener(0)
{
	memset(&m_pinfo,0,sizeof(m_pinfo));
}
WinProcess::~WinProcess()
{
}



void WinProcess::SetApplicationName(const core::string& name)
{
	m_application=name;
}
const core::string& WinProcess::GetApplicationName()
{
	return m_application;
}

void WinProcess::SetCurrentDirectory(const core::string& name)
{
	m_currentDirectory=name;
}
const core::string& WinProcess::GetCurrentDirectory()
{
	return m_currentDirectory;
}

std::vector<core::string>& WinProcess::GetArguments()
{
	return m_arguments;
}

ProcessStartupInfo& WinProcess::GetProcessStartupInfo()
{
	return m_startupInfo;
}

void WinProcess::Close()
{
	CloseHandle(m_pinfo.hProcess);
	CloseHandle(m_pinfo.hThread);
	m_pinfo.hProcess=0;
	m_pinfo.hThread=0;
}
bool WinProcess::Run()
{
	core::string args;
	args=m_application+mT(" ");
	for (int i=0;i<m_arguments.size();++i)
	{
		args+=m_arguments[i]+mT(" ");
	}

	STARTUPINFO startIfo;
	SECURITY_ATTRIBUTES secAttrs;

	memset(&startIfo,0,sizeof(startIfo));
/*
	startIfo.dwX=m_startupInfo.pos.x;
	startIfo.dwY=m_startupInfo.pos.y;
	startIfo.dwXSize=m_startupInfo.size.x;
	startIfo.dwYSize=m_startupInfo.size.y;*/
	startIfo.wShowWindow=m_startupInfo.showWindow;
	
	secAttrs.nLength=sizeof(SECURITY_ATTRIBUTES);
	secAttrs.bInheritHandle=true;
	secAttrs.lpSecurityDescriptor=0;

	if(m_startupInfo.outputStream)
	{
		// Create a pipe for the child stdOut
		if(!CreatePipe(&m_child_stdOut_Read,&m_child_stdOut_Write,&secAttrs,0))
		{
			gOSLoggerSystem.log(mT("CreatePipe:Stdout Read"),ELL_WARNING,EVL_Normal);
			return false;
		}
		//ensure the read handle for the pipe is not inherited
		if(!SetHandleInformation(m_child_stdOut_Read,HANDLE_FLAG_INHERIT,0))
		{
			gOSLoggerSystem.log(mT("SetHandleInformation:Stdout"),ELL_WARNING,EVL_Normal);
			return false;
		}
	}else
	{
		m_child_stdOut_Read=m_child_stdOut_Write=0;
	}
	if(m_startupInfo.inputStream)
	{
		// Create a pipe for the child stdIn
		if(!CreatePipe(&m_child_stdIn_Read,&m_child_stdIn_Write,&secAttrs,0))
		{
			gOSLoggerSystem.log(mT("CreatePipe:Stdin Read"),ELL_WARNING,EVL_Normal);
			return false;
		}
		//ensure the read handle for the pipe is not inherited
		if(!SetHandleInformation(m_child_stdIn_Write,HANDLE_FLAG_INHERIT,0))
		{
			gOSLoggerSystem.log(mT("SetHandleInformation:Stdin"),ELL_WARNING,EVL_Normal);
			return false;
		}
	}else
	{
		m_child_stdOut_Read=m_child_stdOut_Write=0;
	}/*
	startIfo.cb=sizeof(startIfo);
	startIfo.hStdInput=m_child_stdIn_Read;
	startIfo.hStdOutput=m_child_stdOut_Write;
	startIfo.hStdError=m_child_stdOut_Write;
	startIfo.dwFlags|=STARTF_USESTDHANDLES;*/
	/*
	char argsStr[256];
	strncpy(argsStr,args.c_str(),args.length());
	argsStr[args.length()]=0;
*/
	bool res= CreateProcess(m_application.c_str(),&args[0],0,0,false,0,0,0,
		&startIfo,&m_pinfo);

	if(!res)
	{
		gOSLoggerSystem.log(mT("CreateProcess:")+m_application,ELL_WARNING,EVL_Normal);
	}
	return res;
}

void WinProcess::OnWriteData(const void*data,size_t count)
{
	DWORD written=0;
	WriteFile(m_child_stdIn_Write,data,count,&written,0);
}

}
}
