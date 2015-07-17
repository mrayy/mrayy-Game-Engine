

#include "stdafx.h"
#include "Application.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "win32NetInterface.h"
#include "TextureResourceManager.h"
#include "IThreadManager.h"
#include "CMemoryStream.h"
#include "StreamReader.h"

#include <windows.h>


namespace mray
{


	class UDPReceivingThread :public OS::IThreadFunction
	{
	protected:
		network::IUDPClient* m_client;
		Application* m_owner;
	public:

		UDPReceivingThread(network::IUDPClient*c, Application*o)
		{
			m_client = c;
			m_owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			const uint Size = 6000;
			byte buffer[Size];
			while (caller->isActive())
			{
				uint len = Size;
				network::NetAddress addr;
				network::UDPClientError err = m_client->RecvFrom((char*)buffer, &len, &addr, 0);
				if (err == network::UDP_SOCKET_ERROR_NONE)
				{
					OS::CMemoryStream stream("", buffer, len, false);
					m_owner->_ReceiveData(&stream);
				}
				else
				{
					printf("%s\n", network::IUDPClient::TranslateErrorMessage(err).c_str());
				}
			}
		}
	};

Application::Application()
{
	this->m_limitFps = true;
	m_mutex = 0;
	m_rcvThreadFunc = 0;
	m_rcvThread = 0;
	m_rcvClient = 0;

	m_showColor = false;
}

Application::~Application()
{
	if (m_rcvClient)
	{
		m_rcvClient->Close();
	}
	OS::IThreadManager::getInstance().killThread(m_rcvThread);
	delete m_mutex;
	delete m_rcvClient;
}

void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);
}

void Application::StartReciver(int port)
{
	m_rcvClient = network::INetwork::getInstance().createUDPClient();
	m_rcvThreadFunc = new UDPReceivingThread(m_rcvClient, this);
	m_rcvThread = OS::IThreadManager::getInstance().createThread(m_rcvThreadFunc);
	m_mutex = OS::IThreadManager::getInstance().createMutex();
	m_rcvClient->Open(port);

	m_rcvThread->start(0);
}

void Application::_ReceiveData(OS::IStream* stream)
{
	OS::StreamReader rdr(stream);
	m_showColor = rdr.binReadBool();


}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	{

		CMRayApplication::loadResourceFile(mT("Resources.stg"));


		gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
		GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

		//load font
		GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
		gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
		gFontResourceManager.setDefaultFont(font);

	}
	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
	m_mainVP->SetClearColor(video::DefaultColors::White);
	{
		video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), video::EPixel_R8G8B8A8, true);
		renderTargetTex->setBilinearFilter(false);
		renderTargetTex->setTrilinearFilter(false);
		renderTargetTex->setMipmapsFilter(false);
		m_rt = getDevice()->createRenderTarget("", renderTargetTex, 0, 0, 0);
		renderTargetTex->createTexture(math::vector3di(512, 512, 1), video::EPixel_R8G8B8A8);
	}
	network::createWin32Network();

	StartReciver(5123);
	m_guiRenderer = new GUI::GUIBatchRenderer();
	m_guiRenderer->SetDevice(getDevice());

}


void Application::draw(scene::ViewPort* vp)
{
	math::rectf rc(vp->getAbsRenderingViewPort());
	if (m_showColor)
	{
		Engine::getInstance().getDevice()->draw2DRectangle(rc, video::SColor(0, 0, 0, 1));

	}
	else
	{
		Engine::getInstance().getDevice()->draw2DRectangle(rc, video::SColor(1, 1, 1, 1));

	}
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	video::TextureUnit tu;
	//getDevice()->setRenderTarget(m_rt);
	getDevice()->set2DMode();

}

void Application::update(float dt)
{
	CMRayApplication::update(dt);
}

void Application::onDone()
{
}


void Application::onRenderDone(scene::ViewPort*vp)
{
}

}

