

#include "stdafx.h"
#include "PhysXForceField.h"
#include "CPhysXNode.h"
#include "PhysXForceFieldLinearKernal.h"
#include "PhysXForceFieldShapeGroup.h"
#include "PhysXObjectConverter.h"
#include "PhysXManager.h"



namespace mray
{
namespace physics
{

PhysXForceField::PhysXForceField(NxForceField* f,PhysXManager* creator)
{
	m_creator=creator;
	m_forceField=f;
	m_includeShapeGroup=new PhysXForceFieldShapeGroup(&f->getIncludeShapeGroup());
	m_name=f->getName();
}
PhysXForceField::~PhysXForceField()
{
	delete m_includeShapeGroup;
}
IPhysicManager* PhysXForceField::getCreator()const
{
	return m_creator;
}


void PhysXForceField::setPose(const math::matrix4x4 &m)
{
	NxMat34 pos;
	pos.setRowMajor44(m.mat);
	m_forceField->setPose(pos);
}
math::matrix4x4 PhysXForceField::getPose()const
{
	math::matrix4x4 m;
	m_forceField->getPose().getRowMajor44(m.mat);
	return m;
}

void PhysXForceField::setNode(IPhysicalNode* node)
{
	if(node)
		m_forceField->setActor(((CPhysXNode*)node)->getActor());
	else m_forceField->setActor(0);
}
IPhysicalNode* PhysXForceField::getNode()const
{
	NxActor* a= m_forceField->getActor();
	if(!a)
		return 0;
	return (IPhysicalNode*)a->userData;
}

void PhysXForceField::setKernal(IPhysicalForceFieldLinearKernal* k)
{
	if(k)
		m_forceField->setForceFieldKernel(((PhysXForceFieldLinearKernal*)k)->getNxKernal());
	else m_forceField->setForceFieldKernel(0);
}
IPhysicalForceFieldLinearKernal* PhysXForceField::getKernal()const
{
	NxForceFieldKernel* k=m_forceField->getForceFieldKernel();
	if(!k)
		return 0;
	return (IPhysicalForceFieldLinearKernal*)k->userData;
}

IPhysicalForceFieldShapeGroup* PhysXForceField::getIncludeShapeGroup()
{
	return m_includeShapeGroup;
}

void PhysXForceField::addShapeGroup(IPhysicalForceFieldShapeGroup* g)
{
	if(!g)
		return;
	m_groups.push_back(g);
	m_forceField->addShapeGroup(*((PhysXForceFieldShapeGroup*)g)->getNxShapeGroup());
}
void PhysXForceField::removeShapeGroup(IPhysicalForceFieldShapeGroup* g)
{

	std::list<IPhysicalForceFieldShapeGroup*>::iterator it=m_groups.begin();
	for(;it!=m_groups.end();++it)
	{
		if((*it)==g)
		{
			m_groups.erase(it);
			break;
		}
	}
	m_forceField->removeShapeGroup(*((PhysXForceFieldShapeGroup*)g)->getNxShapeGroup());
}
const std::list<IPhysicalForceFieldShapeGroup*> PhysXForceField::getShapeGroups()const
{
	return m_groups;
}

void PhysXForceField::setGroupID(GroupID g)
{
	m_forceField->setGroup(g);
}
GroupID PhysXForceField::getGroupID()const
{
	return m_forceField->getGroup();
}

void PhysXForceField::setGroupMask(uint *bits)
{
	NxGroupsMask g;
	g.bits0=bits[0];
	g.bits1=bits[1];
	g.bits2=bits[2];
	g.bits3=bits[3];
	m_forceField->setGroupsMask(g);
}
void PhysXForceField::getGroupMask(uint *bits)const
{
	NxGroupsMask g;
	g=m_forceField->getGroupsMask();
	bits[0]=g.bits0;
	bits[1]=g.bits1;
	bits[2]=g.bits2;
	bits[3]=g.bits3;
}

void PhysXForceField::setCoordinates(EForceFieldCoordinates c)
{
	m_forceField->setCoordinates(PhysXObjectConverter::convertForceFieldCoordinates(c));
}
EForceFieldCoordinates PhysXForceField::getCoordinates()const
{
	return PhysXObjectConverter::convertForceFieldCoordinates(m_forceField->getCoordinates());
}

void PhysXForceField::setName(const core::string& name)
{
	m_name=name;
	m_forceField->setName(string_to_char(name).c_str());
}
const core::string& PhysXForceField::getName()
{
	return m_name;
}

void PhysXForceField::setFluidType(EForceFieldType t)
{
	m_forceField->setFluidType(PhysXObjectConverter::convertForceFieldType(t));
}
EForceFieldType PhysXForceField::getFluidType()const
{
	return PhysXObjectConverter::convertForceFieldType(m_forceField->getFluidType());
}

void PhysXForceField::setClothType(EForceFieldType t)
{
	m_forceField->setClothType(PhysXObjectConverter::convertForceFieldType(t));
}
EForceFieldType PhysXForceField::getClothType()const
{
	return PhysXObjectConverter::convertForceFieldType(m_forceField->getClothType());
}

void PhysXForceField::setSoftBodyType(EForceFieldType t)
{
	m_forceField->setSoftBodyType(PhysXObjectConverter::convertForceFieldType(t));
}
EForceFieldType PhysXForceField::getSoftBodyType()const
{
	return PhysXObjectConverter::convertForceFieldType(m_forceField->getSoftBodyType());
}

void PhysXForceField::setRigidBodyType(EForceFieldType t)
{
	m_forceField->setRigidBodyType(PhysXObjectConverter::convertForceFieldType(t));
}
EForceFieldType PhysXForceField::getRigidBodyType()const
{
	return PhysXObjectConverter::convertForceFieldType(m_forceField->getRigidBodyType());
}

void PhysXForceField::setFlag(EForceFieldFlags f,bool s)
{
	NxU32 flags= m_forceField->getFlags();
	NxForceFieldFlags ff;
	switch(f)
	{
	case EFFF_VolumetricScalingFluid:ff=NX_FFF_VOLUMETRIC_SCALING_FLUID;break;
	case EFFF_VolumetricScalingCloth:ff=NX_FFF_VOLUMETRIC_SCALING_CLOTH;break;
	case EFFF_VolumetricScalingRigidBody:ff=NX_FFF_VOLUMETRIC_SCALING_RIGIDBODY;break;
	case EFFF_VolumetricScalingSoftBody:ff=NX_FFF_VOLUMETRIC_SCALING_SOFTBODY;break;
	default:
		return;
	}
	if(s)
		flags|=ff;
	else flags&=~ff;
}
bool PhysXForceField::getFlag(EForceFieldFlags f)
{
	NxU32 flags= m_forceField->getFlags();
	NxForceFieldFlags ff;
	switch(f)
	{
	case EFFF_VolumetricScalingFluid:ff=NX_FFF_VOLUMETRIC_SCALING_FLUID;break;
	case EFFF_VolumetricScalingCloth:ff=NX_FFF_VOLUMETRIC_SCALING_CLOTH;break;
	case EFFF_VolumetricScalingRigidBody:ff=NX_FFF_VOLUMETRIC_SCALING_RIGIDBODY;break;
	case EFFF_VolumetricScalingSoftBody:ff=NX_FFF_VOLUMETRIC_SCALING_SOFTBODY;break;
	default:
		return false;
	}
	return flags&ff;
}

ForceFieldVariety PhysXForceField::getVariety()const
{
	return m_forceField->getForceFieldVariety();
}
void PhysXForceField::setVariety(ForceFieldVariety v)
{
	return m_forceField->setForceFieldVariety(v);
}


}
}


