


#include "stdafx.h"
#include "UDPCommunicationLayer.h"
#include "INetwork.h"
#include "IUDPClient.h"

#include "IThreadManager.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "CMemoryStream.h"
#include "CommunicationData.h"
#include "StreamReader.h"

namespace mray
{
namespace VT
{

	class UDPClientReceivingThread:public OS::IThreadFunction
	{
	protected:
		network::IUDPClient* m_client;
		UDPCommunicationLayer* m_owner;
	public:

		UDPClientReceivingThread(network::IUDPClient*c,UDPCommunicationLayer*o)
		{
			m_client=c;
			m_owner=o;
		}
		virtual void execute(OS::IThread*caller,void*arg)
		{
			byte buffer[1024];
			while(caller->isActive())
			{
				uint len=1024;
				network::NetAddress addr;
				network::UDPClientError err= m_client->RecvFrom((char*)buffer,&len,&addr,0);
				if(err==network::UDP_SOCKET_ERROR_NONE)
				{
					OS::CMemoryStream stream("",buffer,len,false);
					m_owner->_ReceiveData(&stream);
				}else
				{
					printf("%s\n",network::IUDPClient::TranslateErrorMessage(err).c_str());
				}
			}
		}
	};

UDPCommunicationLayer::UDPCommunicationLayer(ushort port)
{
	m_port=port;
	m_msgListener=0;
	m_dataMutex=OS::IThreadManager::getInstance().createMutex();
	m_client=network::INetwork::getInstance().createUDPClient();

}
UDPCommunicationLayer::~UDPCommunicationLayer()
{
	Close();
	delete m_client;
}



bool UDPCommunicationLayer::Open(ushort port)
{
	network::UDPClientError err= m_client->Open(port);
	if(err==network::UDP_SOCKET_ERROR_NONE)
	{
		m_thread=OS::IThreadManager::getInstance().createThread(new UDPClientReceivingThread(m_client,this));
		m_thread->start(0);
		return true;
	}

	gLogManager.log(mT("UDPCommunicationLayer::Open()- Failed to open port:")+core::StringConverter::toString(port)+mT(" with error: ")+network::IUDPClient::TranslateErrorMessage(err),ELL_WARNING);

	return false;
}

void UDPCommunicationLayer::Update(float dt)
{
	m_dataMutex->lock();
	FIRE_LISTENR_METHOD(OnDataArrived,(this,&m_data));
	m_dataMutex->unlock();
}
void UDPCommunicationLayer::Start()
{
	Open(m_port);
}
void UDPCommunicationLayer::Close()
{
	m_client->Close();

	if (m_thread)
	{
		m_thread->terminate();
		OS::IThreadManager::getInstance().killThread(m_thread);
	}
}
void UDPCommunicationLayer::_ReceiveData(OS::IStream* stream)
{
	OS::StreamReader rdr(stream);
	int messageType=rdr.binReadInt();
	if(messageType==ESchemeMessage)
	{
		m_scheme.clear();
		int cnt=rdr.binReadInt();
		for(int i=0;i<cnt;++i)
		{
			m_scheme.push_back(rdr.binReadStringC());
		}
		FIRE_LISTENR_METHOD(OnSchemeChanged,(this,m_scheme))
	}else if(messageType==EDataMessage)
	{
		m_dataMutex->lock();
		CommunicationDataStreamer::ReadData(m_data,stream);
		m_dataMutex->unlock();
		//FIRE_LISTENR_METHOD(OnDataArrived,(this,&m_data));

	}else if(messageType==ECustomMessage)
	{
		if(m_msgListener!=0)
		{
			m_msgListener->OnCustomMessage(stream);
		}
	}
}

}
}
