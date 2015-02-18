
/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   15:23
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleWheelDesc.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleWheelDesc
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleWheelDesc__
#define __VehicleWheelDesc__

#include "common.h"
#include "mtypes.h"

namespace mray
{
namespace physics
{

	enum EVehicleWheelFlags
	{
		EVW_Steerable_Input	=BIT(0),
		EVW_Steerable_Auto	=BIT(1),
		EVW_AffectedByHandbrake	=BIT(2),
		EVW_Accelerated		=BIT(3),

		EVW_AllFlags	=	EVW_Steerable_Auto | 
							EVW_AffectedByHandbrake |
							EVW_Accelerated
	};

class VehicleWheelDesc
{
private:
protected:
public:

	core::string name;

	math::vector3d position;
	
	float radius;
	float width;

	float suspension;

	float springRestitution;
	float springDamping;
	float springBias;

	float wheelMass;

	float maxBrakeForce;
	float frictionToSide;
	float frictionToFront;

	//must be either 0 or greater than 3
	uint wheelApproximation;

	uint WheelFlags;

	void *userData;
public:
	VehicleWheelDesc()
	{
		setDefault();
	}
	virtual~VehicleWheelDesc()
	{
	}

	void setDefault()
	{
		position=0;
		wheelMass=10;

		radius=1;
		width=0.1;

		suspension=1;

		springRestitution=1;
		springDamping=0;
		springBias=0;

		maxBrakeForce=0;
		frictionToSide=1;
		frictionToFront=1;

		wheelApproximation=0;

		WheelFlags=0;

		userData=0;
	}

	bool isValid()const
	{
		if(wheelApproximation>0 && wheelApproximation<4)
			return false;
		if((WheelFlags & EVW_Steerable_Input) && (WheelFlags & EVW_Steerable_Auto))
			return false;
		return true;
	}
};


}
}

#endif

