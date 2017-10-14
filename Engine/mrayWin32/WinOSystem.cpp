#include "stdafx.h"

#include "WinOSystem.h"
#include "WinDirOS.h"
#include "WinMutex.h"
#include "WinThreadManager.h"
#include "WinTimer.h"
#include "WinRegistry.h"
#include "WinFileSystem.h"
#include "WinDllManager.h"
#include "WinOSClipboard.h"

#include "ILogManager.h"
#include "WinProcess.h"
#include "Win32CallbackProc.h"
#include "OptionContainer.h"
#include "WinFileMonitor.h"
#include "Win32SerialPort.h"

namespace mray{
namespace OS{


WinOSystem::WinOSystem(){
	m_registryManager=new OS::WinRegistry();
	m_fileSystem=new OS::WinFileSystem();
	m_dllManager=new OS::WinDllManager();
	m_clipboardManager=new OS::WinOSClipboard();
	m_threadManager=new OS::WinThreadManager();
	m_serialportService = new Win32SerialPortService;

	new Win32MessageProc();


	DWORD_PTR ProcessAffinityMask;
	DWORD_PTR SystemAffinityMask;
	GetProcessAffinityMask(GetCurrentProcess(),&ProcessAffinityMask, &SystemAffinityMask);

	SYSTEM_INFO si;
	GetSystemInfo( &si );

	m_processorsAffineMask.clear() ;//resize(si.dwNumberOfProcessors);

	// make sure we can use all cpus
	if(SystemAffinityMask!=ProcessAffinityMask)
	{
		gLogManager.log(mT("Engine's process has been restricted,so it can't use all processors in the system."),ELL_WARNING);
	}

	for (int i=0;ProcessAffinityMask>0;i++,ProcessAffinityMask >>= 1)
	{
		if ( ProcessAffinityMask & 1 )
		{
			m_processorsAffineMask.push_back( i );
		}
	}

	FATAL_ERROR(m_processorsAffineMask.size()!=si.dwNumberOfProcessors,mT("Error while retrieving processors affine mask."));

}

WinOSystem::~WinOSystem(){
	m_dllManager=0;
	m_threadManager=0;
	m_clipboardManager=0;
	m_fileSystem=0;
	m_registryManager=0;
	
	delete Win32MessageProc::getInstancePtr();
}

ITimer* WinOSystem::createTimer()
{
	return new WinTimer();
}


IDirOS* WinOSystem::createDirSystem(){
	return new WinDirOS();
}

bool WinOSystem::getMemoryState(SMemoryState& state){
	MEMORYSTATUSEX ms;
	ms.dwLength=sizeof(ms);
	if(!GlobalMemoryStatusEx(&ms))
		return false;
	state.TotalPhysicalMemory=ms.ullTotalPhys>>10;
	state.AvaliablePhysicalMemory=ms.ullAvailPhys>>10;

	state.TotalVirtualMemory=ms.ullTotalVirtual>>10;
	state.AvaliableVirtualMemory=ms.ullAvailVirtual>>10;

	state.TotalPageFile=ms.ullTotalPageFile>>10;
	state.AvaliablePageFile=ms.ullAvailPageFile>>10;
	return true;

}

bool WinOSystem::getPowerState(SPowerStatus& state){
	SYSTEM_POWER_STATUS st={0};

	if(!::GetSystemPowerStatus(&st))
		return false;

	state.BattaryLifePercent=st.BatteryLifePercent;
	state.BattaryLifeTime=st.BatteryLifeTime;
	state.BattaryFullLifeTime=st.BatteryFullLifeTime;


	state.ACStatus=ACLS_Unkown;
	if(st.ACLineStatus == AC_LINE_OFFLINE)
		state.ACStatus =ACLS_Offline;
	else if(st.ACLineStatus == AC_LINE_ONLINE)
		state.ACStatus =ACLS_Online;
	else if(st.ACLineStatus == AC_LINE_BACKUP_POWER)
		state.ACStatus =ACLS_BackupPower;

	state.BattaryFlags=0;
	if(st.BatteryFlag & BATTERY_FLAG_HIGH)
		state.BattaryFlags |=BF_High;
	if(st.BatteryFlag & BATTERY_FLAG_LOW)
		state.BattaryFlags |=BF_Low;
	if(st.BatteryFlag & BATTERY_FLAG_CRITICAL)
		state.BattaryFlags |=BF_Critical;
	if(st.BatteryFlag & BATTERY_FLAG_CHARGING)
		state.BattaryFlags |=BF_Charging;
	if(st.BatteryFlag & BATTERY_FLAG_NO_BATTERY)
		state.BattaryFlags |=BF_NoBattary;
	if(st.BatteryFlag == BATTERY_FLAG_UNKNOWN)
		state.BattaryFlags =BF_Unkown;

	return true;

}

int WinOSystem::getProcessorsCount(){
	return m_processorsAffineMask.size();
}
void WinOSystem::AffinitizeThreadToProcessor(uint pid){
	if(pid>=m_processorsAffineMask.size())
		return;
	SetThreadAffinityMask(GetCurrentThread(),m_processorsAffineMask[pid]);
	//SetThreadIdealProcessor(GetCurrentThread(),m_processorsAffineMask[pid]);
}
ISystemProcess* WinOSystem::CreateSystemProcess()
{
	return new WinProcess();
}

ulong WinOSystem::HandleWindowEvent(const OptionContainer& params)
{
	const SOptionElement* p;
	HWND hWnd=0;
	UINT msg=0;
	WPARAM wParam=0;
	LPARAM lParam=0;

	p=params.GetOptionByName(mT("hWnd"));
	if(!p)return 0;
	hWnd=(HWND)atoll(p->value.c_str());

	p=params.GetOptionByName(mT("Msg"));
	if(!p)return 0;
	msg=core::StringConverter::toULong(p->value);

	p=params.GetOptionByName(mT("wParam"));
	if(!p)return 0;
	wParam= atoll(p->value.c_str());

	p=params.GetOptionByName(mT("lParam"));
	if(!p)return 0;
	lParam=atoll(p->value.c_str());

	return Win32MessageProc::getInstance().WndProc(hWnd,msg,wParam,lParam);
}

IFileMonitor* WinOSystem::CreateFileMonitor()
{
	return new WinFileMonitor();
}

}
}
