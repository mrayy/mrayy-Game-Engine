

#include "stdafx.h"
#include "GStreamerTestState.h"
#include "Application.h"


namespace mray
{
namespace TBee
{

GStreamerTestState::GStreamerTestState()
:IRenderingState("Video")
{
	m_providers = new GStreamVideoProvider();

	//m_providers->LoadMovie("http://docs.gstreamer.com/media/sintel_trailer-480p.webm");
	 //m_providers->ConnectToCameras("192.168.10.46",5000,5002,5001);
}


GStreamerTestState::~GStreamerTestState()
{
	delete m_providers;
}



void GStreamerTestState::InitState()
{
	IRenderingState::InitState();
	m_remoteTex = Engine::getInstance().getDevice()->createEmptyTexture2D(true);
}




void GStreamerTestState::OnEvent(Event* e)
{
}


void GStreamerTestState::OnEnter(IRenderingState*prev)
{
}


void GStreamerTestState::OnExit()
{
}


void GStreamerTestState::Update(float dt)
{
	m_providers->Update(dt);
	if (m_providers->HasNewImage(0))
	{
		const video::ImageInfo* image = m_providers->GetImage(0);
		m_remoteTex->createTexture(math::vector3d(image->Size.x, image->Size.y, 1), image->format);

		video::LockedPixelBox box(math::box3d(0, image->Size), image->format, image->imageData);
		m_remoteTex->getSurface(0)->blitFromMemory(box);
	}
}


video::IRenderTarget* GStreamerTestState::Render(const math::rectf& rc, ETargetEye eye)
{
	IRenderingState::Render(rc, eye);
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	dev->setRenderTarget(m_renderTarget[GetEyeIndex(eye)]);
	math::rectf trc;
	video::TextureUnit tex;
	tex.SetTexture(m_remoteTex);
	if (eye == Eye_Left)
	{
		trc = m_providers->GetTexRect(0);
	}
	else
	{
		trc = m_providers->GetTexRect(1);
	}
	Engine::getInstance().getDevice()->useTexture(0, &tex);;
	Engine::getInstance().getDevice()->draw2DImage(rc, 1, 0, &trc);
	return m_renderTarget[GetEyeIndex(eye)];
}



void GStreamerTestState::LoadFromXML(xml::XMLElement* e)
{
}


}
}

