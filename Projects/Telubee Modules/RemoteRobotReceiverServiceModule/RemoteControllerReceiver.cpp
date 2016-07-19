
#include "stdafx.h"
//#pragma warning(X:4005)

#include "RemoteControllerReceiver.h"

#include <stdio.h>
#include <windows.h> // for GetAsyncKeyState
#include <iostream>
#include <fstream>
#include <math.h>
#include <direct.h>

#include <stdio.h>
#include <stdlib.h>

#include "INetwork.h"
#include "IUDPClient.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include "CMemoryStream.h"
#include "RobotHandler.h"
#include "Console.h"
#include "StringConverter.h"
#include "LocalDLLRobotController.h"

#include "IThreadManager.h"
#include "MutexLocks.h"

#define GetCurrentDir _getcwd
using namespace std;


namespace mray
{
	namespace TBee
	{

#define ROBOT_COMM_PORT 9090



enum EMessageType
{
	MSG_RobotStatus=100,
	MSG_ClientStatus,
	MSG_RPC,

};
struct ClientStatusData
{
	network::NetAddress ipAddress;
	ERobotControllerStatus controlStatus;
	RobotStatus status;

};
class RemoteRobotStatus
{
protected:
public:

	ERobotControllerStatus status;

	std::vector<float> jointValues;

public:
	RemoteRobotStatus()
	{
		status = ERobotControllerStatus::EDisconnected;
	}
	~RemoteRobotStatus()
	{
	}
};





class RemoteControllerReceiverImpl :public ITelubeeRobotListener,public IRobotStatusProvider
{
public:
	ITelubeeRobotListener* listener;
	RemoteControllerReceiver* _c;


	ClientStatusData clientStatus;
	RemoteRobotStatus robotStatus;
	
	network::NetAddress clientAddress;

	network::IUDPClient* netClientSender;
	network::IUDPClient* netClientReceiver;

	TBee::LocalDLLRobotController* m_robotController;

	GCPtr<OS::IMutex> robotMutex;
	GCPtr<OS::IMutex> clientMutex;


	FILE     *OutputLogFile;
	bool threadStart = false;
	bool isDone = false;
	bool upCount = true;
	bool tuningEnabled = false;

	HANDLE hThreadSend;
	HANDLE hThreadRecv;
	static DWORD WINAPI timerThreadSend(RemoteControllerReceiverImpl *robot, LPVOID pdata);
	static DWORD WINAPI timerThreadRecv(RemoteControllerReceiverImpl *robot, LPVOID pdata);

	RemoteControllerReceiverImpl(RemoteControllerReceiver* c)
	{
		netClientSender = netClientReceiver = 0;
		_c = c;
		listener = 0;
		isDone = false;
		threadStart = false;

		robotMutex = OS::IThreadManager::getInstance().createMutex();
		clientMutex = OS::IThreadManager::getInstance().createMutex();

		robotStatus.status = ERobotControllerStatus::EStopped;

		clientStatus.ipAddress = network::NetAddress::AnyAddr;


	}
	~RemoteControllerReceiverImpl()
	{
		threadStart = false;
		isDone = true;
		Sleep(100);
		TerminateThread(hThreadRecv, 0);
		TerminateThread(hThreadSend, 0);
		if (netClientReceiver)
		{
			netClientReceiver->Close();
			netClientReceiver = 0;
		}
		if (netClientSender)
		{
			netClientSender->Close();
			netClientSender = 0;
		}

		m_robotController->ShutdownRobot();
		delete m_robotController;
	}

	void Init(core::string robotDLL)
	{

		hThreadSend = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadSend, this, NULL, NULL);
		hThreadRecv = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadRecv, this, NULL, NULL);

		m_robotController = new TBee::LocalDLLRobotController(robotDLL);
		m_robotController->SetListener(this);
	}

	void logString(const char* format, ...)
	{
		va_list arglist;

		char Buffer[512];

		va_start(arglist, format);
		vprintf(format, arglist);
		vsprintf(Buffer, format, arglist);
		va_end(arglist);
		OutputLogFile = fopen("RemoteLog.txt", "a+");
		fprintf(OutputLogFile, "Msg --> %s", Buffer);
		fclose(OutputLogFile);
	}

