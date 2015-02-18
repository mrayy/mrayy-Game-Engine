

#include "stdafx.h"
#include "IPhysicalCloth.h"

namespace mray
{
namespace physics
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(BendingStiffness,IPhysicalCloth,float,mT("BendingStiffness"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(BendingStiffness,IPhysicalCloth,float,
		setBendingStiffness,getBendingStiffness,core::StringConverter::toString,
		core::StringConverter::toFloat,false);


	IMPLEMENT_PROPERTY_TYPE_HEADER(StretchingStiffness,IPhysicalCloth,float,mT("StretchingStiffness"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(StretchingStiffness,IPhysicalCloth,float,
		setStretchingStiffness,getStretchingStiffness,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(DampingCoeff,IPhysicalCloth,float,mT("DampingCoeff"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(DampingCoeff,IPhysicalCloth,float,
		setDampingCoefficient,getDampingCoefficient,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Friction,IPhysicalCloth,float,mT("Friction"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Friction,IPhysicalCloth,float,
		setFriction,getFriction,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Pressure,IPhysicalCloth,float,mT("Pressure"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Pressure,IPhysicalCloth,float,
		setPressure,getPressure,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(TearFactor,IPhysicalCloth,float,mT("TearFactor"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(TearFactor,IPhysicalCloth,float,
		setTearFactor,getTearFactor,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(SolverIterations,IPhysicalCloth,uint,mT("SolverIterations"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(SolverIterations,IPhysicalCloth,uint,
		setSolverIterations,getSolverIterations,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(CollisionResponseCoeff,IPhysicalCloth,float,mT("CollisionResponseCoeff"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(CollisionResponseCoeff,IPhysicalCloth,float,
		setCollisionResponseCoefficient,getCollisionResponseCoefficient,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(AttachmentResponseCoeff,IPhysicalCloth,float,mT("AttachmentResponseCoeff"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(AttachmentResponseCoeff,IPhysicalCloth,float,
		setAttachmentResponseCoefficient,getAttachmentResponseCoefficient,core::StringConverter::toString,
		core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(ExternalAcceleration,IPhysicalCloth,vector3d,mT("ExternalAcceleration"),EPBT_Struct,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(ExternalAcceleration,IPhysicalCloth,math::vector3d,
		setExternalAcceleration,getExternalAcceleration,core::StringConverter::toString,
		core::StringConverter::toVector3d,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(WindAcceleration,IPhysicalCloth,vector3d,mT("WindAcceleration"),EPBT_Struct,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(WindAcceleration,IPhysicalCloth,math::vector3d,
		setWindAcceleration,getWindAcceleration,core::StringConverter::toString,
		core::StringConverter::toVector3d,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(SleepLinearVelocity,IPhysicalCloth,float,mT("SleepLinearVelocity"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(SleepLinearVelocity,IPhysicalCloth,float,
		setSleepLinearVelocity,getSleepLinearVelocity,core::StringConverter::toString,
		core::StringConverter::toFloat,false);


	void IPhysicalCloth::fillProperties()
	{
		CPropertieDictionary *dic=0;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeBendingStiffness::instance);
			dic->addPropertie(&PropertyTypeStretchingStiffness::instance);
			dic->addPropertie(&PropertyTypeDampingCoeff::instance);
			dic->addPropertie(&PropertyTypeFriction::instance);
			dic->addPropertie(&PropertyTypePressure::instance);
			dic->addPropertie(&PropertyTypeTearFactor::instance);
			dic->addPropertie(&PropertyTypeSolverIterations::instance);
			dic->addPropertie(&PropertyTypeCollisionResponseCoeff::instance);
			dic->addPropertie(&PropertyTypeAttachmentResponseCoeff::instance);
			dic->addPropertie(&PropertyTypeExternalAcceleration::instance);
			dic->addPropertie(&PropertyTypeWindAcceleration::instance);
			dic->addPropertie(&PropertyTypeSleepLinearVelocity::instance);
		}
	}
}
}