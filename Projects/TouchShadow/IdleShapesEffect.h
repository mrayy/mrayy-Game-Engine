

/********************************************************************
	created:	2013/09/16
	created:	16:9:2013   20:46
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\IdleShapesEffect.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	IdleShapesEffect
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IdleShapesEffect__
#define __IdleShapesEffect__

#include "EffectManager.h"


namespace mray
{

	class IdleShapeBase
	{
	protected:
	public:
		float m_sizeScaler;
		float m_alphaScaler;
		math::vector2d m_pos;
		math::vector2d m_velocity;
		float m_rand;
		float m_time;
		float m_lifespan;
		bool m_fill;
		video::SColor m_color;
		IdleShapeBase();
		virtual void Draw(const math::rectf& rc){}
		virtual void Update(float dt);
		virtual bool IsDone()
		{
			return m_time>=m_lifespan;
		}
	};
class IdleShapesEffect:public IEffect
{
protected:
	std::list<IdleShapeBase*> m_shapes;
	float m_time;
public:
	IdleShapesEffect(EffectCreatePack* desc);

	virtual~IdleShapesEffect();
	void Draw(const math::rectf& rc);

	void Update(float dt);

	bool IsDone();

};

class  IdleShapesEffectFactory:public IEffectFactory
{
public:
	virtual core::string GetType()const
	{
		return "IdleShapes";
	}

	virtual IEffect* CreateEffect(EffectCreatePack* desc)
	{
		return new IdleShapesEffect(desc);
	}
};

}


#endif
