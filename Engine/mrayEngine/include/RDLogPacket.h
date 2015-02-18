
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   23:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RDLogPacket.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RDLogPacket
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RDLogPacket___
#define ___RDLogPacket___

#include "NetPacket.h"
#include "NetDataTypes.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include "RDPacketsID.h"

namespace mray{
namespace debug{

class MRAY_DLL RDLogPacket:public network::NetPacket
{
private:
protected:
public:

	core::string m_msg;


	RDLogPacket(){}
	virtual~RDLogPacket(){}

	virtual network::commands::NTCMD getCommand(){
		return ERDP_LogMessage;
	}

	virtual void readPacket(network::CMsgBuffer*buffer){
		network::NetPacket::readPacket(buffer);
		OS::StreamReader reader((OS::IStream*) buffer);
		m_msg=reader.readLine();

	}
	virtual void writeData(network::CMsgBuffer*buffer){
		network::NetPacket::writeData(buffer);
		OS::StreamWriter writer((OS::IStream*) buffer);
		writer.writeLine(m_msg);
	}


	virtual network::ESendMode getSendMode(){
		return network::ESM_RELIABLE;
	}
};

}
}


#endif //___RDLogPacket___
