
#include "stdafx.h"
#include "VideoWindow.h"

#include "GstNetworkVideoPlayer.h"
#include "TextureRTWrap.h"
#include "StreamReader.h"
#include "IMonitorDeviceManager.h"
#include "IMonitorDevice.h"
#include "ViewPort.h"
#include "RenderWindow.h"
#include "RenderWindowUtils.h"
#include <mrayOIS.h>


namespace mray
{
namespace TBee
{


VideoWindow::VideoWindow()
{

	m_VideoWnd = 0;
	m_VideoMonitor = -1;
}
VideoWindow::~VideoWindow()
{
	OnClose();
}

void VideoWindow::Parse(const OptionContainer& extraOptions)
{
	if(extraOptions.GetOptionByName("VideoDisplay"))
		m_VideoMonitor = core::StringConverter::toInt(extraOptions.GetOptionByName("VideoDisplay")->getValue());
	else m_VideoMonitor = 0;
}

bool VideoWindow::OnInit(TBeeServiceContext* context)
{
	if (m_VideoMonitor == -1)
		return false;


	{
		video::GstNetworkVideoPlayer* player;
		m_player=player = new video::GstNetworkVideoPlayer();
#if USE_PLAYERS
//		app->GetPlayers()->AddPlayer(player, "Video");
#endif

		m_VideoGrabber = new video::VideoGrabberTexture();
		m_VideoGrabber->Set(new video::GstNetworkVideoPlayerGrabber(player), 0);
	}
	{
		OS::IStreamPtr s = gFileSystem.openFile(gFileSystem.getAppPath() + "ProjectionSettings.txt", OS::TXT_READ);
		if (!s.isNull())
		{
			math::rectf bb;
			math::vector2d size;
			OS::StreamReader rdr(s);
			core::StringConverter::parse(rdr.readLine(), bb);
			core::StringConverter::parse(rdr.readLine(), size);

			m_projectionRect = bb;
			printf("Projection Box: %s\n", core::StringConverter::toString(bb).c_str());

// 			m_projectionRect.ULPoint = bb.ULPoint / size;
// 			m_projectionRect.BRPoint = bb.BRPoint / size;
			s->close();
		}
		else
		{
			m_projectionRect = math::rectf(0, 0, 1, 1);
		}
	}
	{
		OptionContainer opt;
		opt["title"].value = "Video Window";
		opt["VSync"].value = "false";
		opt["top"].value = "0";
		opt["left"].value = "0";
		opt["border"].value = "none";

		video::IMonitorDevice* monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(m_VideoMonitor);
		if (!monitor)
		{
			m_VideoMonitor = 0;
			monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(m_VideoMonitor);
		}
		opt["Monitor"].value = core::StringConverter::toString(m_VideoMonitor);

		m_VideoWnd = gEngine.getDevice()->CreateRenderWindow("Video Window", monitor->GetSize(), false, opt, 0);
		m_VideoViewPort = m_VideoWnd->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
		//context->app->AddRenderWindow(m_VideoWnd);
		//force the window to remain on top
		{
			HWND hWnd;
			m_VideoWnd->GetCustomParam("WINDOW", &hWnd);
			if (false)
			{
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

				InputCreationPack pack(m_VideoWnd);
				pack.WinSize = monitor->GetSize();
				pack.exclusiveMouse = false;
				pack.createJoystic = true;

				m_inputManager = CreateOISInputManager(pack);
			}
		}
		m_VideoViewPort->AddListener(this);
		video::RenderWindowUtils::AddListener(m_VideoWnd, this);

		{
			m_I420ToRGB = new video::ParsedShaderPP(Engine::getInstance().getDevice());
			m_I420ToRGB->LoadXML(gFileSystem.openFile("I420ToRGB.peff"));

		}
	}
	return true;
}
void VideoWindow::OnClose()
{
	if (!IsActive())
		return;
	if (!m_VideoWnd)
		return;
	m_VideoWnd->Destroy();
	delete m_VideoWnd;
	m_VideoWnd = 0;

	delete m_VideoGrabber;
	m_VideoGrabber = 0;

	m_player->Close();
	delete m_player;
	m_player = 0;

	m_I420ToRGB = 0;


}
void VideoWindow::OnEnable()
{
	if (!IsActive())
		return;
	printf("Video window enabled\n");
	m_player->CreateStream();
	m_player->Play();
}
void VideoWindow::OnDisable()
{
	if (!IsActive())
		return;
	m_player->Stop();
}

void VideoWindow::OnConnected(const core::string &ipaddr, int VideoPort, bool rtcp)
{
	if (!IsActive())
		return;
	m_player->SetIPAddress(ipaddr, VideoPort, 0, rtcp);
}

void VideoWindow::OnUpdate(float dt)
{/*
	m_inputManager->capture();
	
	controllers::IKeyboardController* kb= m_inputManager->getKeyboard();
	
	if (!kb->isRCtrlPress())
	{
		m_projectionRect.ULPoint.y += (kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN))*dt*0.01f;
		m_projectionRect.ULPoint.x += (kb->getKeyState(KEY_RIGHT) - kb->getKeyState(KEY_LEFT))*dt*0.01f;
	}
	else
	{
		m_projectionRect.BRPoint.y += (kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN))*dt*0.01f;
		m_projectionRect.BRPoint.x += (kb->getKeyState(KEY_RIGHT) - kb->getKeyState(KEY_LEFT))*dt*0.01f;
	}*/

}

void VideoWindow::WindowPostRender(video::RenderWindow* wnd)
{

}
void VideoWindow::onRenderDone(scene::ViewPort*vp)
{
	if (!IsActive())
		return;
	vp = m_VideoViewPort;
	gEngine.getDevice()->set2DMode();
	video::TextureUnit tex;

	m_VideoGrabber->Blit();
	tex.SetTexture(m_VideoGrabber->GetTexture());

	math::vector2d txsz;
	txsz.x = m_VideoGrabber->GetTexture()->getSize().x;
	txsz.y = m_VideoGrabber->GetTexture()->getSize().y/1.5f;
	{
		m_I420ToRGB->Setup(math::rectf(0, txsz));
		m_I420ToRGB->render(&video::TextureRTWrap(tex.GetTexture()));
		tex.SetTexture(m_I420ToRGB->getOutput()->GetColorTexture());

	}

	tex.setTextureClamp(video::ETW_WrapR, video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP_TO_BORDER);

	gEngine.getDevice()->useTexture(0, &tex);

	math::rectf tc(0, 0, 1, 1);

	float r1 = (float)vp->GetSize().y / (float)vp->GetSize().x;
	float r2 = (float)txsz.y / (float)txsz.x;

	float r = r2 / r1; //calculate the ratio between texture and viewport
	
	//tc.ULPoint.y = -r/2;
	//tc.BRPoint.y = r ;
	gEngine.getDevice()->draw2DImage(math::rectf(0, vp->GetSize()), 1, 0,&tc);

	/*	*/
}

}
}

