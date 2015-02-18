
/********************************************************************
	created:	2009/04/07
	created:	7:4:2009   0:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\ConnectedPeer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	ConnectedPeer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ConnectedPeer___
#define ___ConnectedPeer___

#include "compileconfig.h"
#include "NetAddress.h"
#include <mString.h>
#include "SendPacketQueue.h"
#include "GCPtr.h"


namespace mray{
namespace network{

	class ISocket;

class MRAY_DLL ConnectedPeer
{
public:
	ConnectedPeer(ISocket*s);
	virtual~ConnectedPeer();
	bool removed;

	ISocket*m_socket;
	GCPtr<SendPacketQueue> m_sendQueue;

	NetID id;

	long localStartTime;
	long peerStartTime;
	core::string clientName;
	NetAddress address;
	NetAddress relAddress;		//it's reliable address (TCP address)
	EPeerConnectState state;
	long lastMsgTime;
	long lastPingTime;
	long lastPongTime;
	long pingTime;				//time took to ping
	long nextPingTime;			//time to send ping in ms 
	long connectionTime;

};
MakeSharedPtrType(ConnectedPeer);


}
}

#endif //___ConnectedPeer___