	void _Send(OS::StreamWriter &wrtr, OS::CMemoryStream& stream)
	{
	//	_c->_processData();

		if (clientStatus.ipAddress.address == network::NetAddress::AnyAddr.address)
			return;
		wrtr.writeByte((byte)MSG_RobotStatus);
		RemoteRobotStatus st;
		robotMutex->lock();
		st = robotStatus;
		robotMutex->unlock();

		wrtr.write(&st.status, sizeof(st.status));
		int cnt = st.jointValues.size();
		wrtr.binWriteInt(cnt);
		for (int i = 0; i < cnt; ++i)
		{
			wrtr.binWriteFloat(st.jointValues[i]);
		}
		netClientSender->SendTo(&clientStatus.ipAddress, (const char*)stream.getData(), stream.getPos());
	}
	void _Receive(OS::StreamReader& rdr,const network::NetAddress& src)
	{
		byte msg= rdr.readByte();

		RemoteRobotStatus st;

		if ((EMessageType)msg == MSG_ClientStatus)
		{
			ClientStatusData st;
			rdr.read(&st, sizeof(st));
			clientMutex->lock();
			clientStatus = st;
			clientStatus.ipAddress.address = src.address;
			clientMutex->unlock();
		}
	}
	void CallRemoteProcedure()
	{

	}

	void ChangeState(ERobotControllerStatus s)
	{
		bool updated = true;
		robotMutex->lock();
		if (s==EIniting && robotStatus.status==EStopped)
			robotStatus.status = s;
		if (s == EDisconnected && (robotStatus.status == EIniting || robotStatus.status == EDisconnecting))
			robotStatus.status = s;
		else if (s == EDisconnected && robotStatus.status == EStopped)
		{
			robotStatus.status = EIniting;
		}
		else if (s==EConnecting && robotStatus.status==EDisconnected)
			robotStatus.status = s;
		else if (s == EConnecting && robotStatus.status == EStopped)
			robotStatus.status = EIniting;
		else if (s == EConnected && robotStatus.status == EConnecting)
			robotStatus.status = s;
		else if (s == EConnected && robotStatus.status == EStopped)
		{
			//do initing sequence
			robotStatus.status = EIniting;
		}
		else if (s == EConnected && robotStatus.status == EDisconnected)
		{
			robotStatus.status = EConnecting;
		}
		else if (s == EDisconnecting && robotStatus.status == EConnected)
			robotStatus.status = s;
		else if (s == EStopping && (robotStatus.status == EConnected || robotStatus.status == EDisconnected))
			robotStatus.status = s;
		else if (s == EStopped && robotStatus.status == EStopping)
			robotStatus.status = s;
		else updated = false;

		robotMutex->unlock();
		if (!updated)
			return;
		switch (robotStatus.status)
		{
		case EStopped:
			printf("Robot Stopped!\n");
			break;
		case EIniting:
			printf("Robot EIniting!\n");
			break;
		case EStopping:
			printf("Robot EStopping!\n");
			break;
		case EDisconnected:
			printf("Robot EDisconnected!\n");
			break;
		case EDisconnecting:
			printf("Robot EDisconnecting!\n");
			break;
		case EConnected:
			printf("Robot EConnected!\n");
			break;
		case EConnecting:
			printf("Robot EConnecting!\n");
			break;
		default:
			break;
		}
	}

	void _Update()
	{
		if (GetState() != clientStatus.controlStatus)
		{
			ChangeState(clientStatus.controlStatus);

			switch (robotStatus.status)
			{
			case ERobotControllerStatus::EIniting:
				//init the robot
				m_robotController->InitializeRobot(this);
				ChangeState(ERobotControllerStatus::EDisconnected);
				break;
				
			case ERobotControllerStatus::EConnecting:
				//connect to robot
				m_robotController->ConnectRobot();
				ChangeState(ERobotControllerStatus::EConnected);
				break;
			case ERobotControllerStatus::EDisconnecting:
				//Disconnect the robot
				m_robotController->DisconnectRobot();
				ChangeState(ERobotControllerStatus::EDisconnected);
				break;
			case  ERobotControllerStatus::EStopping:
				//shutdown the robot
				m_robotController->ShutdownRobot();
				ChangeState(ERobotControllerStatus::EStopped);
				break;
			case ERobotControllerStatus::EStopped:
			{
			}break;
			default:
				break;
			}

		}
	}

