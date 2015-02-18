
#include "stdafx.h"
#include "IPhysicalNode.h"

namespace mray{
	namespace physics{

	IMPLEMENT_PROPERTY_TYPE_HEADER(Name,IPhysicalNode,core::string,mT("Name"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,IPhysicalNode,core::string,SetName,GetName,,,"");

IMPLEMENT_PROPERTY_TYPE_HEADER(Mass,IPhysicalNode,float,mT("Mass"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Mass,IPhysicalNode,float,
	setMass,getMass,core::StringConverter::toString,core::StringConverter::toFloat,false);


IMPLEMENT_PROPERTY_TYPE_HEADER(LinearDamping,IPhysicalNode,float,mT("LinearDamping"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(LinearDamping,IPhysicalNode,float,
								setLinearDamping,getLinearDamping,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(GlobalPosition,IPhysicalNode,vector3d,mT("GlobalPosition"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(GlobalPosition,IPhysicalNode,math::vector3d,
								setGlobalPosition,getGlobalPosition,core::StringConverter::toString,core::StringConverter::toVector3d,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(GlobalOrintation,IPhysicalNode,quaternion,mT("GlobalOrintation"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(GlobalOrintation,IPhysicalNode,math::quaternion,
								setGlobalOrintation,getGlobalOrintation,core::StringConverter::toString,core::StringConverter::toQuaternion,false);


IMPLEMENT_PROPERTY_TYPE_HEADER(LinearVelocity,IPhysicalNode,vector3d,mT("LinearVelocity"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(LinearVelocity,IPhysicalNode,math::vector3d,
								setLinearVelocity,getLinearVelocity,core::StringConverter::toString,core::StringConverter::toVector3d,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(AngularVelocity,IPhysicalNode,vector3d,mT("AngularVelocity"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AngularVelocity,IPhysicalNode,math::vector3d,
								setAngularVelocity,getAngularVelocity,core::StringConverter::toString,core::StringConverter::toVector3d,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(MaxAngularVelocity,IPhysicalNode,float,mT("MaxAngularVelocity"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(MaxAngularVelocity,IPhysicalNode,float,
								setMaxAngularVelocity,getMaxAngularVelocity,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(LinearMomentum,IPhysicalNode,vector3d,mT("LinearMomentum"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(LinearMomentum,IPhysicalNode,math::vector3d,
								setLinearMomentum,getLinearMomentum,core::StringConverter::toString,core::StringConverter::toVector3d,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(AngularMomentum,IPhysicalNode,vector3d,mT("AngularMomentum"),EPBT_Struct,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(AngularMomentum,IPhysicalNode,math::vector3d,
								setAngularMomentum,getAngularMomentum,core::StringConverter::toString,core::StringConverter::toVector3d,false);


IMPLEMENT_PROPERTY_TYPE_HEADER(SleepLinearVelocity,IPhysicalNode,float,mT("SleepLinearVelocity"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(SleepLinearVelocity,IPhysicalNode,float,
								setSleepLinearVelocity,getSleepLinearVelocity,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(SleepAngularVelocity,IPhysicalNode,float,mT("SleepAngularVelocity"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(SleepAngularVelocity,IPhysicalNode,float,
								setSleepAngularVelocity,getSleepAngularVelocity,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(SleepEnergyThreshold,IPhysicalNode,float,mT("SleepEnergyThreshold"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(SleepEnergyThreshold,IPhysicalNode,float,
								setSleepEnergyThreshold,getSleepEnergyThreshold,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(SolverIterationCount,IPhysicalNode,uint,mT("SolverIterationCount"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(SolverIterationCount,IPhysicalNode,uint,
								SetSolverIterationCount,GetSolverIterationCount,core::StringConverter::toString,core::StringConverter::toUInt,false);


IMPLEMENT_PROPERTY_TYPE_HEADER(ContactReportThreshold,IPhysicalNode,float,mT("ContactReportThreshold"),EPBT_Basic,mT(""),1);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ContactReportThreshold,IPhysicalNode,float,
								SetContactReportThreshold,GetContactReportThreshold,core::StringConverter::toString,core::StringConverter::toFloat,false);


void IPhysicalNode::fillProperties()
{
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeName::instance);
		dic->addPropertie(&PropertyTypeMass::instance);
		dic->addPropertie(&PropertyTypeLinearDamping::instance);
		dic->addPropertie(&PropertyTypeGlobalPosition::instance);
		dic->addPropertie(&PropertyTypeGlobalOrintation::instance);
		dic->addPropertie(&PropertyTypeLinearVelocity::instance);
		dic->addPropertie(&PropertyTypeAngularVelocity::instance);
		dic->addPropertie(&PropertyTypeMaxAngularVelocity::instance);
		dic->addPropertie(&PropertyTypeLinearMomentum::instance);
		dic->addPropertie(&PropertyTypeAngularMomentum::instance);
		dic->addPropertie(&PropertyTypeSleepLinearVelocity::instance);
		dic->addPropertie(&PropertyTypeSleepAngularVelocity::instance);
		dic->addPropertie(&PropertyTypeSleepEnergyThreshold::instance);
		dic->addPropertie(&PropertyTypeSolverIterationCount::instance);
		dic->addPropertie(&PropertyTypeContactReportThreshold::instance);
	}
}
}
}