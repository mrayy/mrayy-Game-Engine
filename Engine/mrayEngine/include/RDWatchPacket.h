
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   17:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RDWatchPacket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RDWatchPacket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Remote Debug Watch Packet
*********************************************************************/

#ifndef ___RDWatchPacket___
#define ___RDWatchPacket___

#include "NetPacket.h"
#include "NetDataTypes.h"
#include "RDPacketsID.h"

namespace mray{
namespace debug{

class MRAY_DLL RDWatchPacket:public network::NetPacket
{
private:
protected:
public:
	uint m_id;
	core::string m_name;
	core::string m_val;
	core::string m_group;

	RDWatchPacket();
	virtual~RDWatchPacket();

	virtual network::commands::NTCMD getCommand(){
		return ERDP_NewWatch;
	}

	virtual void readPacket(network::CMsgBuffer*buffer);
	virtual void writeData(network::CMsgBuffer*buffer);

	virtual network::ESendMode getSendMode(){
		return network::ESM_RELIABLE;
	}

};

}
}

#endif //___RDWatchPacket___
