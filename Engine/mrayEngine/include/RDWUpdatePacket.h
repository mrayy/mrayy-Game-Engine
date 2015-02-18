
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RDWUpdatePacket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RDWUpdatePacket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Remote Debug Watch Update Packet
*********************************************************************/

#ifndef ___RDWUpdatePacket___
#define ___RDWUpdatePacket___

#include "NetPacket.h"
#include "NetDataTypes.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include "RDPacketsID.h"

namespace mray{
namespace debug{

class MRAY_DLL RDWUpdatePacket:public network::NetPacket
{
private:
protected:
public:
	uint m_id;
	core::string m_val;


	RDWUpdatePacket(){}
	virtual~RDWUpdatePacket(){}

	virtual network::commands::NTCMD getCommand(){
		return ERDP_UpdateWatch;
	}

	virtual void readPacket(network::CMsgBuffer*buffer){
		network::NetPacket::readPacket(buffer);
		OS::StreamReader reader((OS::IStream*) buffer);
		reader.read(&m_id,sizeof(m_id));
		m_val=reader.readString();

	}
	virtual void writeData(network::CMsgBuffer*buffer){
		network::NetPacket::writeData(buffer);
		OS::StreamWriter writer((OS::IStream*) buffer);
		writer.write(&m_id,sizeof(m_id));
		writer.writeString(m_val);
	}


	virtual network::ESendMode getSendMode(){
		return network::ESM_RELIABLE;
	}

};

}
}

#endif //___RDWUpdatePacket___
