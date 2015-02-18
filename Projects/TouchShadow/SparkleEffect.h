

/********************************************************************
	created:	2013/09/16
	created:	16:9:2013   21:58
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\SparkleEffect.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	SparkleEffect
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SparkleEffect__
#define __SparkleEffect__


#include "EffectManager.h"

namespace mray
{

class SparkleEffect:public IEffect
{
protected:
	class Particle
	{
	public: 
		math::vector2d pos;
		math::vector2d scale;
		float angle;
		math::vector2d velocity;
		float alpha;
		float rand;
		Particle()
		{
			Reset();
		}
		void Reset()
		{
			alpha=1;
			rand=math::Randomizer::rand01()+0.5;
			velocity.x=math::Randomizer::rand01()-0.5;
			velocity.y=math::Randomizer::rand01()-0.5;

			velocity*=100*rand;
			angle=math::Randomizer::rand01()*math::PI32;

		}
		void Update(float dt)
		{
			float drag=0.9;
			scale+=dt*rand;
			pos+=velocity*dt;
			velocity-=velocity*drag*dt;
			alpha-=dt;
			angle+=dt*math::PI32*0.3;
		}

	};
protected:
	std::vector<Particle> m_particles;
	video::ITexturePtr m_tex;
	float m_time;
	float m_span;
	float m_scale;
	float m_rate;
	math::vector2d m_pos;
	float m_sparked;
	float m_acc;
	video::SColor m_color;
	float m_stength;
	bool m_done;
public:
	SparkleEffect(EffectCreatePack* desc);

	virtual~SparkleEffect();
	void Draw(const math::rectf& rc);

	void Update(float dt);

	bool IsDone();

};

class  SparkleEffectFactory:public IEffectFactory
{
public:
	virtual core::string GetType()const
	{
		return "Sparkle";
	}

	virtual IEffect* CreateEffect(EffectCreatePack* desc)
	{
		return new SparkleEffect(desc);
	}
};

}


#endif
