
#ifndef __SERVICELOADER__
#define __SERVICELOADER__

#include "shmem.h"
#include "ModuleSharedMemory.h"
#include "TBeeServiceContext.h"
#include "IServiceModule.h"
#include "IUDPClient.h"

namespace mray
{
	
class ServiceLoader
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

	std::string m_moduleName;
	bool m_inited;

	TBee::TBeeServiceContext m_context;

	void _MonitorEvents();
	void _UpdateServiceStatus();
	void _destroy();

	void _loadSettings();

	void _sendConnectMessage();
	void _sendDisconnectMessage();
	void _sendPongMessage();

	void _RenderInfo();

	//////////////////////////////////////////////////////////////////////////
	void OnUserConnected(const TBee::UserConnectionData& data);
	void OnUserDisconnected(const network::NetAddress& address);

public:
	ServiceLoader();
	virtual ~ServiceLoader();

	bool Init(int argc, _TCHAR* argv[]);
	void Run();
	bool _ProcessPacket();


};

}


#endif
