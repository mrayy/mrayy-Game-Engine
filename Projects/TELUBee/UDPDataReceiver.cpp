
#include "stdafx.h"
#include "UDPDataReceiver.h"
#include "IUDPClient.h"
#include "INetwork.h"
#include "IThreadManager.h"
#include "NetworkMessage.h"
#include "DynamicMemoryStream.h"
#include "StreamReader.h"

namespace mray
{
namespace TBee
{

class UDPDataReceiverImpl;

class UDPDataReceiverThread:public OS::IThreadFunction
{
	UDPDataReceiverImpl* m_owner;
public:
	UDPDataReceiverThread(UDPDataReceiverImpl*o)
	{
		m_owner=o;
	}
	virtual void execute(OS::IThread*caller,void*arg);
};

class UDPDataReceiverImpl:public network::UDPClientOwner
{
protected:
	UDPDataReceiver* m_owner;
	network::IUDPClient* m_client;
	OS::IThread* m_thread;
	bool m_started;
	OS::DynamicMemoryStream m_stream;

protected:
	bool UDPClientRecvHandler(network::IUDPClient* theSocket)
	{
		return true;
	}
public:

	UDPDataReceiverImpl(UDPDataReceiver* o):m_owner(o)
	{
		m_started=false;
		m_client=network::INetwork::getInstance().createUDPClient();
		m_thread=0;
		//m_client->Init(this,&UDPDataReceiverImpl::UDPClientRecvHandler,0,0);
	}
	virtual~UDPDataReceiverImpl()
	{
		OS::IThreadManager::getInstance().killThread(m_thread);
		m_client->Close();
		delete m_client;
	}

	bool Connect(const core::string& ip,int port)
	{
		if(m_client->IsConnected())
			m_client->Disconnect();
		m_owner->m_closed = false;
		m_client->Open();
		if(!m_client->Connect(network::NetAddress(ip,port)))
			return false;
		char dgram[1];
		dgram[0]=(char)UDPMessages::Connect;
		m_client->SendTo(0,dgram, 1);
		return true;
	}
	bool IsConnected()
	{
		return m_client->IsConnected();
	}
	void Start()
	{
		if(m_started)
			return;
		m_started=true;
		m_thread=OS::IThreadManager::getInstance().createThread(new UDPDataReceiverThread(this));
		m_thread->start(0);
		m_stream.seek(0,OS::ESeek_Set);
		m_stream.Resize(0);
	}
	void Close()
	{
		char dgram[1];
		dgram[0]=(char)UDPMessages::Disconnect;
		m_client->SendTo(0,dgram, 1);
		m_owner->m_closed=true;
		m_started=false;
		m_client->Close();
		if(m_thread)
			m_thread->terminate();
		OS::IThreadManager::getInstance().killThread(m_thread);
		delete m_thread;
	}


	void ProcessStream()
	{
//		try
		{
			OS::StreamReader r(&m_stream);
			NetworkMessage* msg = NetworkMessage::DeserializePacket(&r);
			if (msg == 0)
				return;
			switch (msg->msgType)
			{
			case NetworkMessageType::HeadChange:
				{
				}
				break;
			case NetworkMessageType::SpeedChange:
				{
				}
				break;
			case NetworkMessageType::VideoImage:
				{
					VideoImageMessage* m = (VideoImageMessage*)msg;
					m_owner->__FIRE_OnVideoImage(m_owner,m->m_bmp,m->m_camSource==ENCamera_Left? ECamera_Left:ECamera_Right);
					delete m;
				} break;
			case NetworkMessageType::Stop:
				{
// 					var m = msg as StopMessage;
// 					_NotifyStop();
				} break;
			case NetworkMessageType::UdpPort:
				{
// 					var m = msg as UdpPortMessage;
// 					_NotifyUdpPort(m.Port);
				} break;
			}
		}
// 		catch (std::exception &e)
// 		{
// 			gLogManager.log("ProcessStream() : " + e.what()., Logger.LogStatus.Failure);
// 		}
	}

	bool ReceiveData()
	{
		m_stream.seek(0, OS::ESeek_Set); ;
		//m_stream.se(0);
		char buff[4];
		uint buffLen=4;
		network::UDPClientError err;
		err = m_client->RecvFrom(buff, &buffLen, 0, 0);
		if(err!=network::UDPClientError::UDP_SOCKET_ERROR_NONE || buffLen==0)
			return false;
		uint length=core::BitConverter::ToUInt32((byte*)buff);
		m_stream.Resize(length);
		buffLen=length;
		int recieved=0;
		char* ptr=m_stream.GetDataPtr();
		do 
		{
			buffLen=length- recieved;
			err = m_client->RecvFrom(ptr, &buffLen, 0, 0);
			if(err!=network::UDPClientError::UDP_SOCKET_ERROR_NONE)
				return false;
			ptr+=buffLen;
			recieved+=buffLen;
		} while (recieved<length);
		return true;
	}
	void HandleTick()
	{

//		try
		{
			if (!m_started)
				return;
			if(ReceiveData())
				ProcessStream();
		}
// 		catch(ThreadAbortException e)
// 		{
// 			break;
// 		}
// 		catch (Exception e)
// 		{
// 			Logger.Instance.Log("UDPClient Thread: " + e.ToString(),Logger.LogStatus.Failure);
// 		}
	}
};

void UDPDataReceiverThread::execute(OS::IThread*caller,void*arg)
{
	while(caller->isActive())
	{
		m_owner->HandleTick();
	}
}



UDPDataReceiver::UDPDataReceiver()
{
	m_impl=new UDPDataReceiverImpl(this);
}

UDPDataReceiver::~UDPDataReceiver()
{
	delete m_impl;
}


bool UDPDataReceiver::Connect(const core::string& ip,int port)
{
	return m_impl->Connect(ip,port);
}
bool UDPDataReceiver::IsConnected()
{
	return m_impl->IsConnected();
}


void UDPDataReceiver::Start()
{
	m_impl->Start();
}

void UDPDataReceiver::Close()
{
	m_impl->Close();
}

}
}