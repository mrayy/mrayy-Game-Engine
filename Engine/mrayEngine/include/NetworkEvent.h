

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\NetworkEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	NetworkEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NetworkEvent___
#define ___NetworkEvent___

#include "Event.h"

namespace mray{

	enum ENetworkEventType{
		ENET_Recived,
		ENET_Connected,
		ENET_Disconnect
	};

class NetworkEvent:public Event
{
public:
	ENetworkEventType m_eventType;
public:
	NetworkEvent():Event(ET_NetworkEvent)
	{

	}
};

}


#endif //___NetworkEvent___
