
#include "stdafx.h"
#include "ServiceLoader.h"

#include "WinFileSystem.h"
#include "Engine.h"
#include "WinOSystem.h"
#include "win32NetInterface.h"
#include "XMLTree.h"
#include "StringUtil.h"
#include "tinyxml2.h"
#include "Console.h"
#include "NetworkValueController.h"

#include <windows.h>

#undef StartService

namespace mray
{
	typedef void(*dllFunctionPtr)();
	typedef void*(*dllFunctionGetObjectPtr)();

	class ServiceLoaderThread :public OS::IThreadFunction
	{
		ServiceLoader* m_owner;
	public:
		ServiceLoaderThread(ServiceLoader* o)
		{
			m_owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (m_owner->_ProcessPacket() != 0)
				{
					return;
				}
				//		OS::IThreadManager::getInstance().sleep(1);
			}
		}

	};

	class ServiceLoaderRenderContext :public TBee::ServiceRenderContext
	{
		math::vector2di m_currPos;
	public:
		virtual void RenderText(const core::string &txt, int x, int y, const video::SColor& clr = 1)
		{
			m_currPos += math::vector2di(x, y);
			Console::locate(m_currPos.x, m_currPos.y);
			Console::setColor(clr.R>0.5, clr.G>0.5, clr.B>0.5);
			printf("%s\n", txt.c_str());
			m_currPos.y++;
		}
		virtual void Reset()
		{
			m_currPos = 0;
 			Console::clear();
 			Console::locate(m_currPos.x, m_currPos.y);
		}
	};

ServiceLoader::ServiceLoader()
{
	m_inited = false;
}
ServiceLoader::~ServiceLoader()
{
	_destroy();
	delete m_renderContext;
}

bool ServiceLoader::Init(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		Console::setColor(CONSOLE_CLR_ERROR);
		printf("The service requires the target module name as a command line argument!\n"
			"Usage: %s ServiceName.dll\n", argv[0]);
		return false;
	}
	else
	{
		m_moduleName = argv[1];
	}
	new OS::WinFileSystem();
	new Engine(new OS::WinOSystem());
	gEngine.loadPlugins("plugins.stg");
	gEngine.loadResourceFile("ServicesRes.stg");
	gEngine.createDevice("OpenGL");

	core::string logFile = gFileSystem.getAppPath() + argv[1] + ".log";
	printf("Starting log:%s\n", logFile.c_str());
	GCPtr<StreamLogger> logger = new StreamLogger(true);
	logger->setStream(gFileSystem.createTextFileWriter( logFile));
	gLogManager.addLogDevice(logger);

	network::createWin32Network();

	m_sharedMemory.SetDataSize(sizeof(TBee::ModuleSharedMemory));
	m_sharedMemory.SetName("SH_TBee_Service");

	m_sharedMemory.openRead();
	m_memory = (TBee::ModuleSharedMemory*)m_sharedMemory.GetData();
	if (!m_memory)
	{
		printf("Couldnt open shared memory! Make sure the service host manager is loaded first!\n");
		exit(0);
	}
	memset(&m_oldMemory, 0, sizeof(m_oldMemory));

	Console::setColor(CONSOLE_CLR_INFO);
	printf("Service Host: %s:%d\n", m_memory->hostAddress.toString().c_str(), m_memory->hostAddress.port);
	
	m_context.serviceLoader = this;
	m_context.commChannel = network::INetwork::getInstance().createUDPClient();
	m_context.commChannel->Open();
	m_context.localAddr.address = network::INetwork::getInstance().getLocalAddress();
	m_context.localAddr.port = m_context.commChannel->Port();
	m_context.sharedMemory = m_memory;
	m_context.netValueController = new TBee::NetworkValueController();
	m_context.netValueController->StartReceiver(0);
	m_context.netValueController->GetValues();

	//load settings file
	_loadSettings();

	m_renderContext = new ServiceLoaderRenderContext();

	m_thread = OS::IThreadManager::getInstance().createThread(new ServiceLoaderThread(this));
	m_thread->start(0);

	core::string path;
	gFileSystem.getCorrectFilePath(gFileSystem.getAppPath()+ m_moduleName, path);
	m_moduleLib = OS::IDllManager::getInstance().getLibrary( path);
	if (m_moduleLib)
	{
		dllFunctionPtr libInitPtr;
		dllFunctionGetObjectPtr libGetModule;
		libInitPtr = (dllFunctionPtr)m_moduleLib->getSymbolName("DLL_ServiceInit");
		libGetModule = (dllFunctionGetObjectPtr)m_moduleLib->getSymbolName("DLL_GetServiceModule");

		libInitPtr();
		if (libGetModule)
			m_serviceModule = (TBee::IServiceModule*)libGetModule();

	}
	if (!m_serviceModule)
	{
		m_sharedMemory.Detach();
		delete Engine::getInstancePtr();
		return false;
	}


