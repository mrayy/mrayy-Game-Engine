

#ifndef ___SphereParticleEmitter___
#define ___SphereParticleEmitter___

#include <ParticleEmitter.h>
#include <ICustomParticleEmitter.h>
#include <TypedProperty.h>

#include "compileConfig.h"

namespace mray{
namespace scene{


class MRAY_PS_DLL SphereParticleEmitter:public ICustomParticleEmitter
{
	DECLARE_RTTI
protected:
	math::vector3d m_innerRadius;
	math::vector3d m_outterRadius;


	virtual void setupParticle(IParticle*particle);
public:

	DECLARE_PROPERTY_TYPE(InnerRadius,math::vector3d,MRAY_PS_DLL);
	DECLARE_PROPERTY_TYPE(OutterRadius,math::vector3d,MRAY_PS_DLL);

/*	rwProperty<SphereParticleEmitter,math::vector3d> InnerRadius;
	rwProperty<SphereParticleEmitter,math::vector3d> OutterRadius;*/

	const math::vector3d& getInnerRadius() { return m_innerRadius; }
	bool setInnerRadius(const math::vector3d &val) ;

	const math::vector3d& getOutterRadius() { return m_outterRadius; }
	bool setOutterRadius(const math::vector3d &val) ;

	SphereParticleEmitter();

	

};


class MRAY_PS_DLL SSpherePSFactory:public IParticleEmitterFactory
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



