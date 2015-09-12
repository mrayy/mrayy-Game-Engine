
#ifndef __SERVICEHOSTMANAGER__
#define __SERVICEHOSTMANAGER__

#include "shmem.h"
#include "ModuleSharedMemory.h"
#include "TBeeServiceContext.h"
#include "RobotCommunicator.h"
#include "IServiceModule.h"
#include "IUDPClient.h"
#include "TBRobotInfo.h"

#include <windows.h>

namespace mray
{
	
class ServiceHostManager :public TBee::IRobotCommunicatorListener
{
protected:
	TBee::ModuleSharedMemory *m_memory;
	shmem m_sharedMemory;

	bool m_autoRestartService;// restart when the service die
	bool m_inited;

	network::IUDPClient* m_commLink;
	OS::IMutex* m_dataMutex;
	OS::IThread* m_commThread;
	OS::IThread* m_serviceThread;

	TBee::TBRobotInfo m_info;

	TBee::RobotCommunicator* m_robotCommunicator;

	struct ServiceInfo
	{
		ServiceInfo()
		{
			processHandle = 0;
			threadHandle = 0;
			lastTime = 0;
			pingSent = false;
		}
		core::string name;
		network::NetAddress address;
		TBee::EServiceStatus status;

		HANDLE processHandle;
		HANDLE threadHandle;

		bool pingSent;
		ulong lastTime;//last time this service was alive
	};

	typedef std::vector<ServiceInfo> ServiceList;
	ServiceList m_serviceList;

	int GetServiceByName(const core::string &name);
	int GetServiceByAddress(const network::NetAddress* addr);

	void _destroy();

	void _ServiceAdded();
	bool _ProcessPacket();
	bool _ProcessServices();

public:
	ServiceHostManager();
	virtual ~ServiceHostManager();

	bool Init(int argc, _TCHAR* argv[]);
	void Run();
	bool RunLocalService(const core::string& name);


	virtual void OnUserConnected(TBee::RobotCommunicator* sender, const TBee::UserConnectionData& data);
	void OnUserDisconnected(TBee::RobotCommunicator* sender, const network::NetAddress& address);
	void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value);
};

}


#endif

