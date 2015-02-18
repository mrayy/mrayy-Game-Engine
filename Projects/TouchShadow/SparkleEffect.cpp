

#include "stdafx.h"
#include "SparkleEffect.h"
#include "TextureResourceManager.h"

namespace mray
{

SparkleEffect::SparkleEffect(EffectCreatePack* desc)
{
	m_pos=desc->pos;
	m_rate=core::StringConverter::toFloat(desc->GetParam("Rate","30"));
	m_span=core::StringConverter::toFloat(desc->GetParam("Span","1"));
	m_scale=core::StringConverter::toFloat(desc->GetParam("Scale","20"));
	m_color=core::StringConverter::toColor(desc->GetParam("Color","0.4,0.6,0,1"));
	m_stength=core::StringConverter::toFloat(desc->GetParam("Strength","1"));
	core::string tex=desc->GetParam("Texture","Star.png");
	m_tex=gTextureResourceManager.loadTexture2D(tex);
	m_sparked=0;
	m_acc=0;

	m_done=false;
	m_rate*=m_stength;
}
SparkleEffect::~SparkleEffect()
{
}
void SparkleEffect::Draw(const math::rectf& vp)
{
	if(m_done)
		return;
	video::IVideoDevice* dev= Engine::getInstance().getDevice();
	video::TextureUnit tex;
	tex.SetTexture(m_tex);
	dev->useTexture(0,&tex);
	for(int i=0;i<m_particles.size();++i)
	{
		math::rectf rc;
		rc.ULPoint=m_pos+m_particles[i].pos-m_particles[i].scale*m_scale/2;
		rc.BRPoint=rc.ULPoint+m_particles[i].scale*m_scale;
		dev->draw2DImageRot(rc,m_particles[i].scale*m_scale/2,video::SColor(m_color.R,m_color.G,m_color.B,m_color.A*m_particles[i].alpha),m_particles[i].angle);
	}
}

void SparkleEffect::Update(float dt)
{
	if(m_done)
		return;
	m_sparked=0;
	m_done=true;
	for(int i=0;i<m_particles.size();++i)
	{
		if(m_particles[i].alpha>0)
		{
			m_particles[i].Update(dt);
			m_done=false;
		}
		else
		{
			if(m_sparked<m_rate*dt && m_span>0)
			{
				++m_sparked;
				m_particles[i].Reset();
				float r=(math::Randomizer::rand01()+0.1)*m_scale;
				float a=math::Randomizer::rand01()*math::PI32*2;
				m_particles[i].pos.set(cos(a)*r,sin(a)*r);
			}
		}
	}
	if(m_span>0)
	{
		int sparked=0;
		for(;m_sparked<m_rate*dt+m_acc;++m_sparked)
		{
			Particle p;
			m_done=false;
			float r=(math::Randomizer::rand01()+0.1)*m_scale;
			float a=math::Randomizer::rand01()*math::PI32*2;
			p.pos.set(cos(a)*r,sin(a)*r);
			m_particles.push_back(p);
			++sparked;
		}
		m_acc+=m_rate*dt-sparked;
		m_span-=dt;
	}
}

bool SparkleEffect::IsDone()
{
	return m_done;
}

}
