


/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   14:18
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleGearDesc.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleGearDesc
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleGearDesc__
#define __VehicleGearDesc__

#include "mtypes.h"

namespace mray
{
namespace physics
{

class VehicleGearDesc
{
private:
protected:
public:
	std::vector<float> forwardGearRatio;
	float backwardGearRatio;
public:

	VehicleGearDesc()
	{
		setDefault();
	}
	virtual~VehicleGearDesc(){}


	void setDefault()
	{
		backwardGearRatio=-1;
		forwardGearRatio.clear();
	}
	void setToCorvette()
	{
		forwardGearRatio.clear();
		forwardGearRatio.push_back(2.66);
		forwardGearRatio.push_back(1.78);
		forwardGearRatio.push_back(1.30);
		forwardGearRatio.push_back(1);
		forwardGearRatio.push_back(0.74);
		forwardGearRatio.push_back(0.50);

		backwardGearRatio=-2.90;
	}

	bool isValid()const
	{
		if(forwardGearRatio.size()==0)
			return false;
		if(backwardGearRatio>0)
			return false;
		for(uint i=0;i<forwardGearRatio.size();++i)
		{
			if(forwardGearRatio[i]<0)
				return false;
		}
		return true;
	}
};

}
}

#endif