// 	m_serviceClient = network::INetwork::getInstance().createUDPClient();
// 	m_serviceClient->Open();


	m_dataMutex = OS::IThreadManager::getInstance().createMutex();

	_sendConnectMessage();


	m_serviceModule->InitService(&m_context);

	m_inited = true;

	return true;
}
void ServiceLoader::_sendConnectMessage()
{
	xml::XMLWriter w;
	xml::XMLElement* e = new xml::XMLElement("ServiceModule");
	e->addAttribute("Message", "Connect");
	e->addAttribute("Name", m_serviceModule->GetServiceName());
// 	e->addAttribute("IP", m_context.localAddr.toString());
 	e->addAttribute("Port",core::StringConverter::toString(m_context.localAddr.port));
	w.addElement(e);
	core::string msg= w.flush();

	m_context.commChannel->SendTo(&m_memory->hostAddress, msg.c_str(), msg.length() + 1);

	delete e;
}
void ServiceLoader::_sendDisconnectMessage()
{
	core::string msg = "<ServiceModule Message=\"Disconnect\"/>";

	m_context.commChannel->SendTo(&m_memory->hostAddress, msg.c_str(), msg.length() + 1);

}
void ServiceLoader::_sendPongMessage()
{
	core::string msg = "<ServiceModule Message=\"Pong\"/>";

	m_context.commChannel->SendTo(&m_memory->hostAddress, msg.c_str(), msg.length() + 1);

}
void ServiceLoader::_loadSettings()
{
	xml::XMLTree tree;
	core::string path,name,ext;
//	core::StringUtil::SplitPathFileName(m_moduleName, path, name);
	core::StringUtil::SplitPathExt(m_moduleName, name, ext);
	if (tree.load("Modules\\" + name + ".xml"))
	{
		xml::XMLElement* e = tree.getSubElement(name);
		if (!e)
			return;
		e = e->getSubElement("Parameters");
		if (!e)
			return;
		e = e->getSubElement("Param");
		while (e)
		{
			SOptionElement v;
			v.name = e->getValueString("Name");
			v.value = e->getValueString("Value");
			e = e->nextSiblingElement("Param");
			m_context.appOptions.AddOption(v);
		}
	}
	else
	{
		printf("Failed to load Module Settings!\n");
	}
}

void ServiceLoader::_MonitorEvents()
{
	if (m_memory->UserConnected && !m_oldMemory.UserConnected)
	{
		m_oldMemory.userConnectionData = m_memory->userConnectionData;
		OnUserConnected(m_oldMemory.userConnectionData);
	}
	if (!m_memory->UserConnected && m_oldMemory.UserConnected)
	{
		m_oldMemory.userConnectionData = m_memory->userConnectionData;
		OnUserDisconnected(m_oldMemory.userConnectionData.userData.clientAddress);
	}

	m_oldMemory.UserConnected = m_memory->UserConnected;
}

void ServiceLoader::_destroy()
{
	if (m_inited)
	{
		m_inited = false;
		_sendDisconnectMessage();

		m_serviceModule->DestroyService();
		m_context.commChannel->Close();
		delete m_context.commChannel;
		m_context.commChannel = 0;
		if (m_moduleLib)
		{
			dllFunctionPtr libDestroyPtr;
			libDestroyPtr = (dllFunctionPtr)m_moduleLib->getSymbolName("DLL_SreviceDestroy");
			if (libDestroyPtr)
				libDestroyPtr();
		}
		m_moduleLib = 0;

		delete m_context.netValueController;
		m_context.netValueController = 0;

		OS::IThreadManager::getInstance().killThread(m_thread);

		delete m_thread;
		m_thread = 0;

// 		delete m_serviceClient;
// 		m_serviceClient = 0;

		delete m_dataMutex;

		m_sharedMemory.Detach();
	//	delete Engine::getInstancePtr();

	}
}

void ServiceLoader::_UpdateServiceStatus()
{
	if (m_memory->UserConnected && (m_serviceModule->GetServiceStatus() == TBee::EServiceStatus::Inited ||
		m_serviceModule->GetServiceStatus() == TBee::EServiceStatus::Stopped))
	{
		m_serviceModule->StartService(&m_context);
	}

	if (!m_memory->UserConnected && m_serviceModule->GetServiceStatus() == TBee::EServiceStatus::Running)
	{
		m_serviceModule->StopService();
	}
}

void ServiceLoader::Run()
{
	if (!m_inited)
		return;

	float dt=30.0/1000.0f;

	while (m_serviceModule->GetServiceStatus() != TBee::EServiceStatus::Shutdown)
	{
		_MonitorEvents();
		_UpdateServiceStatus();
		m_serviceModule->Update(dt);
		_RenderInfo();
		Sleep(dt*1000);
	}
	
}

