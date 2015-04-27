
#include "stdafx.h"
#include "HandsWindow.h"

#include "GstNetworkVideoPlayer.h"
#include "TRApplication.h"
#include "TextureRTWrap.h"


namespace mray
{


HandsWindow::HandsWindow()
{

	m_handsWnd = 0;
	m_handsMonitor = -1;
	m_app = 0;
}
HandsWindow::~HandsWindow()
{
	OnClose();
}

void HandsWindow::Parse(const OptionContainer& extraOptions)
{
	m_handsMonitor = core::StringConverter::toInt(extraOptions.GetOptionByName("HandsDisplay")->getValue());
}

bool HandsWindow::OnInit(TRApplication* app)
{
	m_app = app;
	if (m_handsMonitor == -1)
		return false;
	{
		video::GstNetworkVideoPlayer* player;
		player = new video::GstNetworkVideoPlayer();

		app->GetPlayers()->AddPlayer(player, "Hands");

		m_handsGrabber = new video::VideoGrabberTexture();
		m_handsGrabber->Set(new video::GstNetworkVideoPlayerGrabber(player), 0);
	}
	{
		{
			OptionContainer opt;
			opt["title"].value = "Hands Window";
			opt["VSync"].value = "false";
			opt["top"].value = "0";
			opt["left"].value = "0";
			//opt["border"].value = "none";
			opt["Monitor"].value = core::StringConverter::toString(m_handsMonitor);
			m_handsWnd = gEngine.getDevice()->CreateRenderWindow("Hands Window", app->GetRenderWindow(0)->GetSize(), false, opt, 0);
			m_handsViewPort = m_handsWnd->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
			app->AddRenderWindow(m_handsWnd);
			m_handsViewPort->AddListener(this);


			video::ParsedShaderPP* pp = new video::ParsedShaderPP(gEngine.getDevice());
			pp->LoadXML(gFileSystem.openFile("ProjectionCorrect.peff"));
			m_undistortShader = pp;
		}
	}
	return true;
}
void HandsWindow::OnClose()
{
	if (!m_handsWnd)
		return;

}
void HandsWindow::OnEnable()
{

}
void HandsWindow::OnDisable()
{

}

void HandsWindow::OnUpdate(float dt)
{

}

void HandsWindow::onRenderDone(scene::ViewPort*vp)
{
	gEngine.getDevice()->set2DMode();
	video::TextureUnit tex;

	m_handsGrabber->Blit();
	tex.SetTexture(m_handsGrabber->GetTexture());
	math::vector2d txsz;
	txsz.x = m_handsGrabber->GetTexture()->getSize().x;
	txsz.y = m_handsGrabber->GetTexture()->getSize().y;
	float r = (float)vp->GetSize().y / (float)vp->GetSize().x;
	float w = txsz.x*r;
	float c = txsz.x - w;


	gEngine.getDevice()->useTexture(0, &tex);
	m_undistortShader->Setup(math::rectf(0, txsz));
	m_undistortShader->render(&video::TextureRTWrap(m_handsGrabber->GetTexture()));
	tex.SetTexture(m_undistortShader->getOutput()->GetColorTexture());
	gEngine.getDevice()->unuseShader();

	gEngine.getDevice()->useTexture(0, &tex);/**/
	math::rectf texCoords(0, 0, 1, 1);
	gEngine.getDevice()->draw2DImage(math::rectf(0, vp->GetSize()), 1, 0, &texCoords);
	/*	*/
}

}

