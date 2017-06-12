

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


#include "IUDPClient.h"

namespace mray
{


	class RobotSerialPort;
	struct RobotStatus
	{
		bool connected;
		float speedX, speedY;
		float rotation;
		float tilt, yaw, roll;

		RobotStatus()
		{
			connected = false;
			speedX = speedY = 0;
			rotation = 0;
			tilt = yaw = roll = 0;
		}
	};

	struct UserStatus
	{
		network::NetAddress address;
	};


	class RobotCommunicator;
	class IRobotCommunicatorListener
	{
	public:

		virtual void OnUserConnected(RobotCommunicator* sender, const network::NetAddress& address){};
		virtual void OnRobotStatus(RobotCommunicator* sender, const RobotStatus& status){};
	};

class RobotCommunicator
{
protected:

	RobotSerialPort* m_controller;
	RobotStatus m_robotStatus;
	UserStatus m_userStatus;
	void HandleData(const core::string& name, const core::string& value);
	void ProcessPacket(const char* buffer);

	network::IUDPClient* m_client;

	OS::IThread* m_thread;



	IRobotCommunicatorListener* m_listener;
public:
	RobotCommunicator();
	virtual~RobotCommunicator();

	void StartServer(int port);
	void StopServer();

	void SetListener(IRobotCommunicatorListener* l){ m_listener = l; }

	int _Process();
};

}


#endif
