
#include "stdafx.h"
#include "PhysXForceFieldLinearKernal.h"
#include "PhysXCommon.h"


namespace mray
{
namespace physics
{

PhysXForceFieldLinearKernal::PhysXForceFieldLinearKernal(NxForceFieldLinearKernel* k)
{
	m_kernal=k;
	m_name=k->getName();
}

math::vector3d PhysXForceFieldLinearKernal::getConstant()const
{
	NxVec3 v=m_kernal->getConstant();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setConstant(const math::vector3d& c)
{
	m_kernal->setConstant(ToNxVec3(c));
}

math::matrix3x3 PhysXForceFieldLinearKernal::getPositionMultiplier()const
{
	math::matrix3x3 ret;
	m_kernal->getPositionMultiplier().getRowMajor(ret.mat);
	return ret;
}
void PhysXForceFieldLinearKernal::setPositionMultiplier(const math::matrix3x3& c)
{
	NxMat33 ret;
	ret.setRowMajor(c.mat);
	m_kernal->setPositionMultiplier(ret);
}


math::matrix3x3 PhysXForceFieldLinearKernal::getVelocityMultiplier()const
{
	math::matrix3x3 ret;
	m_kernal->getVelocityMultiplier().getRowMajor(ret.mat);
	return ret;
}
void PhysXForceFieldLinearKernal::setVelocityMultiplier(const math::matrix3x3& c)
{
	NxMat33 ret;
	ret.setRowMajor(c.mat);
	m_kernal->setVelocityMultiplier(ret);
}


math::vector3d PhysXForceFieldLinearKernal::getPositionTarget()const
{
	NxVec3 v=m_kernal->getPositionTarget();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setPositionTarget(const math::vector3d& c)
{
	m_kernal->setPositionTarget(ToNxVec3(c));
}

math::vector3d PhysXForceFieldLinearKernal::getVelocityTarget()const
{
	NxVec3 v=m_kernal->getVelocityTarget();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setVelocityTarget(const math::vector3d& c)
{
	m_kernal->setVelocityTarget(ToNxVec3(c));
}

math::vector3d PhysXForceFieldLinearKernal::getFalloffLinear()const
{
	NxVec3 v=m_kernal->getFalloffLinear();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setFalloffLinear(const math::vector3d& c)
{
	m_kernal->setFalloffLinear(ToNxVec3(c));
}

math::vector3d PhysXForceFieldLinearKernal::getFalloffQuadratic()const
{
	NxVec3 v=m_kernal->getFalloffQuadratic();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setFalloffQuadratic(const math::vector3d& c)
{
	m_kernal->setFalloffQuadratic(ToNxVec3(c));
}

math::vector3d PhysXForceFieldLinearKernal::getNoise()const
{
	NxVec3 v=m_kernal->getNoise();
	return ToVec3(v);
}
void PhysXForceFieldLinearKernal::setNoise(const math::vector3d& c)
{
	m_kernal->setNoise(ToNxVec3(c));
}

float PhysXForceFieldLinearKernal::getTorusRadius()const
{
	return m_kernal->getTorusRadius();
}
void PhysXForceFieldLinearKernal::setTorusRadius(float c)
{
	m_kernal->setTorusRadius(c);
}

const core::string& PhysXForceFieldLinearKernal::getName()const
{
	return m_name;
}
void PhysXForceFieldLinearKernal::setName(const core::string& name)
{
	m_name=name;
	m_kernal->setName(string_to_char(name).c_str());
}

}
}


