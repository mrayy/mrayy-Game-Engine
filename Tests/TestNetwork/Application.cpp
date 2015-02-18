
#include "stdafx.h"
#include "Application.h"
#include "../common/DemoCamera.h"
#include "mrayNet.h"

namespace mray
{

	class RemoteDebuggerListener:public debug::IRemoteDebuggerListener
	{
	public:
		virtual void OnDebugClientConnected(const network::NetAddress&addr)
		{
			MessageBoxA(0,"Connection Established.","Client",0);
		}
		virtual void OnDebugClientDisconnected(const network::NetAddress&addr)
		{
			MessageBoxA(0,"Connection Lost.","Client",0);
		}
	}g_debuggerListener;

	class CustomSocketListener:public network::SocketListener
	{
	public:
		virtual void onPeerConnected(const network::NetAddress&addr)
		{
			MessageBoxA(0,"Connection Established.","Client",0);
		}
		virtual void onPeerDisconnect(const network::NetAddress&addr)
		{
			MessageBoxA(0,"Connection Lost.","Client",0);
		}
		virtual void onPacketArrived(network::SRecivedMessage*msg)
		{
		}

		virtual void onUnkownPacketArrived(int cmd,network::CMsgBuffer*buffer,const network::NetAddress& address,network::ESendMode mode)
		{
		}
	}g_socketListener;
Application::Application()
{
	m_remoteDebugger=0;
}
Application::~Application()
{
}

void Application::init(core::array<SOptionElement> *extraOptions)
{
	traceFunction(Application);
	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	m_network=network::createWin32Network();

	{
		gImageSetResourceManager.loadImageSet("VistaCG.imageset");
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}

	int g_port=4564;

	if(MessageBoxA(0,"Is RemoteDebugger?","Debugger",MB_YESNO)==6)
	{
		
		/*m_socket=network::INetwork::getInstance().createSocket("Server",true,true);

		m_socket->createSocket(g_port,10,10);
		m_socket->setListener(&g_socketListener);*/
		
		m_remoteDebugger=new debug::RemoteDebugger();
		m_remoteDebugger->setPort(g_port);

		m_remoteDebugger->AddListener(&g_debuggerListener);

		m_remoteDebugger->startLog();

	}else
	{
		/*m_socket=network::INetwork::getInstance().createSocket("Client",false,true);

		if(!m_socket->connectToHost("127.0.0.1",g_port,""))
		{
			MessageBoxA(0,"Failed to connect to server","",0);
		}*/
		m_remoteClient=new debug::RemoteDebugClient();
		m_remoteClient->connect("127.0.0.1",g_port);
	}
	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	m_camera=new DemoCamera(getSceneManager(),100);
	m_viewPort=getSceneManager()->createViewport(mT("MainVP"),0);
	m_viewPort->setCamera(m_camera);
	getSceneManager()->addSceneNode(m_camera);


	scene::LightNode*light=getSceneManager()->createLightNode("");
	light->setPosition(math::vector3d(10,100,10));
}

void Application::update(float dt)
{
	CMRayApplication::update(dt);

	if(m_remoteDebugger)
	{
		m_remoteDebugger->update(dt);
	}
}
void Application::draw()
{
	CMRayApplication::draw();

}


}
