


/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   19:46
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\UDPCommunicationLayer.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	UDPCommunicationLayer
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___UDPCommunicationLayer___
#define ___UDPCommunicationLayer___


#include "ICommunicationLayer.h"
#include "CommunicationData.h"

namespace mray
{
namespace network
{
	class IUDPClient;
}
namespace OS
{
	class IThread;
}
namespace VT
{

	enum EUDPMessageType
	{
		ESchemeMessage=0x100,
		EDataMessage,
		ECustomMessage
	};


	class IUDPCommunicationLayerListener
	{
	public:
		virtual void OnCustomMessage(OS::IStream* stream){};
	};

	//UDP CommunicationLayer implementation, converts to robot scheme, and pass robot data to the assigned listener
class UDPCommunicationLayer:public ICommunicationLayer
{
protected:
	network::IUDPClient* m_client;
	GCPtr<OS::IThread> m_thread;
	OS::IMutex* m_dataMutex;
	CommunicationData m_data;
	ushort m_port;

	std::vector<core::string> m_scheme;

	IUDPCommunicationLayerListener* m_msgListener;
public:
	UDPCommunicationLayer(ushort port);
	virtual~UDPCommunicationLayer();

	virtual bool Open(ushort port);

	virtual void Update(float dt);

	virtual void Start();
	virtual void Close();

	void SetMsgListener(IUDPCommunicationLayerListener*l){m_msgListener=l;}
	IUDPCommunicationLayerListener* GetMsgListener(){return m_msgListener;}

	virtual const std::vector<core::string>& GetScheme(){return m_scheme;};
	

	void _ReceiveData(OS::IStream* stream);
};

}
}

#endif
