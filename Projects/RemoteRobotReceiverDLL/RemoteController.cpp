
#include "stdafx.h"
//#pragma warning(X:4005)

#include "RemoteController.h"

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

#define GetCurrentDir _getcwd
using namespace std;


using namespace mray;

#define ROBOT_COMM_PORT 9090

class Mutex
{
	CRITICAL_SECTION m_mutex;
public:
	Mutex(){
		InitializeCriticalSection(&m_mutex);
	}
	~Mutex(){
		DeleteCriticalSection(&m_mutex);
	}

	void lock(){
		EnterCriticalSection(&m_mutex);
	}

	bool tryLock()
	{
		return TryEnterCriticalSection(&m_mutex) ? true : false;
	}

	void unlock(){
		LeaveCriticalSection(&m_mutex);
	}

};

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

class AutoLock
{
	Mutex *_m;
public:
	AutoLock(Mutex& m)
	{
		_m = &m;
		_m->lock();
	}
	~AutoLock()
	{
		_m->unlock();
	}
};





class RemoteControllerImpl
{
public:
	ITelubeeRobotListener* listener;
	RemoteController* _c;

	ClientStatusData clientStatus;
	RemoteRobotStatus robotStatus;
	
	network::NetAddress clientAddress;

	network::IUDPClient* netClientSender;
	network::IUDPClient* netClientReceiver;

	Mutex robotMutex;
	Mutex clientMutex;


	FILE     *OutputLogFile;
	bool threadStart = false;
	bool isDone = false;
	bool upCount = true;
	bool tuningEnabled = false;

	HANDLE hThreadSend;
	HANDLE hThreadRecv;
	static DWORD WINAPI timerThreadSend(RemoteControllerImpl *robot, LPVOID pdata);
	static DWORD WINAPI timerThreadRecv(RemoteControllerImpl *robot, LPVOID pdata);

	RemoteControllerImpl(RemoteController* c)
	{
		netClientSender = netClientReceiver = 0;
		_c = c;
		listener = 0;
		isDone = false;
		threadStart = false;
		robotStatus.status = ERobotControllerStatus::EStopped;
		hThreadSend = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadSend, this, NULL, NULL);
		hThreadRecv = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadRecv, this, NULL, NULL);

		clientStatus.ipAddress = network::NetAddress::AnyAddr;
	}
	~RemoteControllerImpl()
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
	}
	void NotifyCollision(float l, float r)
	{
		if (listener)
		{
			listener->OnCollisionData(_c,l, r);
		}
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
		_c->_processData();

		if (clientStatus.ipAddress.address == network::NetAddress::AnyAddr.address)
			return;
		wrtr.writeByte((byte)MSG_RobotStatus);
		RemoteRobotStatus st;
		robotMutex.lock();
		st = robotStatus;
		robotMutex.unlock();

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
			clientMutex.lock();
			clientStatus = st;
			clientMutex.unlock();
		}
	}
	void CallRemoteProcedure()
	{
		char buffer[64];

	}

	void ChangeState(ERobotControllerStatus s)
	{
		bool updated = true;
		robotMutex.lock();
		if (s==EIniting && robotStatus.status==EStopped)
			robotStatus.status = s;
		if (s == EDisconnected && (robotStatus.status == EIniting || robotStatus.status == EDisconnecting))
			robotStatus.status = s;
		else if (s==EConnecting && robotStatus.status==EDisconnected)
			robotStatus.status = s;
		else if (s == EConnected && robotStatus.status == EConnecting)
			robotStatus.status = s;
		else if (s == EDisconnecting && robotStatus.status == EConnected)
			robotStatus.status = s;
		else if (s == EStopping && (robotStatus.status == EConnected || robotStatus.status == EDisconnected))
			robotStatus.status = s;
		else if (s == EStopped && robotStatus.status == EStopping)
			robotStatus.status = s;
		else updated = false;

		robotMutex.unlock();
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

				Sleep(1000);
				ChangeState(ERobotControllerStatus::EDisconnected);
				break;
			case ERobotControllerStatus::EConnecting:
				//connect to robot
				Sleep(1000);
				ChangeState(ERobotControllerStatus::EConnected);
				break;
			case ERobotControllerStatus::EDisconnecting:
				//Disconnect the robot
				Sleep(1000);
				ChangeState(ERobotControllerStatus::EDisconnected);
				break;
			case  ERobotControllerStatus::EStopping:
				//shutdown the robot
				Sleep(1000);
				ChangeState(ERobotControllerStatus::EStopped);
				break;
			default:
				break;
			}
		}
	}

	ERobotControllerStatus GetState()
	{
		AutoLock l(robotMutex);
		return robotStatus.status;
	}
};


DWORD RemoteControllerImpl::timerThreadSend(RemoteControllerImpl *robot, LPVOID pdata){
	

	char buffer[512];
	uint len = 512;
	network::NetAddress src;
	OS::CMemoryStream stream("", (byte*)buffer, len, false,OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	while (!robot->isDone){
		if (robot->threadStart &&
			robot->clientStatus.ipAddress.address != network::NetAddress::AnyAddr.address){
			stream.seek(0, OS::ESeek_Set);
			robot->_Send(wrtr,stream);
		}
		else
			Sleep(50);
	}

	return 0;
}

DWORD RemoteControllerImpl::timerThreadRecv(RemoteControllerImpl *robot, LPVOID pdata){
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


RemoteController::RemoteController()
{
	m_impl = new RemoteControllerImpl(this);

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

	m_impl->threadStart = true;

	m_robotStatusProvider = 0;

}

RemoteController::~RemoteController()
{
	DisconnectRobot();
	m_impl->isDone = true;
	delete m_impl;


	
}


void RemoteController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	if (m_impl->GetState() != ERobotControllerStatus::EStopped)
		return;

	m_robotStatusProvider = robotStatusProvider;

	m_impl->ChangeState(ERobotControllerStatus::EIniting);

	return;
}

void RemoteController::_setupCaps()
{
}
ERobotControllerStatus RemoteController::GetRobotStatus() {
	AutoLock a(m_impl->robotMutex);
	return m_impl->robotStatus.status;
}


bool RemoteController::GetJointValues(std::vector<float>& values){
	if (m_impl->robotMutex.tryLock())
	{
		values = m_impl->robotStatus.jointValues;
		m_impl->robotMutex.unlock();
		return true;
	}
	else
		return false;
}


void RemoteController::ConnectRobot()
{
	if (m_impl->GetState() == ERobotControllerStatus::EDisconnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EConnecting);
	}
	return;
}

void RemoteController::ManualControlRobot()
{
	return;
}

void RemoteController::DisconnectRobot()
{
	if (m_impl->GetState() == ERobotControllerStatus::EDisconnecting ||
		m_impl->GetState() == ERobotControllerStatus::EConnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EDisconnecting);
	}
	return;
}

void RemoteController::ShutdownRobot(){

	if (m_impl->GetState() != ERobotControllerStatus::EDisconnecting ||
		m_impl->GetState() != ERobotControllerStatus::EDisconnected)
	{
		m_impl->ChangeState(ERobotControllerStatus::EStopping);
	}
	return;
}

void RemoteController::tuningMode(){


	return;
}


void RemoteController::UpdateRobotStatus(const RobotStatus& st)
{
	m_impl->clientMutex.lock();
	m_impl->clientStatus.status = st;
	m_impl->clientMutex.unlock();
}


void RemoteController::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}
void RemoteController::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}


void RemoteController::ParseParameters(const std::map<std::string, std::string>& valueMap)
{
}

