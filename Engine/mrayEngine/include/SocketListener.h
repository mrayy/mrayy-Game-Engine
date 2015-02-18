
/********************************************************************
	created:	2009/04/07
	created:	7:4:2009   0:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\SocketListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	SocketListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SocketListener___
#define ___SocketListener___

#include "NetDataTypes.h"
#include "GCPtr.h"

namespace mray{
namespace network{


class SocketListener
{
private:
protected:
public:
	SocketListener(){}
	virtual~SocketListener(){}


	virtual void onPeerConnected(const NetAddress&addr)=0;
	virtual void onPeerDisconnect(const NetAddress&addr)=0;
	virtual void onPacketArrived(SRecivedMessage*msg)=0;

	virtual void onUnkownPacketArrived(int cmd,CMsgBuffer*buffer,const NetAddress& address,ESendMode mode)=0;
};

MakeSharedPtrType(SocketListener);

}
}


#endif //___SocketListener___
