

/********************************************************************
	created:	2013/05/14
	created:	14:5:2013   20:28
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack\Communicator.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\TelesarOptitrack
	file base:	Communicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __Communicator__
#define __Communicator__




#include "IUDPClient.h"
#include "CommunicationData.h"

namespace mray
{

class Communicator
{
protected:
	network::IUDPClient* m_udpClient;
	OS::CMemoryStream *m_stream;
	core::string m_ip;
	int m_port;
	std::vector<core::string> m_scheme;
	VT::CommunicationData m_sendData;
private:
	static Communicator* m_instance;
public:
	Communicator();
	virtual~Communicator();

	static Communicator* Instance(){return m_instance;}

	void Connec(const core::string& ip, int port,const core::string& targetName);
	void Close();

	void AddSchemeItem(const core::string& name);
	void SendScheme();

	void AddSendValue(const core::string& name,float* v,int c);
	void SendValues();
	void SendCustomCommand(int c);
};

}

#endif
