
#ifndef __SERVICELOADER__
#define __SERVICELOADER__

#include "shmem.h"
#include "ModuleSharedMemory.h"
#include "TBeeServiceContext.h"
#include "IServiceModule.h"
#include "IUDPClient.h"
#include "XMLTree.h"

namespace mray
{
	
class ServiceLoader:public TBee::IServiceLoader
{
protected:
	TBee::ModuleSharedMemory *m_memory;
	TBee::ModuleSharedMemory m_oldMemory;

	shmem m_sharedMemory;
	OS::IDynamicLibraryPtr m_moduleLib;
	TBee::IServiceModule* m_serviceModule;

	//network::IUDPClient* m_serviceClient;
	OS::IMutex* m_dataMutex;
	OS::IThread* m_thread;
	TBee::ServiceRenderContext* m_renderContext;
	TBee::TBeeServiceContext m_context;
	xml::XMLTree m_valueTree;
	xml::XMLElement* m_valueRootElement;

	std::string m_moduleName;
	bool m_inited;

	struct ProcessLock
	{
		std::string process;
		int count;
	};
	std::vector<ProcessLock> m_processLocks;


	void _MonitorEvents();
	void _UpdateServiceStatus();
	void _destroy();

	void _loadSettings();

	void _sendConnectMessage();
	void _sendDisconnectMessage();
	void _sendPongMessage();

	void _RenderInfo();

	bool _isProcessLocked();
	void _lockProcess(const std::string& requester);//request to lock this process until the requester finish its cpu usage
	void _unlockProcess(const std::string& requester);

	//////////////////////////////////////////////////////////////////////////
	void OnUserConnected(const TBee::UserConnectionData& data);
	void OnUserDisconnected(const network::NetAddress& address);

public:
	ServiceLoader();
	virtual ~ServiceLoader();

	bool Init(int argc, _TCHAR* argv[]);
	void Run();
	bool _ProcessPacket();


	// when a service needs to utilize the cpu for a certain of time, then it can request to lock/unlock others
	void RequestLock();
	void RequestUnlock();

};

}


#endif
