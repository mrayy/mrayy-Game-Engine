
#include "stdafx.h"
#include "VehicleXMLParser.h"

#include "IStream.h"
#include "XMLTree.h"

#include "VehicleDesc.h"
#include "VehicleMotorDesc.h"
#include "VehicleGearDesc.h"
#include "VehicleWheelDesc.h"


#include "EnumManager.h"

namespace mray
{
namespace physics
{

VehicleXMLParser::VehicleXMLParser()
{
}
VehicleXMLParser::~VehicleXMLParser()
{
}

VehicleDesc* VehicleXMLParser::LoadVehicleDesc(OS::IStream* path)
{
	xml::XMLTree tree;
	if(!path || !tree.load(path))
		return 0;
	return LoadVehicleDesc(tree.getSubElement(mT("Vehicle")));
}
VehicleDesc* VehicleXMLParser::LoadVehicleDesc(xml::XMLElement* node)
{
	if(!node)
		return 0;
	VehicleDesc* ret=new VehicleDesc();
	xml::XMLAttribute* attr;

	attr=node->getAttribute(mT("Type"));
	if(attr)ret->vehicleTypeName=attr->value;

	attr=node->getAttribute(mT("Mass"));
	if(attr)ret->mass=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MotorForce"));
	if(attr)ret->motorForce=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("TransmissionEfficiency"));
	if(attr)ret->transmissionEfficiency=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("DifferentialRatio"));
	if(attr)ret->differentialRatio=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("SteeringTurnPoint"));
	if(attr)ret->steeringTurnPoint=core::StringConverter::toVector3d(attr->value);

	attr=node->getAttribute(mT("SteeringSteerPoint"));
	if(attr)ret->steeringSteerPoint=core::StringConverter::toVector3d(attr->value);

	attr=node->getAttribute(mT("SteeringMaxAngle"));
	if(attr)ret->steeringMaxAngle=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("CenterOfMass"));
	if(attr)ret->centerOfMass=core::StringConverter::toVector3d(attr->value);

	attr=node->getAttribute(mT("DigitalSteeringDelta"));
	if(attr)ret->digitalSteeringDelta=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MaxVelocity"));
	if(attr)ret->maxVelocity=core::StringConverter::toFloat(attr->value);

	ret->motorDesc=LoadMotorDesc(node->getSubElement(mT("Motor")));
	ret->gearDesc=LoadGearDesc(node->getSubElement(mT("Gears")));

	ret->wheels=LoadWheels(node->getSubElement(mT("Wheels")));

	return ret;
}

VehicleMotorDesc* VehicleXMLParser::LoadMotorDesc(xml::XMLElement* node)
{
	if(!node)
		return 0;
	VehicleMotorDesc* ret=new VehicleMotorDesc();
	xml::XMLAttribute* attr;
	xml::XMLElement* elem;

	attr=node->getAttribute(mT("MaxRPMToGearUp"));
	if(attr)ret->maxRPMToGearUp=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MinRPMToGearDown"));
	if(attr)ret->minRPMToGearDown=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MaxRPM"));
	if(attr)ret->maxRPM=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MinRPM"));
	if(attr)ret->minRPM=core::StringConverter::toFloat(attr->value);

	elem=node->getSubElement(mT("Torque"));
	while(elem)
	{
		xml::XMLAttribute* attrVal;
		attr=elem->getAttribute(mT("RPM"));
		attrVal=elem->getAttribute(mT("Value"));
		if(attr && attrVal)
		{
			ret->torqueCurve.addKey(
				core::StringConverter::toFloat(attr->value),
			core::StringConverter::toFloat(attrVal->value));
		}
		elem=elem->nextSiblingElement(mT("Torque"));
	}
	return ret;
}
VehicleGearDesc* VehicleXMLParser::LoadGearDesc(xml::XMLElement* node)
{
	if(!node)
		return 0;
	VehicleGearDesc* ret=new VehicleGearDesc();
	xml::XMLAttribute* attr;
	xml::XMLElement* elem;

	attr=node->getAttribute(mT("BackwardGearRatio"));
	if(attr)ret->backwardGearRatio=core::StringConverter::toFloat(attr->value);

	elem=node->getSubElement(mT("ForwardGearRatio"));
	while(elem)
	{
		attr=elem->getAttribute(mT("Value"));
		if(attr)
			ret->forwardGearRatio.push_back(core::StringConverter::toFloat(attr->value));
		elem=elem->nextSiblingElement(mT("ForwardGearRatio"));
	}

	return ret;
}
VehicleWheelDesc* VehicleXMLParser::LoadWheelDesc(xml::XMLElement* node)
{
	if(!node)
		return 0;
	VehicleWheelDesc* ret=new VehicleWheelDesc();
	xml::XMLAttribute* attr;
	xml::XMLElement* elem;

	attr=node->getAttribute(mT("Name"));
	if(attr)ret->name=attr->value;

	attr=node->getAttribute(mT("Position"));
	if(attr)ret->position=core::StringConverter::toVector3d(attr->value);

	attr=node->getAttribute(mT("Radius"));
	if(attr)ret->radius=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("Width"));
	if(attr)ret->width=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("Suspension"));
	if(attr)ret->suspension=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("SpringRestitution"));
	if(attr)ret->springRestitution=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("SpringDamping"));
	if(attr)ret->springDamping=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("SpringBias"));
	if(attr)ret->springBias=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("WheelMass"));
	if(attr)ret->wheelMass=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("MaxBrakeForce"));
	if(attr)ret->maxBrakeForce=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("FrictionToSide"));
	if(attr)ret->frictionToSide=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("FrictionToFront"));
	if(attr)ret->frictionToFront=core::StringConverter::toFloat(attr->value);

	attr=node->getAttribute(mT("WheelApproximation"));
	if(attr)ret->wheelApproximation=core::StringConverter::toInt(attr->value);

	ret->WheelFlags=0;
	elem=node->getSubElement(mT("Flag"));
	while(elem)
	{
		attr=elem->getAttribute(mT("Name"));
		if(attr)
		{
			int f=EnumManager::getInstance().getValue(mT("EVehicleWheelFlags"),attr->value);
			if(f>=0)
				ret->WheelFlags|=f;
		}
		elem=elem->nextSiblingElement(mT("Flag"));
	}

	return ret;
}

std::vector<VehicleWheelDesc*> VehicleXMLParser::LoadWheels(xml::XMLElement* node)
{
	std::vector<VehicleWheelDesc*> ret;
	if(!node)return ret;
	node=node->getSubElement(mT("Wheel"));
	while(node)
	{
		VehicleWheelDesc* d=LoadWheelDesc(node);
		if(d)
			ret.push_back(d);
		node=node->nextSiblingElement(mT("Wheel"));
	}

	return ret;
}

}
}