	ERobotControllerStatus GetState()
	{
		OS::ScopedLock l(robotMutex);
		return robotStatus.status;
	}

	//request data is called from robot side
	virtual void GetRobotStatus(RobotStatus& st)
	{
		clientMutex->lock();
		memcpy(&st, &clientStatus.status, sizeof(st));
		clientMutex->unlock();
		_RobotStatus(st);

	}
	virtual bool RequestData(TBee::RobotHandler* r, RobotStatus& status)
	{
		if (clientMutex->tryLock())
		{
			memcpy(&status, &clientStatus.status, sizeof(status));
			clientMutex->unlock();
			return true;
		}
		return false;
	}


	void _RobotStatus(const RobotStatus& st)
	{
		if (!m_robotController)
			return;

		m_robotController->UpdateRobotStatus(st);
	}
	void Render(ServiceRenderContext* context)
	{

		RobotStatus m_robotData;
		if (!RequestData(0, m_robotData))
			return;

		
		char buffer[512];
		float yoffset = 50;

		core::string msg;
		{
			msg = "Local IP:" ;
		}
		{
			ERobotControllerStatus st = m_robotController->GetRobotStatus();
			msg = core::string("Robot Status: ");
			if (st == EStopped)msg += "Stopped";
			if (st == EDisconnected)msg += "Disconnected";
			if (st == EConnecting)msg += "Connecting";
			if (st == EDisconnecting)msg += "Disconnecting";
			if (st == EConnected)msg += "Connected";
			context->RenderText(msg, 50, 0);
		}
		msg = core::string("User Controlling: ") + (m_robotData.connected ? "Yes" : "No");
		context->RenderText(msg, 50, 0);
		if (m_robotData.connected )
		{

			sprintf_s(buffer, "%-2.2f, %-2.2f", m_robotData.speed[0], m_robotData.speed[1]);
			msg = core::string("Speed: ") + buffer;
			context->RenderText(msg, 100, 0);

			msg = core::string("Rotation: ") + core::StringConverter::toString(m_robotData.rotation, 2);
			context->RenderText(msg, 100, 0);

			math::vector3d angles;
			math::quaternion q(m_robotData.headRotation[0], m_robotData.headRotation[3],
				m_robotData.headRotation[1], m_robotData.headRotation[2]);
			q.toEulerAngles(angles);
			angles.set(angles.y, angles.z, angles.x);
			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", angles.x, angles.y, angles.z);
			msg = core::string("Head Rotation: ") + buffer;
			context->RenderText(msg, 100, 0);

			sprintf_s(buffer, "%-2.2f, %-2.2f, %-2.2f", m_robotData.headPos[0], m_robotData.headPos[1], m_robotData.headPos[2]);
			msg = core::string("Head Position: ") + buffer;
			context->RenderText(msg, 100, 0);

		}


		{
			msg = "Robot Started: " + m_robotController->ExecCommand(IRobotController::CMD_IsStarted, "");
			context->RenderText(msg, 100, 0);

			std::vector<float> jvalues;

			m_robotController->GetJointValues(jvalues);
			context->RenderText(core::string("Robot Joint Values:"), 50, 0);

			msg = "";
			context->RenderText("   \tIK\t/ Real", 100, 0);
			for (int i = 0; i < jvalues.size(); i += 2)
			{

				sprintf_s(buffer, "\t%-2.2f\t/ %-2.2f", jvalues[i], jvalues[i + 1]);
				msg = core::string("J[") + core::StringConverter::toString(i / 2) + "]:" + buffer;
				context->RenderText(msg, 100, 0);
			}

		}

	}

};