void ServiceLoader::_RenderInfo()
{
	m_renderContext->Reset();

	m_renderContext->RenderText("Service Name: " + m_moduleName, 0, 0, video::SColor(CONSOLE_CLR_INFO, 1));
	m_renderContext->RenderText("NetValues Port: " + core::StringConverter::toString(m_context.netValueController->GetPort()), 0, 0, video::SColor(CONSOLE_CLR_INFO, 1));
	m_serviceModule->Render(m_renderContext);
	m_serviceModule->DebugRender(m_renderContext);

	m_renderContext->RenderText(core::string("Status: ") + (m_memory->UserConnected? "User Connected":"User Disconnected"), 0, 0, video::SColor(CONSOLE_CLR_INFO, 1));
	if (m_memory->UserConnected)
	{
		m_renderContext->RenderText("User IP Address:" + m_memory->userConnectionData.userData.clientAddress.toString(), 0, 0, video::SColor(CONSOLE_CLR_SUCCESS, 1));
	}
	if (_isProcessLocked())
	{
		m_renderContext->RenderText("Process is locked!", 0, 0, video::SColor(CONSOLE_CLR_INFO, 1));
	}
}

bool ServiceLoader::_ProcessPacket()
{
	if (!m_context.commChannel || !m_context.commChannel->IsOpen())
		return -1;
#define MAX_BUFFER 4096*4
	char buffer[MAX_BUFFER];
	network::NetAddress src;
	uint len = MAX_BUFFER;
	if (m_context.commChannel->RecvFrom(buffer, &len, &src, 0) != network::UDP_SOCKET_ERROR_NONE)
		return 0;//failed to receive the packet

	buffer[len] = 0;


	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.Parse(buffer);
	if (err != tinyxml2::XML_NO_ERROR)
	{
		return 0;
	}
	tinyxml2::XMLElement*root = doc.RootElement();

	//Process messages from the host manager
	std::string msg = root->Attribute("Message");
	if (msg == "Ping")
	{
		//printf("Recevied ping message\n");
		_sendPongMessage();
	}
	else if (msg == "ServiceLock")
	{
		std::string requester=root->Attribute("Service");
		if (requester != m_serviceModule->GetServiceName())
		{
			_lockProcess(requester);
		}
	}
	else if (msg == "ServiceUnlock")
	{
		std::string requester = root->Attribute("Service");
		if (requester != m_serviceModule->GetServiceName())
		{
			_unlockProcess(requester);
		}
	}
	else
	{
		//printf("Message Arrived: %s\n", msg.c_str());
		core::string value = root->Attribute("Value");
		m_context.__FIRE_OnUserMessage(&src, msg, value);
	}

	return 0;

}
bool ServiceLoader::_isProcessLocked()
{
	bool locked = false;

	for (int i = 0; i < m_processLocks.size(); ++i)
	{
		if (m_processLocks[i].count>0)
		{
			locked = true;
			break;
		}
	}

	return locked;
}
void ServiceLoader::_lockProcess(const std::string& requester)
{
	//first, lock the process
	if (!_isProcessLocked())
	{
		//lock here
	}
	for (int i = 0; i < m_processLocks.size(); ++i)
	{
		if (m_processLocks[i].process == requester)
		{
			m_processLocks[i].count++;
			return;
		}
	}
	ProcessLock l;
	l.count = 1;
	l.process = requester;
	m_processLocks.push_back(l);
}
void ServiceLoader::_unlockProcess(const std::string& requester)
{
	if (!_isProcessLocked())
		return;

	for (int i = 0; i < m_processLocks.size(); ++i)
	{
		if (m_processLocks[i].process == requester)
		{
			m_processLocks[i].count--;
			if (m_processLocks[i].count == 0)
			{
				m_processLocks.erase(m_processLocks.begin() + i);
			}
			break;
		}
	}
	if (!_isProcessLocked())
	{
		//unlock process here
	}
}

void ServiceLoader::RequestLock()
{
	core::string msg;
	"<Broadcast Message=\"ServiceLock\" Service=\"" + m_serviceModule->GetServiceName() + "\"/>";
	m_context.commChannel->SendTo(&m_memory->hostAddress, msg.c_str(), msg.length() + 1);

}
void ServiceLoader::RequestUnlock()
{
	core::string msg;
	"<Broadcast Message=\"ServiceUnlock\" Service=\"" + m_serviceModule->GetServiceName() + "\"/>";
	m_context.commChannel->SendTo(&m_memory->hostAddress, msg.c_str(), msg.length() + 1);

}
//////////////////////////////////////////////////////////////////////////

void ServiceLoader::OnUserConnected( const TBee::UserConnectionData& data)
{
	if (m_context.remoteAddr.address != data.userData.clientAddress.address)
	{
		Console::setColor(CONSOLE_CLR_INFO);
		printf("User Connected : %s\n", data.userData.clientAddress.toString().c_str());
	}
	m_context.remoteAddr = data.userData.clientAddress;
	//m_videoProvider->StreamDataTo(address,videoPort,audioPort);
	int ip[4];
	data.userData.clientAddress.getIP(ip);
	core::string ipaddr = core::StringConverter::toString(ip[0]) + "." +
		core::StringConverter::toString(ip[1]) + "." +
		core::StringConverter::toString(ip[2]) + "." +
		core::StringConverter::toString(ip[3]);

	m_context.__FIRE_OnUserConnected(data);

}
void ServiceLoader::OnUserDisconnected( const network::NetAddress& address)
{
	Console::setColor(CONSOLE_CLR_INFO);
	printf("User Disconnected : %s\n", address.toString().c_str());
	m_context.__FIRE_OnUserDisconnected();
}

}

