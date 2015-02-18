

#include "stdafx.h"
#include "Communicator.h"
#include "CommunicationData.h"
#include "CMemoryStream.h"

#include "INetwork.h"
#include "StreamWriter.h"
#include "UDPCommunicationLayer.h"


namespace mray
{

	Communicator* Communicator::m_instance=new Communicator();


Communicator::Communicator()
{
	m_udpClient=0;

	m_stream=new OS::CMemoryStream("",new byte[1024],1024,true,OS::BIN_WRITE);

}
Communicator::~Communicator()
{
	Close();
}

void Communicator::Connec(const core::string& ip, int port,const core::string& targetName)
{
	Close();
	m_ip=ip;
	m_port=port;
	m_udpClient=network::INetwork::getInstance().createUDPClient();
	m_udpClient->Open();
	if(!m_udpClient->Connect(network::NetAddress(ip,port)))//"192.168.10.111",1234));
		printf("Failed to connect to %s:%d\n",ip.c_str(),port);


	m_sendData.SetTargetName(targetName);//"TelesarV");

}
void Communicator::Close()
{
	if(!m_udpClient)
		return;
	m_udpClient->Close();
	delete m_udpClient;
	m_udpClient=0;
}

void Communicator::SendScheme()
{
	if(!m_udpClient)
		return;

	m_stream->reopen(OS::BIN_WRITE);
	OS::StreamWriter wrtr(m_stream);

	wrtr.binWriteInt(VT::ESchemeMessage);
	wrtr.binWriteInt(m_scheme.size());
	for (int i=0;i<m_scheme.size();++i)
	{
		wrtr.binWriteString(m_scheme[i]);
	}

	network::NetAddress addr(network::EAT_Localhost,1234);
	network::UDPClientError e= m_udpClient->SendTo(&addr,(char*)m_stream->getData(),m_stream->getPos());
	m_scheme.clear();
}
void Communicator::AddSchemeItem(const core::string& name)
{
	m_scheme.push_back(name);
}


void Communicator::AddSendValue(const core::string& name,float* v,int c)
{
	std::vector<VT::ControlInputValues> vals;
	vals.resize(c);
	for(int i=0;i<c;++i)
		vals[i].KinValue=vals[i].RealtimeValue=v[i];

	m_sendData.AddData(name,&vals[0],c);
}
void Communicator::SendValues()
{
	if(m_udpClient)
	{
		OS::StreamWriter wrtr(m_stream);
		wrtr.binWriteInt(VT::EDataMessage);
		VT::CommunicationDataStreamer::WriteData(m_sendData,m_stream);
		m_sendData.ClearData();
		network::NetAddress addr(network::EAT_Localhost,1234);
		//network::NetAddress addr(m_ip,m_port);
		network::UDPClientError e= m_udpClient->SendTo(&addr,(char*)m_stream->getData(),m_stream->getPos());
		if(e!=network::UDP_SOCKET_ERROR_NONE)
		{
			printf("UDP Error: %s\n",network::IUDPClient::TranslateErrorMessage(e).c_str());
		}

	}
	m_stream->reopen(OS::BIN_WRITE);

}
void Communicator::SendCustomCommand(int c)
{
	if(m_udpClient)
	{
		OS::StreamWriter wrtr(m_stream);
		wrtr.binWriteInt(VT::ECustomMessage);
		wrtr.binWriteInt(c);
		network::NetAddress addr(network::EAT_Localhost,1234);
		//network::NetAddress addr(m_ip,m_port);
		network::UDPClientError e= m_udpClient->SendTo(&addr,(char*)m_stream->getData(),m_stream->getPos());
		if(e!=network::UDP_SOCKET_ERROR_NONE)
		{
			printf("%s\n",network::IUDPClient::TranslateErrorMessage(e).c_str());
		}

	}
	m_stream->reopen(OS::BIN_WRITE);
}

}