


/********************************************************************
	created:	2012/01/02
	created:	2:1:2012   10:17
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicalWheelShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicalWheelShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicalWheelShape__
#define __IPhysicalWheelShape__

#include "IPhysicalShape.h"
#include "PhysicalShapes.h"

namespace mray
{
namespace physics
{

class IPhysicalWheelShape
{
private:
protected:
public:
	IPhysicalWheelShape(){}
	virtual~IPhysicalWheelShape(){}

	virtual IPhysicalShape* GetShape()=0;
	virtual const IPhysicalShape* GetShape()const=0;

	virtual void SetRadius(float r)=0;
	virtual float GetRadius()const=0;

	virtual void SetSuspensionTravel(float v)=0;
	virtual float GetSuspensionTravel()const=0;

	virtual void SetSuspension(const WheelShapeDesc::SuspensionSpring&s)=0;
	virtual void GetSuspension(WheelShapeDesc::SuspensionSpring&s)const=0;

	virtual void SetLongitudalTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s)=0;
	virtual void GetLongitudalTireForceFunction(WheelShapeDesc::TireFunctionDesc&s)=0;	

	virtual void SetLateralTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s)=0;	
	virtual void GetLateralTireForceFunction(WheelShapeDesc::TireFunctionDesc&s)const=0;	

	virtual void SetInverseWheelMass(float invM)=0;
	virtual float GetInverseWheelMass()const=0;

	virtual void SetMotorTorque(float t)=0;
	virtual float GetMotorTorque()const=0;

	virtual void SetBrakeTorque(float t)=0;
	virtual float GetBrakeTorque()const=0;

	virtual void SetSteerAngle(float t)=0;
	virtual float GetSteerAngle()const=0;

	virtual void SetAxleSpeed(float s)=0;
	virtual float GetAxleSpeed()const=0;

	virtual IPhysicalShape* GetContact(WheelContactData& contact)=0;
};


}
}

#endif

