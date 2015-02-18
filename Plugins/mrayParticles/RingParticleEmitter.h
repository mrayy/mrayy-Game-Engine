

#ifndef ___RingParticleEmitter___
#define ___RingParticleEmitter___

#include <ICustomParticleEmitter.h>
#include <IParticleEmitterFactory.h>
#include <TypedProperty.h>

#include "compileConfig.h"
namespace mray{
namespace scene{


class MRAY_PS_DLL RingParticleEmitter:public ICustomParticleEmitter
{
	DECLARE_RTTI
protected:
	math::vector2d m_innerRadius;
	math::vector2d m_outterRadius;
	float m_height;


	
	virtual void setupParticle(IParticle*particle);
public:
	DECLARE_PROPERTY_TYPE(InnerRadius,math::vector2d,MRAY_PS_DLL);
	DECLARE_PROPERTY_TYPE(OutterRadius,math::vector2d,MRAY_PS_DLL);
	DECLARE_PROPERTY_TYPE(Height,float,MRAY_PS_DLL);
/*	rwProperty<RingParticleEmitter,float> Height;
	rwProperty<RingParticleEmitter,math::vector2d> InnerRadius;
	rwProperty<RingParticleEmitter,math::vector2d> OutterRadius;*/

	const math::vector2d& getInnerRadius() { return m_innerRadius; }
	bool setInnerRadius(const math::vector2d &val);

	const math::vector2d& getOutterRadius() { return m_outterRadius; }
	bool setOutterRadius(const math::vector2d &val);


	float getHeight() { return m_height; }
	bool setHeight(const float &val) { m_height = val;return true; }

	RingParticleEmitter();

};


class MRAY_PS_DLL SRingPSFactory:public IParticleEmitterFactory
{
public:
	static const core::string Type;
	virtual const core::string& getType();
	virtual bool canCreate(const core::string&type);

	virtual ICustomParticleEmitter*createParticleEmitter();
};

}
}


#endif



