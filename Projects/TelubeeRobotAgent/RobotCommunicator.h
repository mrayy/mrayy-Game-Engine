

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

#include "IRobotController.h"

namespace mray
{



	struct UserStatus
	{
		network::NetAddress address;
	};


	class RobotCommunicator;
	class IRobotCommunicatorListener
	{
	public:

		virtual void OnUserDisconnected(RobotCommunicator* sender, const network::NetAddress& address){}
		virtual void OnUserConnected(RobotCommunicator* sender, const network::NetAddress& address, int videoPort, int audioPort, int handsPort, bool rtcp){};
		virtual void OnRobotStatus(RobotCommunicator* sender, const RobotStatus& status){};

		virtual void OnCalibrationDone(RobotCommunicator* sender){};

		virtual void OnCollisionData(RobotCommunicator* sender, float left, float right){}

		virtual void OnReportMessage(RobotCommunicator* sender, int code, const core::string& msg){}
	};

	class IMessageSink
	{
	public:
		virtual void OnMessage(network::NetAddress* addr,const core::string& msg, const core::string& value){};
	};


class RobotCommunicator :public ITelubeeRobotListener, public IRobotStatusProvider
{
protected:
	OS::IDynamicLibraryPtr m_robotLib;
	IRobotController* m_robotController;
	RobotStatus m_robotStatus;
	UserStatus m_userStatus;
	void HandleData(network::NetAddress* addr,const core::string& name, const core::string& value);
	void ProcessPacket(network::NetAddress* addr,const char* buffer);

	network::IUDPClient* m_client;

	OS::IMutex* m_dataMutex;
	OS::IThread* m_thread;

	IRobotCommunicatorListener* m_listener;
	IMessageSink* m_msgSink;

	bool m_localControl;

	void _RobotStatus(const RobotStatus& st);
public:
	RobotCommunicator();
	virtual~RobotCommunicator();

	void Initialize();

	const RobotStatus& GetRobotStatus()const{
		return m_robotStatus;
	}

	virtual void GetRobotStatus(RobotStatus& st)const ;

	IRobotController* GetRobotController(){ return m_robotController; }

	void StartServer(int port);
	void StopServer();

	void SetLocalControl(bool c){ m_localControl = c; }
	bool IsLocalControl(){ return m_localControl; }

	void SetRobotData(const RobotStatus &st);

	void SetListener(IRobotCommunicatorListener* l){ m_listener = l; }
	void SetMessageSink(IMessageSink*s){ m_msgSink = s; }


	int _Process();

	virtual void OnCollisionData(float left, float right);
	void OnReportMessage(int code, const std::string& msg);

};

}


#endif
