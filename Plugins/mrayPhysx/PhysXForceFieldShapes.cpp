
#include "stdafx.h"
#include "PhysXForceFieldShapes.h"
#include "PhysXForceField.h"
#include "PhysXCommon.h"

#include <NxSphereForceFieldShape.h>

namespace mray
{
namespace physics
{

math::matrix4x4 PhysXForceFieldSphereShape::getTransformation()const
{
	math::matrix4x4 m;
	m_shape->getPose().getRowMajor44(m.mat);
	return m;
}
void PhysXForceFieldSphereShape::setTransformation(const math::matrix4x4& t)
{
	NxMat34 m;
	m.setRowMajor44(t.mat);
	m_shape->setPose(m);
}

IPhysicalForceField* PhysXForceFieldSphereShape::getOwnerForceField()const
{
	if(!m_shape->getForceField())
		return 0;
	return (PhysXForceField*)m_shape->getForceField()->userData;
}

void PhysXForceFieldSphereShape::setName(const core::string &name)
{
	m_name=name;
	m_shape->setName((char*)m_name.c_str());
}

void PhysXForceFieldSphereShape::setRadius(float r)
{
	m_shape->setRadius(r);
}
float PhysXForceFieldSphereShape::getRadius()const
{
	return m_shape->getRadius();
}

//////////////////////////////////////////////////////////////////////////

math::matrix4x4 PhysXForceFieldCapsuleShape::getTransformation()const
{
	math::matrix4x4 m;
	m_shape->getPose().getRowMajor44(m.mat);
	return m;
}
void PhysXForceFieldCapsuleShape::setTransformation(const math::matrix4x4& t)
{
	NxMat34 m;
	m.setRowMajor44(t.mat);
	m_shape->setPose(m);
}

IPhysicalForceField* PhysXForceFieldCapsuleShape::getOwnerForceField()const
{
	if(!m_shape->getForceField())
		return 0;
	return (PhysXForceField*)m_shape->getForceField()->userData;
}

void PhysXForceFieldCapsuleShape::setName(const core::string &name)
{
	m_name=name;
	m_shape->setName((char*)m_name.c_str());
}

void PhysXForceFieldCapsuleShape::setRadius(float r)
{
	m_shape->setRadius(r);
}
float PhysXForceFieldCapsuleShape::getRadius()const
{
	return m_shape->getRadius();
}

void PhysXForceFieldCapsuleShape::setHeight(float r)
{
	m_shape->setHeight(r);
}
float PhysXForceFieldCapsuleShape::getHeight()const
{
	return m_shape->getHeight();
}

//////////////////////////////////////////////////////////////////////////

math::matrix4x4 PhysXForceFieldBoxShape::getTransformation()const
{
	math::matrix4x4 m;
	m_shape->getPose().getRowMajor44(m.mat);
	return m;
}
void PhysXForceFieldBoxShape::setTransformation(const math::matrix4x4& t)
{
	NxMat34 m;
	m.setRowMajor44(t.mat);
	m_shape->setPose(m);
}

IPhysicalForceField* PhysXForceFieldBoxShape::getOwnerForceField()const
{
	if(!m_shape->getForceField())
		return 0;
	return (PhysXForceField*)m_shape->getForceField()->userData;
}

void PhysXForceFieldBoxShape::setName(const core::string &name)
{
	m_name=name;
	m_shape->setName((char*)m_name.c_str());
}

void PhysXForceFieldBoxShape::setDimensions(const math::vector3d& r)
{
	m_shape->setDimensions(ToNxVec3(r));
}
math::vector3d PhysXForceFieldBoxShape::getDimensions()const
{
	NxVec3 v= m_shape->getDimensions();
	return ToVec3(v);
}

//////////////////////////////////////////////////////////////////////////

math::matrix4x4 PhysXForceFieldConvexShape::getTransformation()const
{
	math::matrix4x4 m;
	m_shape->getPose().getRowMajor44(m.mat);
	return m;
}
void PhysXForceFieldConvexShape::setTransformation(const math::matrix4x4& t)
{
	NxMat34 m;
	m.setRowMajor44(t.mat);
	m_shape->setPose(m);
}

IPhysicalForceField* PhysXForceFieldConvexShape::getOwnerForceField()const
{
	if(!m_shape->getForceField())
		return 0;
	return (PhysXForceField*)m_shape->getForceField()->userData;
}

void PhysXForceFieldConvexShape::setName(const core::string &name)
{
	m_name=name;
	m_shape->setName((char*)m_name.c_str());
}



}
}
