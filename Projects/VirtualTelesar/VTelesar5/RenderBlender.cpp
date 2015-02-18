
#include "StdAfx.h"
#include "RenderBlender.h"


namespace mray
{
namespace VT
{


RenderBlender::RenderBlender()
{
	m_time=0;
	m_speed=1;
	m_staticBlending=false;
}
RenderBlender::~RenderBlender()
{
}


void RenderBlender::Reset()
{
	m_time=0;
}
void RenderBlender::SetSpeed(float s)
{
	m_speed=s;
}

void RenderBlender::BlendImages(video::ITextureCRef t1,video::ITextureCRef t2,const math::rectf& rc)
{
	video::TextureUnit tu1,tu2;
	tu1.SetTexture(t1);
	tu2.SetTexture(t2);
	video::IVideoDevice* dev= Engine::getInstance().getDevice();

	math::rectf tc(0,0,1,1);

	float alpha[2]={1-m_time,m_time};
	if(m_staticBlending)
	{
		alpha[0]=m_alpha[0];
		alpha[1]=m_alpha[1];
	}
	if(!t1.isNull() && !t2.isNull())
	{
		if(t1)
		{
			dev->useTexture(0,&tu1);
			dev->draw2DImage(rc,video::SColor(1,1,1,alpha[0]),0,&tc);
		}
		if(t2 )
		{
			dev->useTexture(0,&tu2);
			dev->draw2DImage(rc,video::SColor(1,1,1,alpha[1]),0,&tc);
		}
	}else
	{
		if(t2)
		{
			dev->useTexture(0,&tu2);
			dev->draw2DImage(rc,video::SColor(1,1,1,1),0,&tc);
		}
	}
}

void RenderBlender::Update(float dt)
{
	m_time=math::Min<float>(m_time+m_speed*dt,1.0f);
}


}
}

