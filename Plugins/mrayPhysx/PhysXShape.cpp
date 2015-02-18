#include "stdafx.h"

#include "PhysXShape.h"
#include "physXCommon.h"
#include "NxBounds3.h"
#include "CPhysXMaterial.h"



namespace mray{
namespace physics{

PhysXShape::PhysXShape(NxShape*shape,IPhysicalNode* node)
{
	m_nxShape=shape;
	m_node=node;
	if(m_nxShape)
	{
		const char* n=m_nxShape->getName();
		core::char_to_string(n,m_name);
	}
}
PhysXShape::~PhysXShape()
{
}

NxShape* PhysXShape::getNxShape()
{
	return m_nxShape;
}
void  PhysXShape::setPhysicalNode(IPhysicalNode* node)
{
	m_node=node;
}
const core::string& PhysXShape::getName()
{
	return m_name;
}

IPhysicalNode* PhysXShape::getPhysicalNode()
{
	return m_node;
}

void PhysXShape::setCollisionGroup(ushort collisionGroup)
{
	m_nxShape->setGroup(collisionGroup);
}
ushort PhysXShape::getCollisionGroup()const
{
	return m_nxShape->getGroup();
}
void PhysXShape::getWorldBounds(math::box3d&bounds)const
{
	NxBounds3 b;
	m_nxShape->getWorldBounds(b);
	bounds.MinP.set(b.min.x,b.min.y,b.min.z);
	bounds.MaxP.set(b.max.x,b.max.y,b.max.z);
}

void PhysXShape::setLocalPos(const math::matrix4x4&m)
{
	NxMat34 nm;
	nm.setRowMajor44(m.getMatPointer());
	m_nxShape->setLocalPose(nm);
}
void PhysXShape::setLocalTranslation(const math::vector3d&v)
{
	m_nxShape->setLocalPosition(ToNxVec3(v));
}
void PhysXShape::setLocalOrintation(const math::quaternion&v)
{
	NxMat33 nm;
	NxQuat q;
	q.setWXYZ(v.w,v.x,v.y,v.z);
	nm.fromQuat(q);
	m_nxShape->setLocalOrientation(nm);
}

math::matrix4x4 PhysXShape::getLocalPos()const
{
	math::matrix4x4 m;
	NxMat34 nm=m_nxShape->getLocalPose();
	nm.getRowMajor44(m.pointer());
	return m;
}
math::vector3d PhysXShape::getLocalTranslation()const
{
	NxVec3 r=m_nxShape->getLocalPosition();
	return ToVec3(r);
}
math::quaternion PhysXShape::getLocalOrintation()const
{
	NxQuat qq;
	NxMat33 nm=m_nxShape->getLocalOrientation();
	nm.toQuat(qq);
	return math::quaternion(qq.w,qq.x,qq.y,qq.z);
}

void PhysXShape::setGlobalPos(const math::matrix4x4&m)
{
	NxMat34 nm;
	nm.setRowMajor44(m.getMatPointer());
	m_nxShape->setGlobalPose(nm);
}
void PhysXShape::setGlobalTranslation(const math::vector3d&v)
{
	m_nxShape->setGlobalPosition(ToNxVec3(v));
}
void PhysXShape::setGlobalOrintation(const math::quaternion&v)
{
	NxMat33 nm;
	NxQuat q;
	q.setWXYZ(v.w,v.x,v.y,v.z);
	nm.fromQuat(q);
	m_nxShape->setGlobalOrientation(nm);
}

math::matrix4x4 PhysXShape::getGlobalPos()const
{
	math::matrix4x4 m;
	NxMat34 nm=m_nxShape->getLocalPose();
	nm.getRowMajor44(m.pointer());
	return m;
}
math::vector3d PhysXShape::getGlobalTranslation()const
{
	NxVec3 r=m_nxShape->getGlobalPosition();
	return ToVec3(r);
}
math::quaternion PhysXShape::getGlobalOrintation()const
{
	NxQuat qq;
	NxMat33 nm=m_nxShape->getGlobalOrientation();
	nm.toQuat(qq);
	return math::quaternion(qq.w,qq.x,qq.y,qq.z);
}

void PhysXShape::setMaterial(IPhysicMaterial* mat)
{
	CPhysXMaterial*m=(CPhysXMaterial*)mat;
	if(!m)
		return;
	NxMaterialIndex i=m->getMaterialIndex();
}
IPhysicMaterial* PhysXShape::getMaterial()const
{
	return 0;
}

void PhysXShape::setSkinWidth(float w)
{
}
float PhysXShape::getSkinWidth()
{
	return 0;
}


}
}
