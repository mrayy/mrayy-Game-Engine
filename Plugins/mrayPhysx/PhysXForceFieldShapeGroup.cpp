
#include "stdafx.h"
#include "PhysXForceFieldShapeGroup.h"
#include "PhysXObjectConverter.h"
#include "PhysXForceFieldShapes.h"
#include "IPhysicalForceField.h"


namespace mray
{
namespace physics
{

PhysXForceFieldShapeGroup::PhysXForceFieldShapeGroup(NxForceFieldShapeGroup* g):m_group(g),m_userData(0)
{
	m_name=m_group->getName();
	m_group->resetShapesIterator();
	NxForceFieldShape* s;
	while((s=m_group->getNextShape())!=0)
	{
		IPhysicalForceFieldShape*ret=_CreateShape(s);
		if(ret)
			m_shapes.push_back(ret);
	}
}
PhysXForceFieldShapeGroup::~PhysXForceFieldShapeGroup()
{}

IPhysicalForceFieldShape* PhysXForceFieldShapeGroup::_CreateShape(NxForceFieldShape* s)
{
	IPhysicalForceFieldShape*ret=0;

	switch(s->getType())
	{
	case NX_SHAPE_SPHERE:
		ret=new PhysXForceFieldSphereShape((NxSphereForceFieldShape*)s);
		break;
	case NX_SHAPE_BOX:
		ret=new PhysXForceFieldBoxShape((NxBoxForceFieldShape*)s);
		break;
	case NX_SHAPE_CAPSULE:
		ret=new PhysXForceFieldCapsuleShape((NxCapsuleForceFieldShape*)s);
		break;
	case NX_SHAPE_CONVEX:
		ret=new PhysXForceFieldConvexShape((NxConvexForceFieldShape*)s);
		break;
	}
	return ret;
}
NxForceFieldShapeGroup* PhysXForceFieldShapeGroup::getNxShapeGroup()
{
	return m_group;
}


void PhysXForceFieldShapeGroup::setName(const core::string& name)
{
	m_name=name;
	m_group->setName(core::string_to_char(m_name).c_str());
}

const core::string& PhysXForceFieldShapeGroup::getName()const
{
	return m_name;
}


void PhysXForceFieldShapeGroup::setUserData(IObject* d)
{
	m_userData=d;
}

IObject* PhysXForceFieldShapeGroup::getUserData()const
{
	return m_userData;
}


IPhysicalForceFieldShape* PhysXForceFieldShapeGroup::createShape(IPhysicalForceFieldShapeDesc* desc)
{
	NxForceFieldShapeDesc*dsc=0;
	switch(desc->getType())
	{
	case EFFShape_Sphere:
		dsc=new NxSphereForceFieldShapeDesc();
		break;
	case EFFShape_Box:
		dsc=new NxBoxForceFieldShapeDesc();
		break;
	case EFFShape_Capsule:
		dsc=new NxCapsuleForceFieldShapeDesc();
		break;
	case EFFShape_Convex:
		dsc=new NxConvexForceFieldShapeDesc();
		break;
	default:
		return 0;
	}
	PhysXObjectConverter::ConvertForceFieldShapeDesc(desc,dsc,(PhysXManager*)getForceField()->getCreator());
	NxForceFieldShape* s= m_group->createShape(*dsc);
	delete dsc;
	if(!s)
		return 0;
	IPhysicalForceFieldShape* ret=_CreateShape(s);
	if(ret)
		m_shapes.push_back(ret);
	return ret;
}

void PhysXForceFieldShapeGroup::releaseShape(IPhysicalForceFieldShape*shape)
{
	switch(shape->getType())
	{
	case EFFShape_Sphere:
		m_group->releaseShape(*((PhysXForceFieldSphereShape*)shape)->getNxShape());
		break;
	case EFFShape_Box:
		m_group->releaseShape(*((PhysXForceFieldBoxShape*)shape)->getNxShape());
		break;
	case EFFShape_Capsule:
		m_group->releaseShape(*((PhysXForceFieldCapsuleShape*)shape)->getNxShape());
		break;
	case EFFShape_Convex:
		m_group->releaseShape(*((PhysXForceFieldConvexShape*)shape)->getNxShape());
		break;
	default:
		return ;
	}
	std::list<IPhysicalForceFieldShape*>::iterator it=m_shapes.begin();
	for(;it!=m_shapes.end();++it)
	{
		if((*it)==shape)
		{
			delete *it;
			m_shapes.erase(it);
			break;
		}
	}
}


IPhysicalForceField* PhysXForceFieldShapeGroup::getForceField()const
{
	return (IPhysicalForceField*)m_group->getForceField()->userData;
}


const std::list<IPhysicalForceFieldShape*>& PhysXForceFieldShapeGroup::getShapes()const
{
	return m_shapes;
}


}
}



