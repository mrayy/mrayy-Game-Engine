
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   16:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IOSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IOSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IOSystem___
#define ___IOSystem___

#include "ISingleton.h"
#include "common.h"
//#include "OptionContainer.h"

namespace mray{
	class  OptionContainer;
namespace OS{

	class  IOSClipboard;

	class  IDirOS;
	class  ISystemProcess;
	class  ITimer;

	struct SMemoryState
	{
	public:
		int TotalPhysicalMemory;
		int AvaliablePhysicalMemory;
		int TotalVirtualMemory;
		int AvaliableVirtualMemory;
		int TotalPageFile;
		int AvaliablePageFile;
	};

	enum EACLinesStatus
	{
		ACLS_Offline,
		ACLS_Online,
		ACLS_BackupPower,
		ACLS_Unkown
	};

	enum EBattaryFlag
	{
		BF_High=BIT(0),
		BF_Low=BIT(1),
		BF_Critical=BIT(2),
		BF_Charging=BIT(3),
		BF_NoBattary=BIT(4),
		BF_Unkown=BIT(5)
	};

	struct SPowerStatus
	{
	public:
		EACLinesStatus ACStatus;
		uchar BattaryFlags;
		uchar BattaryLifePercent;
		uint BattaryLifeTime;
		uint BattaryFullLifeTime;
	};

	class  IFileMonitor;
	class  ISerialPortService;
	class MRAY_CORE_DLL IOSystem :public ISingleton<IOSystem>
{
public:
	virtual~IOSystem(){}
	virtual IDirOS* createDirSystem()=0;

	virtual ITimer* createTimer() = 0;
	virtual IOSClipboard* getClipboard() = 0;

	virtual bool getMemoryState(SMemoryState& state)=0;
	virtual bool getPowerState(SPowerStatus& state)=0;
	virtual int getProcessorsCount()=0;
	virtual void AffinitizeThreadToProcessor(uint pid)=0;

	virtual ISystemProcess* CreateSystemProcess()=0;
	
	virtual ulong HandleWindowEvent(const OptionContainer& params)=0;

	virtual IFileMonitor* CreateFileMonitor() = 0;
	virtual ISerialPortService* GetSerialPortService()=0;
};

#define gOSystem  mray::OS::IOSystem::getInstance()

}
}


#endif //___IOSystem___