DWORD RemoteControllerReceiverImpl::timerThreadSend(RemoteControllerReceiverImpl *robot, LPVOID pdata){
	

	char buffer[512];
	uint len = 512;
	network::NetAddress src;
	OS::CMemoryStream stream("", (byte*)buffer, len, false,OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	while (!robot->isDone){
		if (robot->threadStart &&
			robot->clientStatus.ipAddress.address != network::NetAddress::AnyAddr.address){
			stream.seek(0, OS::ESeek_Set);
			robot->_Send(wrtr, stream);
			Sleep(10);
		}
		else
			Sleep(50);
	}

	return 0;
}

DWORD RemoteControllerReceiverImpl::timerThreadRecv(RemoteControllerReceiverImpl *robot, LPVOID pdata){
	char buffer[512];
	uint len;
	network::NetAddress src;
	OS::StreamReader rdr;
	while (!robot->isDone){
		if (robot->threadStart){
			len = sizeof(buffer);
			if (robot->netClientReceiver->RecvFrom(buffer, &len, &src, 0) == network::UDP_SOCKET_ERROR_NONE && len>0)
			{
				OS::CMemoryStream stream("", (byte*)buffer, len, false);
				stream.seek(0, OS::ESeek_Set);
				rdr.setStream(&stream);
				robot->_Receive(rdr,src);
				robot->_Update();
			}
		}
		else
			Sleep(50);
	}

	return 0;
}


RemoteControllerReceiver::RemoteControllerReceiver()
{
	m_impl = new RemoteControllerReceiverImpl(this);

	m_robotStatusProvider = 0;

}
void RemoteControllerReceiver::Init(core::string robotdll)
{
	m_impl->Init(robotdll);

	if (!m_impl->netClientReceiver)
	{
		m_impl->netClientReceiver = network::INetwork::getInstance().createUDPClient();
		m_impl->netClientReceiver->Open(ROBOT_COMM_PORT);
	}
	if (!m_impl->netClientSender)
	{
		m_impl->netClientSender = network::INetwork::getInstance().createUDPClient();
		m_impl->netClientSender->Open();
	}
	gLogManager.log("Start receiving", ELL_INFO);
	m_impl->threadStart = true;
}


RemoteControllerReceiver::~RemoteControllerReceiver()
{
	m_impl->isDone = true;
	delete m_impl;


	
}



void RemoteControllerReceiver::_setupCaps()
{
}
ERobotControllerStatus RemoteControllerReceiver::GetRobotStatus() {
	OS::ScopedLock a(m_impl->robotMutex);
	return m_impl->robotStatus.status;
}

/*
bool RemoteControllerReceiver::GetJointValues(std::vector<float>& values){
	if (m_impl->robotMutex->tryLock())
	{
		values = m_impl->robotStatus.jointValues;
		m_impl->robotMutex->unlock();
		return true;
	}
	else
		return false;
}


void RemoteControllerReceiver::ConnectRobot()
{
	if (m_impl->GetState() == ERobotControllerStatus::EDisconnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EConnecting);
	}
	return;
}

void RemoteControllerReceiver::ManualControlRobot()
{
	return;
}

void RemoteControllerReceiver::DisconnectRobot()
{
	if (m_impl->GetState() == ERobotControllerStatus::EDisconnecting ||
		m_impl->GetState() == ERobotControllerStatus::EConnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EDisconnecting);
	}
	return;
}

void RemoteControllerReceiver::ShutdownRobot(){

	if (m_impl->GetState() != ERobotControllerStatus::EDisconnecting ||
		m_impl->GetState() != ERobotControllerStatus::EDisconnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EStopping);
	}
	return;
}

void RemoteControllerReceiver::tuningMode(){


	return;
}


void RemoteControllerReceiver::UpdateRobotStatus(const RobotStatus& st)
{
// 	m_impl->clientMutex->lock();
// 	m_impl->clientStatus.status = st;
// 	m_impl->clientMutex->unlock();
	printf("%f,%f,%f\n", st.headRotation[0], st.headRotation[1], st.headRotation[2]);
}


void RemoteControllerReceiver::_processData()
{
	RobotStatus st;
	m_impl->GetRobotStatus(st);
	UpdateRobotStatus(st);
// 	if (m_robotStatusProvider)
// 	{
// 		RobotStatus st;
// 		m_robotStatusProvider->GetRobotStatus(st);
// 	}
}
void RemoteControllerReceiver::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}


void RemoteControllerReceiver::ParseParameters(const std::map<core::string, core::string>& valueMap)
{
}
*/
void RemoteControllerReceiver::Render(ServiceRenderContext* context)
{

	m_impl->Render(context);
	
}

}
}