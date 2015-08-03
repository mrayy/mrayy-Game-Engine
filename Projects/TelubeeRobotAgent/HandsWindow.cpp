
#include "stdafx.h"
#include "HandsWindow.h"

#include "GstNetworkVideoPlayer.h"
#include "TRApplication.h"
#include "TextureRTWrap.h"
#include "StreamReader.h"
#include "IMonitorDeviceManager.h"
#include "IMonitorDevice.h"


namespace mray
{
namespace TBee
{


HandsWindow::HandsWindow()
{

	m_handsWnd = 0;
	m_handsMonitor = -1;
}
HandsWindow::~HandsWindow()
{
	OnClose();
}

void HandsWindow::Parse(const OptionContainer& extraOptions)
{
	m_handsMonitor = core::StringConverter::toInt(extraOptions.GetOptionByName("HandsDisplay")->getValue());
}

bool HandsWindow::OnInit(TBeeServiceContext* context)
{
	if (m_handsMonitor == -1)
		return false;

	m_I420ToRGB = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_I420ToRGB->LoadXML(gFileSystem.openFile("I420ToRGB.peff"));


	{
		video::GstNetworkVideoPlayer* player;
		m_player=player = new video::GstNetworkVideoPlayer();
#if USE_PLAYERS
//		app->GetPlayers()->AddPlayer(player, "Hands");
#endif

		m_handsGrabber = new video::VideoGrabberTexture();
		m_handsGrabber->Set(new video::GstNetworkVideoPlayerGrabber(player), 0);
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
		opt["title"].value = "Hands Window";
		opt["VSync"].value = "false";
		opt["top"].value = "0";
		opt["left"].value = "0";
		opt["border"].value = "none";

		video::IMonitorDevice* monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(m_handsMonitor);
		if (!monitor)
		{
			m_handsMonitor = 0;
			monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(m_handsMonitor);
		}
		opt["Monitor"].value = core::StringConverter::toString(m_handsMonitor);

		m_handsWnd = gEngine.getDevice()->CreateRenderWindow("Hands Window", monitor->GetSize(), false, opt, 0);
		m_handsViewPort = m_handsWnd->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
		context->app->AddRenderWindow(m_handsWnd);
		m_handsViewPort->AddListener(this);


		video::ParsedShaderPP* pp = new video::ParsedShaderPP(gEngine.getDevice());
		pp->LoadXML(gFileSystem.openFile("ProjectionCorrect.peff"));
		m_undistortShader = pp;
	}
	return true;
}
void HandsWindow::OnClose()
{
	if (!IsActive())
		return;
	if (!m_handsWnd)
		return;
	m_handsWnd->Destroy();
	delete m_handsWnd;
	m_handsWnd = 0;

	delete m_handsGrabber;
	m_handsGrabber = 0;

	m_player->Close();
	delete m_player;
	m_player = 0;

	m_undistortShader = 0;
	m_I420ToRGB = 0;


}
void HandsWindow::OnEnable()
{
	if (!IsActive())
		return;
	printf("Hands window enabled\n");
	m_player->CreateStream();
	m_player->Play();
}
void HandsWindow::OnDisable()
{
	if (!IsActive())
		return;
	m_player->Stop();
}

void HandsWindow::OnConnected(const core::string &ipaddr, int handsPort, bool rtcp)
{
	if (!IsActive())
		return;
	m_player->SetIPAddress(ipaddr, handsPort, 0, rtcp);
}

void HandsWindow::OnUpdate(float dt)
{

}

void HandsWindow::onRenderDone(scene::ViewPort*vp)
{
	if (!IsActive())
		return;
	gEngine.getDevice()->set2DMode();
	video::TextureUnit tex;

	m_handsGrabber->Blit();
	tex.SetTexture(m_handsGrabber->GetTexture());

	math::vector2d txsz;
	txsz.x = m_handsGrabber->GetTexture()->getSize().x;
	txsz.y = m_handsGrabber->GetTexture()->getSize().y;
	{
		m_I420ToRGB->Setup(math::rectf(0, txsz));
		m_I420ToRGB->render(&video::TextureRTWrap(tex.GetTexture()));
		tex.SetTexture(m_I420ToRGB->getOutput()->GetColorTexture());

	}


	float r = (float)vp->GetSize().y / (float)vp->GetSize().x;
	float w = txsz.x*r;
	float c = txsz.x - w;


	gEngine.getDevice()->useTexture(0, &tex);
	m_undistortShader->Setup(math::rectf(0, txsz));
	m_undistortShader->render(&video::TextureRTWrap(m_handsGrabber->GetTexture()));
	tex.SetTexture(m_undistortShader->getOutput()->GetColorTexture());
	gEngine.getDevice()->unuseShader();

	gEngine.getDevice()->useTexture(0, &tex);/**/
	math::rectf texCoords(0, 1, 1, 0);

	texCoords = m_projectionRect;
	texCoords.ULPoint.y = 1 - texCoords.ULPoint.y;
	texCoords.BRPoint.y = 1 - texCoords.BRPoint.y;
	gEngine.getDevice()->draw2DImage(math::rectf(0, vp->GetSize()), 1, 0, &texCoords);
	/*	*/
}

}
}

