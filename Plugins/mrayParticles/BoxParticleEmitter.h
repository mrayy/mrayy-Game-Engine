

#ifndef ___BoxParticleEmitter___
#define ___BoxParticleEmitter___

#include <ICustomParticleEmitter.h>
#include <IParticleEmitterFactory.h>
#include <TypedProperty.h>


#include "compileConfig.h"
namespace mray{
namespace scene{


class MRAY_PS_DLL BoxParticleEmitter:public ICustomParticleEmitter
{
	DECLARE_RTTI;
protected:
	math::vector3d m_size;

	
public:
//	rwProperty<BoxParticleEmitter,math::vector3d> Size;

	virtual void setupParticle(IParticle*particle);
	DECLARE_PROPERTY_TYPE(Size,math::vector3d,MRAY_PS_DLL);

	const math::vector3d& getSize() { return m_size; }
	bool setSize(const math::vector3d &val) { m_size = val; return true;}

	BoxParticleEmitter();
};


class MRAY_PS_DLL SBoxPEFactory:public IParticleEmitterFactory
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



