

/********************************************************************
	created:	2013/12/04
	created:	4:12:2013   12:43
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\RobotCommunicator.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
	file base:	RobotCommunicator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotCommunicator__
#define __RobotCommunicator__


#include "IXMLNetMessageHandler.h"


namespace mray
{
namespace TBee
{



	struct UserStatus
	{
		network::NetAddress receivedAddress;	
		network::NetAddress clientAddress;
	};


	struct UserConnectionData
	{
		UserStatus userData;
		/*
		uint videoPort;
		uint audioPort;
		uint handsPort;
		uint clockPort;
		bool rtcp;*/
	};

	class RobotCommunicator;
	class IRobotCommunicatorListener
	{
	public:

		virtual void OnUserDisconnected(RobotCommunicator* sender, const network::NetAddress& address) = 0;
		virtual void OnUserConnected(RobotCommunicator* sender, const UserConnectionData& data) = 0;
		virtual void OnUserMessage(network::NetAddress* addr, const core::string& target, const core::string& msg, const core::string& value) = 0;
		virtual void OnUserDataArrived(network::NetAddress* addr, const char* buffer) = 0;
	};


class RobotCommunicator :protected IXMLNetMessageHandler
{
protected:
	UserStatus m_userStatus;
	void _HandleData(network::NetAddress* addr, const core::string& target, const core::string& name, const core::string& value);
	void _OnDataArrived(network::NetAddress* addr, const char* buffer);

	IRobotCommunicatorListener* m_listener;


public:
	RobotCommunicator();
	virtual~RobotCommunicator();


	void StartServer(int port);
	void StopServer();

	int GetServerPort(){ return GetPort(); }

	void SetListener(IRobotCommunicatorListener* l){ m_listener = l; }

};

}
}


#endif
