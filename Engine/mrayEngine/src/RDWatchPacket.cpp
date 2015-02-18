#include "stdafx.h"

#include "RDWatchPacket.h"
#include "StreamReader.h"
#include "StreamWriter.h"


namespace mray{
namespace debug{

RDWatchPacket::RDWatchPacket(){
}
RDWatchPacket::~RDWatchPacket(){
}

void RDWatchPacket::readPacket(network::CMsgBuffer*buffer){
	network::NetPacket::readPacket(buffer);
	OS::StreamReader reader((OS::IStream*) buffer);

	reader.read(&m_id,sizeof(m_id));
	m_name=reader.readString();
	m_val=reader.readString();
	m_group=reader.readString();

}
void RDWatchPacket::writeData(network::CMsgBuffer*buffer){
	network::NetPacket::writeData(buffer);
	OS::StreamWriter writer((OS::IStream*) buffer);

	writer.write(&m_id,sizeof(m_id));
	writer.writeString(m_name);
	writer.writeString(m_val);
	writer.writeString(m_group);
}


}
}