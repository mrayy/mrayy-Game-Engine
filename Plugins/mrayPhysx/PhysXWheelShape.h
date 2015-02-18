

/********************************************************************
	created:	2012/01/02
	created:	2:1:2012   10:29
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXWheelShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXWheelShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXWheelShape__
#define __PhysXWheelShape__

#include "IPhysicalWheelShape.h"
#include "PhysXShape.h"

class NxWheelShape;

namespace mray
{
namespace physics
{

class PhysXWheelShape:public IPhysicalWheelShape,public PhysXShape
{
private:
protected:
	NxWheelShape* m_wheelShape;
public:
	PhysXWheelShape(NxWheelShape*shape,IPhysicalNode* node);
	~PhysXWheelShape();

	NxWheelShape* GetNXWheelShape()const{return m_wheelShape;}
	
	virtual IPhysicalShape* GetShape(){return this;}
	virtual const IPhysicalShape* GetShape()const{return this;}



	void SetRadius(float r);
	float GetRadius()const;

	void SetSuspensionTravel(float v);
	float GetSuspensionTravel()const;

	void SetSuspension(const WheelShapeDesc::SuspensionSpring&s);
	void GetSuspension(WheelShapeDesc::SuspensionSpring&s)const;

	void SetLongitudalTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s);
	void GetLongitudalTireForceFunction(WheelShapeDesc::TireFunctionDesc&s);	

	void SetLateralTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s);	
	void GetLateralTireForceFunction(WheelShapeDesc::TireFunctionDesc&s)const;	

	void SetInverseWheelMass(float invM);
	float GetInverseWheelMass()const;

	void SetMotorTorque(float t);
	float GetMotorTorque()const;

	void SetBrakeTorque(float t);
	float GetBrakeTorque()const;

	void SetSteerAngle(float t);
	float GetSteerAngle()const;

	void SetAxleSpeed(float s);
	float GetAxleSpeed()const;

	IPhysicalShape* GetContact(WheelContactData& contact);




};


}
}

#endif
