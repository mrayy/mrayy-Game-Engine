


#include "stdafx.h"
#include "ServiceHostManager.h"

#include "WinFileSystem.h"
#include "Engine.h"
#include "WinOSystem.h"
#include "win32NetInterface.h"
#include "XMLTree.h"
#include "StringUtil.h"
#include "MutexLocks.h"
#include "CommunicationMessages.h"
#include "LogManager.h"

#include <tinyxml2.h>
#include <windows.h>
#include <conio.h>
#include <iostream>

#undef StartService

namespace mray
{
#define COMMUNICATION_PORT 6000
#define PingTime 5000

	typedef bool(ServiceHostManager::*ProcessFunction)();
class ServiceHostManagerThread :public OS::IThreadFunction
{
	ServiceHostManager* m_owner;
	ProcessFunction m_function;
	int m_sleepTime;
public:
	ServiceHostManagerThread(ServiceHostManager* o, ProcessFunction f,int sleepTime=0)
	{
		m_owner = o;
		m_function = f;
		m_sleepTime = sleepTime;
	}
	virtual void execute(OS::IThread*caller, void*arg)
	{
		while (caller->isActive())
		{
			if ((m_owner->*m_function)() != 0)
			{
				return;
			}
			if (m_sleepTime > 0){
				OS::IThreadManager::getInstance().sleep(m_sleepTime);
			}
		}
	}

};

ServiceHostManager::ServiceHostManager()
{
	m_inited = false;
	m_robotCommunicator = 0;
	m_autoRestartService = false;
	m_commLink = 0;
	m_commThread = 0;
	m_serviceThread = 0;
	m_dataMutex = 0;
}
ServiceHostManager::~ServiceHostManager()
{
	_destroy();
}

void ServiceHostManager::_destroy()
{
	for (int i = 0; i < m_serviceList.size(); ++i)
	{
		TerminateProcess(m_serviceList[i].processHandle, 0);
		CloseHandle(m_serviceList[i].processHandle);
		CloseHandle(m_serviceList[i].threadHandle);
	}
	m_sharedMemory.Detach();
	if (m_commLink)
		m_commLink->Close();

	OS::IThreadManager::getInstance().killThread(m_commThread);
	delete m_commThread;
	m_commThread = 0;

	OS::IThreadManager::getInstance().killThread(m_serviceThread);
	delete m_serviceThread;
	m_serviceThread = 0;

	delete m_commLink;
	delete m_dataMutex;
}

bool ServiceHostManager::Init(int argc, _TCHAR* argv[])
{
	new OS::WinFileSystem();
	new Engine(new OS::WinOSystem());
	gEngine.loadPlugins("plugins.stg");
	gEngine.createDevice("OpenGL");

	StreamLogger *log = new StreamLogger(true);
	log->setStream(gFileSystem.createTextFileWriter("ServiceHostManager.log"));
	gLogManager.addLogDevice(log);
	gLogManager.log("Service Host Manager Started", ELL_INFO);

	network::createWin32Network();

	m_sharedMemory.SetDataSize(sizeof(TBee::ModuleSharedMemory));
	m_sharedMemory.SetName("SH_TBee_Service");

	m_sharedMemory.createWrite();
	m_sharedMemory.openWrite();
	m_memory = (TBee::ModuleSharedMemory*)m_sharedMemory.GetData();
	m_memory->InitMaster(m_memory);

	{
		TBee::SharedMemoryLock m(m_memory);
	}

	m_commLink = network::INetwork::getInstance().createUDPClient();
	m_commLink->Open();

	m_memory->hostAddress.address= network::INetwork::getInstance().getLocalAddress();
	m_memory->hostAddress.port = m_commLink->Port();
	printf("Service Host: %s:%d\n\n", m_memory->hostAddress.toString().c_str(), m_memory->hostAddress.port);

	m_dataMutex = OS::IThreadManager::getInstance().createMutex();


	printf("Initializing RobotCommunicator\n");
	gLogManager.log("Initializing RobotCommunicator", ELL_INFO);
	m_robotCommunicator = new TBee::RobotCommunicator();
	m_robotCommunicator->StartServer(COMMUNICATION_PORT);
	m_robotCommunicator->SetListener(this);

	m_info.IP = m_memory->hostAddress.toString();
	m_info.name = "Telexistence Robot";

	gLogManager.log("Starting Services", ELL_INFO);
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i] ,"-r")==0)
		{
			m_autoRestartService = true;
			printf("Auto Restart Services: Enabled\n");
		}
		else
		{
			RunLocalService(argv[i]);
		}
	}
