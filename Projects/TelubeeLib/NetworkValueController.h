

#ifndef __NETWORKVALUECONTROLLER__
#define __NETWORKVALUECONTROLLER__

#include "IUDPClient.h"
#include "ValueGroup.h"

namespace mray
{
namespace TBee
{
	
class NetworkValueController
{
protected:
	network::IUDPClient* m_client;
	ValueGroup m_values;
	OS::IMutex* m_dataMutex;
	OS::IThread* m_thread;

	network::NetAddress m_remote;

	bool m_isReceiver;
public:
	NetworkValueController();
	virtual ~NetworkValueController();

	ValueGroup* GetValues();
	
	void StartReceiver(int port);
	void StartSender(const network::NetAddress& target);

	void Stop();

	//if not receiver, then you can send data
	void SendData();

	bool _Process();
};

}
}


#endif
