



#include "stdafx.h"
#include "PhysicXMLSerializer.h"
#include "IPhysical3DDef.h"
#include "XMLElement.h"


namespace mray
{
namespace physics
{

#define IMPLEMENT_LOAD_SAVE_XML(type,node)\
	type* PhysicXMLSerializer::##node ##_Load(xml::XMLElement*e)\
	{\
		xml::XMLElement* elem=e->getSubElement(mT(#node));\
		if(elem)\
		{\
			type* d=new type();\
			d->LoadFromXML(elem);\
			return d;\
		}\
		return 0;\
	}\
	xml::XMLElement* PhysicXMLSerializer::Parse(xml::XMLElement*e,type* d)\
	{\
		xml::XMLElement* elem=e->getSubElement(mT(#node));\
		if(elem)\
			d->LoadFromXML(elem);\
		return elem;\
	}\
	xml::XMLElement* PhysicXMLSerializer::Save(xml::XMLElement*e,type*d)\
	{\
		xml::XMLElement* ee=new xml::XMLElement(mT(#node));\
		e->addSubElement(ee);\
		d->SaveToXML(ee);\
		return ee;\
	} 
IMPLEMENT_LOAD_SAVE_XML(PhysicsSystemDesc,PhysicsSystem);
IMPLEMENT_LOAD_SAVE_XML(PhysicalBodyDesc,PhysicalBody);
IMPLEMENT_LOAD_SAVE_XML(PhysicalNodeDesc,PhysicalNode);
IMPLEMENT_LOAD_SAVE_XML(PhysicalSpringDesc,PhysicalSpring);
IMPLEMENT_LOAD_SAVE_XML(PhysicalJointLimitDesc,PhysicalJointLimit);
IMPLEMENT_LOAD_SAVE_XML(PhysicalJointLimitPairDesc,PhysicalJointLimitPair);
IMPLEMENT_LOAD_SAVE_XML(PhysicalMotor3DDesc,PhysicalMotor);
IMPLEMENT_LOAD_SAVE_XML(PhysicalJointLimitSoftDesc,PhysicalJointLimitSoft);
IMPLEMENT_LOAD_SAVE_XML(PhysicalJointLimitSoftPairDesc,PhysicalJointLimitSoftPair);
IMPLEMENT_LOAD_SAVE_XML(PhysicalJointDriveDesc,PhysicalJointDrive);
/*
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointDistanceDesc,PhysicalJointDistance);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointSphericalDesc,PhysicalJointSpherical);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointRevoluteDesc,PhysicalJointRevolute);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointPulleyDesc,PhysicalJointPulley);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointPrismaticDesc,PhysicalJointPrismatic);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointPointOnLineDesc,IPhysicalJointPointOnLine);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointCylinderDesc,PhysicalJointCylinder);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointFixedDesc,PhysicalJointFixed);
IMPLEMENT_LOAD_SAVE_XML(IPhysicalJointD6Desc,PhysicalJointD6);
*/
IMPLEMENT_LOAD_SAVE_XML(PhysicMaterialDesc,PhysicMaterial);


IPhysicalJoint3DDesc* PhysicXMLSerializer::PhysicalJoint_Load(xml::XMLElement*e)
{
	xml::XMLElement* elem=e->getSubElement(mT("PhysicalJoint"));
	if(elem)
	{
		IPhysicalJoint3DDesc* d;
		xml::XMLAttribute*attr= elem->getAttribute(mT("JointType"));
		if(!attr)
			return 0;
		d=IPhysicalJoint3DDesc::CreateJoint(attr->value);
		if(d)
			d->LoadFromXML(elem);
		return d;
	}
	return 0;
}
xml::XMLElement* PhysicXMLSerializer::Parse(xml::XMLElement*e,IPhysicalJoint3DDesc* d)
{
	xml::XMLElement* elem=e->getSubElement(mT("PhysicalJoint"));
	if(elem)
		d->LoadFromXML(elem);
	return elem;
}
xml::XMLElement* PhysicXMLSerializer::Save(xml::XMLElement*e,IPhysicalJoint3DDesc*d)
{
	xml::XMLElement* ee=new xml::XMLElement(mT("PhysicalJoint"));
	e->addSubElement(ee);
	d->SaveToXML(ee);
	return ee;
} 
}
}