//  	RunLocalService("AVStreamServiceModule");
//  	RunLocalService("RobotControlServiceModule");

	m_commThread = OS::IThreadManager::getInstance().createThread(new ServiceHostManagerThread(this, &ServiceHostManager::_ProcessPacket));
	m_commThread->start(0);

	m_serviceThread = OS::IThreadManager::getInstance().createThread(new ServiceHostManagerThread(this, &ServiceHostManager::_ProcessServices, 100));
	m_serviceThread->start(0);

	m_inited = true;
	return true;
}

void ServiceHostManager::Run()
{

	while (true)
	{
		if (kbhit())
		{
			char c = getch();
			if (c == ' ')
			{
				m_memory->userConnectionData.userData.clientAddress= mray::network::NetAddress("127.0.0.1", 1234);
				//m_memory->userConnectionData.videoPort = 7000;
				m_memory->UserConnected = !m_memory->UserConnected;
			}
			else if (c == 'q')
				break;
		}
		Sleep(100);
	}
}
bool ServiceHostManager::RunLocalService(const core::string& name)
{

	gLogManager.log("Creating Service: " + name, ELL_INFO);
	OS::ScopedLock lock(m_dataMutex);
	core::string lpApplicationName = core::string(".\\ServiceLoader.exe"); /* The program to be executed */
	core::string args = lpApplicationName+" "+(name + ".dll");


	STARTUPINFO lpStartupInfo;
	PROCESS_INFORMATION lpProcessInfo;

	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
	memset(&lpProcessInfo, 0, sizeof(lpProcessInfo));
	lpStartupInfo.cb = sizeof lpStartupInfo;
	if (!CreateProcess(lpApplicationName.c_str(),
		(LPSTR)args.c_str(), NULL, NULL,
		NULL, CREATE_NEW_CONSOLE, NULL, NULL,
		&lpStartupInfo,
		&lpProcessInfo
		)) {
		std::cerr << "RunLocalService failed to start module: \"" << lpApplicationName << "\"\n";
		return false;
	}

	ServiceInfo ifo;
	ifo.name = name;
	ifo.processHandle = lpProcessInfo.hProcess;
	ifo.threadHandle = lpProcessInfo.hThread;
	ifo.lastTime = gEngine.getTimer()->getMilliseconds();
	m_serviceList.push_back(ifo);

	printf("Service [%s] process was created\n",name.c_str());
	gLogManager.log("Service Created: " + name, ELL_INFO);

	return true;
}

int ServiceHostManager::GetServiceByAddress(const network::NetAddress* addr)
{
	OS::ScopedLock lock(m_dataMutex);
	for (int i = 0; i < m_serviceList.size(); ++i)
	{
		if (m_serviceList[i].address == *addr)
			return i;
	}
	return -1;
}
int ServiceHostManager::GetServiceByName(const core::string &name)
{
	OS::ScopedLock lock(m_dataMutex);
	for (int i = 0; i < m_serviceList.size(); ++i)
	{
		if (m_serviceList[i].name == name)
			return i;
	}
	return -1;
}


