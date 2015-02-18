

/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   14:08
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleMotorDesc.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleMotorDesc
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleMotorDesc__
#define __VehicleMotorDesc__

#include "CKeyableAttr.h"

namespace mray
{
namespace physics
{

class VehicleMotorDesc
{
private:
protected:
public:
	core::CKeyableAttr<float> torqueCurve;

	float maxRPMToGearUp;
	float minRPMToGearDown;

	float maxRPM;
	float minRPM;

public:
	VehicleMotorDesc()
	{
		setDefault();
	}
	virtual~VehicleMotorDesc()
	{
	}

	void setDefault()
	{
		torqueCurve.clear();
		maxRPMToGearUp=4000;
		minRPMToGearDown=1500;
		maxRPM=5000;
		minRPM=1000;
	}
	void setToCorvette()
	{
		torqueCurve.clear();
		torqueCurve.addKey(1000,400);
		torqueCurve.addKey(2000,435);
		torqueCurve.addKey(3000,475);
		torqueCurve.addKey(4000,475);
		torqueCurve.addKey(5000,365);

		maxRPMToGearUp=5000;
		minRPMToGearDown=2500;
		maxRPM=6000;
		minRPM=1500;
	}

	bool isValid()const
	{
		if(torqueCurve.getKeysCount()==0)
			return false;
		if(maxRPMToGearUp<minRPMToGearDown)
			return false;
		return true;
	}
};

}
}

#endif
