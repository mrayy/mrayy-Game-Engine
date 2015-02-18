
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RDWRemovePacket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RDWRemovePacket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RDWRemovePacket___
#define ___RDWRemovePacket___


#include "NetPacket.h"
#include "NetDataTypes.h"
#include "RDPacketsID.h"

namespace mray{
namespace debug{

class MRAY_DLL RDWRemovePacket:public network::NetPacket
{
private:
protected:
public:
	uint m_id;

	RDWRemovePacket(){}
	virtual~RDWRemovePacket(){}

	virtual network::commands::NTCMD getCommand(){
		return ERDP_RemoveWatch;
	}

	virtual void readPacket(network::CMsgBuffer*buffer){
		network::NetPacket::readPacket(buffer);
		OS::StreamReader reader((OS::IStream*) buffer);
		reader.read(&m_id,sizeof(m_id));
	}
	virtual void writeData(network::CMsgBuffer*buffer){
		network::NetPacket::writeData(buffer);
		OS::StreamWriter writer((OS::IStream*) buffer);
		writer.write(&m_id,sizeof(m_id));
	}

	virtual network::ESendMode getSendMode(){
		return network::ESM_RELIABLE;
	}

};

}
}


#endif //___RDWRemovePacket___