bool ServiceHostManager::_ProcessPacket()
{
	if (!m_commLink->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_commLink->RecvFrom(buffer, &len, &src, 0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	buffer[len] = 0;


	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buffer);
	if (err != tinyxml2::XML_NO_ERROR)
	{
		return 0;
	}
	tinyxml2::XMLElement*root = doc.RootElement();
	core::string rootName = root->Name();
	if (rootName == "ServiceModule")
	{
		std::string msg = root->Attribute("Message");
		if (msg == "Connect") //New service
		{
			ServiceInfo ifo;
			ifo.name = root->Attribute("Name");
			ifo.address = src;


			printf("New service was connected [%s]: %s:%d\n", ifo.name.c_str(), ifo.address.toString().c_str(), ifo.address.port);

			int i = GetServiceByName(ifo.name);
			ifo.lastTime = gEngine.getTimer()->getMilliseconds();
			if (i != -1)
			{

				m_dataMutex->lock();
				m_serviceList[i].lastTime = ifo.lastTime;
				m_serviceList[i].address = src;
				m_dataMutex->unlock();
			}
			else
			{
				m_dataMutex->lock();
				m_serviceList.push_back(ifo);
				m_dataMutex->unlock();
			}
		}
		else if (msg == "Disconnect") //New service
		{
			int i = GetServiceByAddress(&src);
			if (i != -1)
			{
				printf("Service disconnected [%s]\n", m_serviceList[i].name.c_str());
				m_dataMutex->lock();
				m_serviceList.erase(m_serviceList.begin() + i);
				m_dataMutex->unlock();
			}
		}
		else if (msg == "Pong")
		{
			int i = GetServiceByAddress(&src);
			if (i != -1)
			{
				//printf("Pong message recevied from :%s\n", m_serviceList[i].name.c_str());
				m_dataMutex->lock();
				m_serviceList[i].lastTime = gEngine.getTimer()->getMilliseconds();
				m_serviceList[i].pingSent = false;
				m_dataMutex->unlock();
			}
		}
	}
	else if (rootName == "Broadcast") // broadcast message to all services
	{
		for (int i = 0; i < m_serviceList.size(); ++i)
		{
			m_dataMutex->lock();
			if (m_serviceList[i].address.address != 0 && m_serviceList[i].address.port != 0 &&
				m_serviceList[i].address.address != src.address && m_serviceList[i].address.port != src.port)
				m_commLink->SendTo(&m_serviceList[i].address, buffer, len);
			m_dataMutex->unlock();
		}
	}
	
	return 0;
}


bool ServiceHostManager::_ProcessServices()
{
	std::vector<ServiceList::iterator> toRemove;
	std::vector<core::string> servicesToRun;
	ulong t=gEngine.getTimer()->getMilliseconds();
	for (int i = 0; i < m_serviceList.size(); ++i)
	{
		if (!m_serviceList[i].pingSent && (t - m_serviceList[i].lastTime) > PingTime)
		{
			m_serviceList[i].lastTime = t;
			m_serviceList[i].pingSent = true;
			core::string msg = "<Host Message=\"Ping\"/>";

		//	printf("Sending Ping to %s\n", m_serviceList[i].name.c_str());
			m_commLink->SendTo(&m_serviceList[i].address, msg.c_str(), msg.length() + 1);
		}else if (m_serviceList[i].pingSent && (t - m_serviceList[i].lastTime) > PingTime)
		{
			//Service is dead..
			printf("Service %s has stopped working\n", m_serviceList[i].name.c_str());
			toRemove.push_back(m_serviceList.begin()+i);

			//Restart the service
			if (m_autoRestartService)
				servicesToRun.push_back(m_serviceList[i].name);
		}
	}
	m_dataMutex->lock();
	for (int i = 0; i < toRemove.size(); ++i)
	{
		TerminateProcess(toRemove[i]->processHandle, 0);
		CloseHandle(toRemove[i]->processHandle);
		CloseHandle(toRemove[i]->threadHandle);
		m_serviceList.erase( toRemove[i]);
	}
	m_dataMutex->unlock();
	for (int i = 0; i < servicesToRun.size(); ++i)
	{
		RunLocalService(servicesToRun[i]);
	}
	return 0;
}
void ServiceHostManager::OnUserConnected(TBee::RobotCommunicator* sender, const TBee::UserConnectionData& data)
{
	if (!m_inited)
		return;
	if (m_memory->userConnectionData.userData.clientAddress.address != data.userData.clientAddress.address)
		printf("User Connected : %s\n", data.userData.clientAddress.toString().c_str());
	//m_videoProvider->StreamDataTo(address,videoPort,audioPort);
	int ip[4];
	data.userData.clientAddress.getIP(ip);
	core::string ipaddr = core::StringConverter::toString(ip[0]) + "." +
		core::StringConverter::toString(ip[1]) + "." +
		core::StringConverter::toString(ip[2]) + "." +
		core::StringConverter::toString(ip[3]);

	
	m_memory->UserConnected = true;
	m_memory->userConnectionData = data;


}
void ServiceHostManager::OnUserDisconnected(TBee::RobotCommunicator* sender, const network::NetAddress& address)
{
	m_memory->UserConnected =  false;
	printf("User Disconnected : %s\n", address.toString().c_str());
}
void ServiceHostManager::OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
{
	const int BufferLen = 65537;
	uchar buffer[BufferLen];
	OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
	OS::StreamWriter wrtr(&stream);
	int i = msg.find('#');
	core::string m;
	if (i != -1)
		m = msg.substr(0, i);
	else m = msg;

	std::vector<core::string> vals;
	vals = core::StringUtil::Split(value, ",");
	if (m.equals_ignore_case("commPort"))
	{
		TBee::SharedMemoryLock m(m_memory);
		m_memory->userConnectionData.userData.clientAddress.port = core::StringConverter::toInt(value);
		m_memory->userCommPort = m_memory->userConnectionData.userData.clientAddress.port;
	}
	else if (m.equals_ignore_case("detect"))
	{
		printf("Robot scan message was received, sending presence message.\n");
		//detect message arrived from a broadcast, reply to let the src about our existence!
		int reply = (int)TBee::EMessages::Presence;
		int len = stream.write(&reply, sizeof(reply));
		len += m_info.Write(&wrtr);
		network::NetAddress retAddr ;
		retAddr.address = addr->address;
		retAddr.port = core::StringConverter::toInt(value);
		m_commLink->SendTo(&retAddr, (char*)buffer, len);
	}
	else
	{
		//printf("Forwarding Message: %s\n", msg.c_str());
		core::string buffer;
		buffer = "<Data Message=\"" + msg + "\" Value=\"" + value + "\"/>";
		//forward the message to the services
		for (int i = 0; i < m_serviceList.size(); ++i)
		{
			//	printf("Sending Ping to %s\n", m_serviceList[i].name.c_str());
			if (m_serviceList[i].address.address != 0 && m_serviceList[i].address.port!=0)
				m_commLink->SendTo(&m_serviceList[i].address, buffer.c_str(), buffer.length()+1);
		}
	}
#if USE_OPENNI
	else
	if (m.equals_ignore_case("depthSize") && m_depthSend)
	{
		int reply = (int)EMessages::DepthSize;
		int len = stream.write(&reply, sizeof(reply));
		math::vector2di sz = m_openNi->GetSize();
		len += stream.write(&sz, sizeof(sz));
		m_serviceContext.commChannel->SendTo(&m_serviceContext.remoteAddr, (char*)buffer, len);
	}
	else
	if (m.equals_ignore_case("depth") && m_depthSend)
	{
		math::rectf rc = core::StringConverter::toRect(value);
		TBee::DepthFrame* f = m_openNi->GetNormalCalculator().GetDepthFrame();
		m_depthRect.SetFrame(f, rc);
		int reply = (int)EMessages::DepthData;
		int len = stream.write(&reply, sizeof(reply));
		len += m_depthRect.WriteToStream(&stream);
		m_serviceContext.commChannel->SendTo(&m_serviceContext.remoteAddr, (char*)buffer, len);
	}
#endif
}

}

