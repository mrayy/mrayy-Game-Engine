


#include "stdafx.h"
#include "CameraRenderingState.h"



#include "Application.h"
#include "VTAppGlobals.h"
#include <IThreadManager.h>
#include "AppData.h"

namespace mray
{
namespace VT
{

	//typedef video::DirectShowVideoGrabber VCameraType ;


CameraRenderingState::CameraRenderingState(const core::string&name)
:IRenderingState(name)
{
	m_exitCode=0;

	m_texCoordRectSize=1;
	m_VerticalShift=0;

	m_cams[0]=new video::CameraTextureSource();
	m_cams[1]=new video::CameraTextureSource();

	m_cams[1]->SetCameraInfo(0,1024,720,30);
	m_cams[1]->SetCameraInfo(1,1024,720,30);
}


CameraRenderingState::~CameraRenderingState()
{
	delete m_cams[0];
	delete m_cams[1];
}



void CameraRenderingState::InitState()
{
	IRenderingState::InitState();
	m_texCoordRectSize=1;

}



void CameraRenderingState::OnEvent(Event* e)
{
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* ev=(KeyboardEvent*)e;
		if(ev->press && ev->key==KEY_UP)
		{
			m_VerticalShift+=1;
		}else if(ev->press && ev->key==KEY_DOWN)
		{
			m_VerticalShift-=1;
		}
		if(ev->press && ev->key==KEY_LEFT)
		{
			m_texCoordRectSize+=0.01;
			//m_texCoordRectSize=math::Min(m_texCoordRectSize,1.0f);
		}else if(ev->press && ev->key==KEY_RIGHT)
		{
			m_texCoordRectSize-=0.01;
			m_texCoordRectSize=math::Max(m_texCoordRectSize,0.5f);
		}
	}
}


void CameraRenderingState::OnEnter(IRenderingState*prev)
{
	int c = 1;
	if (TBee::AppData::Instance()->stereoMode != TBee::ERenderStereoMode::None)
		c = 2;
	for(int i=0;i<c;++i)
	{
		m_cams[i]->Load();
	}
}


void CameraRenderingState::OnExit()
{
	IRenderingState::OnExit();
	int c = 1;
	if (TBee::AppData::Instance()->stereoMode != TBee::ERenderStereoMode::None)
		c = 2;
	for(int i=0;i<c;++i)
	{
		m_cams[i]->Unload();
	}
}
video::IRenderTarget* CameraRenderingState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	video::IRenderTarget* rt=IRenderingState::Render(rc,eye);
	int index=eye==TBee::Eye_Left?0:1;
	float shift=(float)m_VerticalShift*0.5f;

	math::rectf texCoord(1-m_texCoordRectSize,m_texCoordRectSize,m_texCoordRectSize,1-m_texCoordRectSize);

// 	if(!m_stereo)
// 		index=0;
	if(index==1)
		shift=-shift;


	video::IVideoDevice* dev=Engine::getInstance().getDevice();
	dev->setRenderTarget(rt);

	m_cams[index]->Update(0);

	video::TextureUnit tex;
	tex.SetEdgeColor(video::SColor(0,0,0,1));
	tex.setTextureClamp(video::ETW_WrapR,video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapS,video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapT,video::ETC_CLAMP_TO_BORDER);
	tex.SetTexture(m_cams[index]->GetTexture());
	dev->useTexture(0,&tex);
	dev->draw2DImage(math::rectf(math::vector2d(0,shift),rc.getSize()+math::vector2d(0,shift)),1,0,&texCoord);

	dev->setRenderTarget(0);
	dev->useTexture(0,0);


	return rt;
}

void CameraRenderingState::Update(float dt)
{
}




void CameraRenderingState::LoadFromXML(xml::XMLElement* e)
{
	IRenderingState::LoadFromXML(e);

	xml::XMLElement* elem=e->getSubElement("LeftCamera");
	if(elem)
		m_cams[0]->LoadFromXML(elem);
	elem=e->getSubElement("RightCamera");
	if(elem)
		m_cams[1]->LoadFromXML(elem);
}


}
}


