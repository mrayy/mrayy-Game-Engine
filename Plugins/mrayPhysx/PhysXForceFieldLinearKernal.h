

/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   13:59
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXForceFieldLinearKernal.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXForceFieldLinearKernal
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXForceFieldLinearKernal__
#define __PhysXForceFieldLinearKernal__

#include "PhysicalForceFieldDef.h"

namespace mray
{
namespace physics
{

class PhysXForceFieldLinearKernal:public IPhysicalForceFieldLinearKernal
{
private:
protected:
	NxForceFieldLinearKernel* m_kernal;
	core::string m_name;
public:
	PhysXForceFieldLinearKernal(NxForceFieldLinearKernel* k);
	virtual~PhysXForceFieldLinearKernal(){}

	NxForceFieldLinearKernel* getNxKernal(){return m_kernal;}

	math::vector3d getConstant()const;
	void setConstant(const math::vector3d& c);

	math::matrix3x3 getPositionMultiplier()const;
	void setPositionMultiplier(const math::matrix3x3& c);


	math::matrix3x3 getVelocityMultiplier()const;
	void setVelocityMultiplier(const math::matrix3x3& c);


	math::vector3d getPositionTarget()const;
	void setPositionTarget(const math::vector3d& c);

	math::vector3d getVelocityTarget()const;
	void setVelocityTarget(const math::vector3d& c);

	math::vector3d getFalloffLinear()const;
	void setFalloffLinear(const math::vector3d& c);

	math::vector3d getFalloffQuadratic()const;
	void setFalloffQuadratic(const math::vector3d& c);

	math::vector3d getNoise()const;
	void setNoise(const math::vector3d& c);

	float getTorusRadius()const;
	void setTorusRadius(float c);

	const core::string& getName()const;
	void setName(const core::string& name);
};

}
}

#endif

