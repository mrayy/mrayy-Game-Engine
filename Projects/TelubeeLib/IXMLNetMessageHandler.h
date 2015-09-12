

#ifndef __IXMLNETMESSAGEHANDLER__
#define __IXMLNETMESSAGEHANDLER__

#include "IUDPClient.h"

namespace mray
{
namespace TBee
{
	
class IXMLNetMessageHandler
{
protected:

	virtual void _HandleData(network::NetAddress* addr, const core::string& name, const core::string& value) = 0;
	void ProcessPacket(network::NetAddress* addr, const char* buffer);

	network::IUDPClient* m_client;

	OS::IMutex* m_dataMutex;
	OS::IThread* m_thread;
public:
	IXMLNetMessageHandler();
	virtual ~IXMLNetMessageHandler();

	virtual void StartHandler(int port);
	virtual void StopHandler();

	int _Process();
};

}
}


#endif

