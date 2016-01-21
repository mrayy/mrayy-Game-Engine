
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

#define ROBOT_COMM_PORT 9090

using namespace mray;


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



bool isnan(double var){
	volatile double d = var;
	return d != d;
}



class RemoteControllerImpl
{
public:
	ITelubeeRobotListener* listener;
	RemoteController* _c;

	ClientStatusData clientStatus;
	RemoteRobotStatus robotStatus;

	std::string robotIP;
	std::string localIP;

	network::IUDPClient* netClientSender;
	network::IUDPClient* netClientReceiver;
	network::NetAddress robotAddress;

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
		netClientSender = 0;
		netClientReceiver = 0;
		_c = c;
		listener = 0;
		isDone = false;
		threadStart = false;
		clientStatus.controlStatus = ERobotControllerStatus::EStopped;
		robotStatus.status= ERobotControllerStatus::EStopped;
		hThreadSend = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadSend, this, NULL, NULL);
		hThreadRecv = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadRecv, this, NULL, NULL);
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

	void _Send(OS::StreamWriter &wrtr,OS::CMemoryStream& stream)
	{
		_c->_processData();

		wrtr.writeByte((byte)MSG_ClientStatus);
		clientMutex.lock();
		wrtr.write(&clientStatus,sizeof(clientStatus));
		clientMutex.unlock();

		netClientSender->SendTo(&robotAddress,(const char*)stream.getData(),stream.getPos());
	}
	void _Receive(OS::StreamReader& rdr,const network::NetAddress& src)
	{
		byte msg= rdr.readByte();

		RemoteRobotStatus st;

		switch ((EMessageType)msg)
		{
		case MSG_RobotStatus:
		{
			//parse robot status
			rdr.read(&st.status, sizeof(st.status));
			int joints=rdr.binReadInt();
			for (int i = 0; i < joints;++i)
			{
				st.jointValues.push_back(rdr.binReadFloat());
			}

			//now copy this status to local memory
			robotMutex.lock();
			memcpy(&robotStatus, &st, sizeof(st));
			ChangeState(robotStatus.status);
			robotMutex.unlock();
			if (st.status == ERobotControllerStatus::EStopped) //the robot has been totally disconnected
			{
//				threadStart = false;//stop the thread until the robot is reinited again..
			}
		}
			break;
		default:
			break;
		}
	}
	void CallRemoteProcedure()
	{
		char buffer[64];

	}

	void ChangeState(ERobotControllerStatus s)
	{
		bool updated = true;
		/**/
		printf("Requesting: ");
		switch (s)
		{
		case EStopped:
			printf("EStopped\n");
			break;
		case EIniting:
			printf("EIniting!\n");
			break;
		case EStopping:
			printf("EStopping!\n");
			break;
		case EDisconnected:
			printf("EDisconnected!\n");
			break;
		case EDisconnecting:
			printf("EDisconnecting!\n");
			break;
		case EConnected:
			printf("EConnected!\n");
			break;
		case EConnecting:
			printf("EConnecting!\n");
			break;
		default:
			break;
		}

		clientMutex.lock();
		if (s == EIniting && clientStatus.controlStatus == EStopped)
			clientStatus.controlStatus = s;
		else if (s == EDisconnected && (clientStatus.controlStatus == EIniting || clientStatus.controlStatus == EDisconnecting))
			clientStatus.controlStatus = s;
		else if (s == EConnecting && clientStatus.controlStatus == EDisconnected)
			clientStatus.controlStatus = s;
		else if (s == EConnected && clientStatus.controlStatus == EConnecting)
			clientStatus.controlStatus = s;
		else if (s == EDisconnecting && clientStatus.controlStatus == EConnected)
			clientStatus.controlStatus = s;
		else if (s == EStopping && (clientStatus.controlStatus == EConnected || clientStatus.controlStatus == EDisconnected))
			clientStatus.controlStatus = s;
		else if (s == EStopped && clientStatus.controlStatus == EStopping)
			clientStatus.controlStatus = s;
		else updated = false;
		clientMutex.unlock();

		if (!updated)
			return;
		switch (s)
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

	ERobotControllerStatus GetState()
	{
		AutoLock l(clientMutex);
		return clientStatus.controlStatus;
	}
};


DWORD RemoteControllerImpl::timerThreadSend(RemoteControllerImpl *robot, LPVOID pdata){
	

	char buffer[512];
	uint len=512;
	network::NetAddress src;
	OS::CMemoryStream stream("", (byte*)buffer, len, false,OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	while (!robot->isDone){
		if (robot->threadStart){
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
	uint len = 512;
	network::NetAddress src;
	OS::StreamReader rdr;
	while (!robot->isDone){
		if (robot->threadStart){
			len = sizeof(buffer);
			if (robot->netClientReceiver->RecvFrom(buffer, &len, &src, 0) == network::UDP_SOCKET_ERROR_NONE && len>0)
			{
				OS::CMemoryStream stream("",(byte*) buffer, len, false);
				rdr.setStream(&stream);
				robot->_Receive(rdr,src);
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

	if (!m_impl->netClientReceiver)
	{
		m_impl->netClientReceiver = network::INetwork::getInstance().createUDPClient();
		m_impl->netClientReceiver->Open();

		m_impl->clientStatus.ipAddress = network::NetAddress(m_impl->localIP, m_impl->netClientReceiver->Port());

	}
	if (!m_impl->netClientSender)
	{
		m_impl->netClientSender = network::INetwork::getInstance().createUDPClient();
		m_impl->netClientSender->Open();
	}

	m_impl->robotAddress = network::NetAddress(m_impl->robotIP, ROBOT_COMM_PORT);
	m_impl->threadStart = true;
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
	std::map<std::string, std::string>::const_iterator it= valueMap.find("RobotIP");
	if (it != valueMap.end())
	{
		m_impl->robotIP = it->second;
	}
	it = valueMap.find("LocalIP");
	if (it != valueMap.end())
	{
		m_impl->localIP = it->second;
	}
}

