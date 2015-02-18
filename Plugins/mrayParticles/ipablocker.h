

#ifndef ___IPABlocker___
#define ___IPABlocker___

#include <IParticleAffector.h>
#include <IParticleAffectorFactory.h>

#include <Plane.h>
#include <box3d.h>
#include "compileConfig.h"



namespace mray{
namespace scene{

enum EPABlockType{
	BT_Kill,
	BT_Bounce
};

class IParticle;

class MRAY_PS_DLL IPABlocker:public IParticleAffector
{
protected:
	EPABlockType m_blockType;
	float m_bounce;

public:

	IPABlocker();
	virtual~IPABlocker();

	virtual bool affectsTransformations(){return true;}

	void setBlockType(EPABlockType type);
	EPABlockType getBlockType();

	void setBounce(float b);
	float getBounce();

};

////////////////////////////
class MRAY_PS_DLL PAPlaneBlocker:public IPABlocker
{
protected:
	math::Plane m_plane;
public:

	PAPlaneBlocker();
	PAPlaneBlocker(const math::Plane &p);
	virtual~PAPlaneBlocker();

	virtual void applyOnParticle(IParticle*p,float dt);

	void setPlane(const math::Plane &p);
	math::Plane getPlane();
};

class MRAY_PS_DLL PlaneBlockerFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();

};

////////////////////////////
class MRAY_PS_DLL PABoxBlocker:public IPABlocker
{
protected:

	math::box3d m_box;
	math::vector3d m_center;
public:

	PABoxBlocker();
	PABoxBlocker(const math::box3d &box);
	virtual~PABoxBlocker();

	virtual void applyOnParticle(IParticle*p,float dt);

	void setBox(const math::box3d &box);
	math::box3d getBox();
};

class MRAY_PS_DLL BoxBlockerFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();
};

////////////////////////////
class MRAY_PS_DLL PASphereBlocker:public IPABlocker
{
protected:

	math::vector3d m_center;
	float m_radius;
	float m_sqrRadius;
public:

	PASphereBlocker();
	PASphereBlocker(const math::vector3d& c,float r);
	virtual~PASphereBlocker();

	virtual void applyOnParticle(IParticle*p,float dt);

	void setSphere(const math::vector3d& c,float r);
	float getRadius();
	math::vector3d getCenter();
};

class MRAY_PS_DLL SphereBlockerFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);


	virtual IParticleAffector*createAffector();
};



}
}







#endif


