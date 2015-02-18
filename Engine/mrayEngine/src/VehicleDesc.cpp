
#include "stdafx.h"
#include "VehicleDesc.h"

#include "PhysicalShapes.h"
#include "VehicleWheelDesc.h"
#include "VehicleGearDesc.h"
#include "VehicleMotorDesc.h"



namespace mray
{
namespace physics
{


VehicleDesc::VehicleDesc()
{
	SetDefault();
}
VehicleDesc::~VehicleDesc()
{
/*	for(int i=0;i<shapes.size();++i)
		delete shapes[i];*/
	for(int i=0;i<wheels.size();++i)
		delete wheels[i];
	delete gearDesc;
	delete motorDesc;
}

void VehicleDesc::SetDefault()
{
//	shapes.clear();
	wheels.clear();
	gearDesc=0;
	motorDesc=0;
	mass=1;
	motorForce=0;
	transmissionEfficiency=1;
	differentialRatio=1;
	steeringMaxAngle=100;
	digitalSteeringDelta=1;
	maxVelocity=100;
	userData=0;
}
VehicleWheelDesc* VehicleDesc::GetWheel(const core::string& name)const
{
	for(int i=0;i<wheels.size();++i)
	{
		if(wheels[i]->name.equals_ignore_case(name))
			return wheels[i];
	}
	return 0;
}


}
}