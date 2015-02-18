


/********************************************************************
	created:	2012/01/02
	created:	2:1:2012   13:01
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleDesc.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleDesc
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleDesc__
#define __VehicleDesc__

#include "compileConfig.h"
#include "mstring.h"

namespace mray
{
namespace physics
{

	class IPhysicalShapeDesc;
	class VehicleWheelDesc;
	class VehicleGearDesc;
	class VehicleMotorDesc;

class MRAY_DLL VehicleDesc
{
private:
protected:
public:
	VehicleDesc();
	virtual~VehicleDesc();

	void SetDefault();

	VehicleWheelDesc* GetWheel(const core::string& name)const;

	//std::vector<IPhysicalShapeDesc*> shapes;
	std::vector<VehicleWheelDesc*> wheels;

	VehicleGearDesc* gearDesc;
	VehicleMotorDesc* motorDesc;

	core::string vehicleTypeName;

	math::vector3d position;
	float mass;
	float motorForce;
	float transmissionEfficiency;
	float differentialRatio;

	math::vector3d steeringTurnPoint;
	math::vector3d steeringSteerPoint;
	float steeringMaxAngle;

	math::vector3d centerOfMass;

	float digitalSteeringDelta;

	float maxVelocity;

	void *userData;

};

}
}

#endif
