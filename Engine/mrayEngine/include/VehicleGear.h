


/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   14:23
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleGear.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleGear
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleGear__
#define __VehicleGear__

#include "VehicleGearDesc.h"
#include "compileconfig.h"

namespace mray
{
namespace physics
{

class MRAY_DLL VehicleGear
{
private:
protected:
	VehicleGearDesc m_gearDesc;
	int m_currGear;
public:
	VehicleGear(const VehicleGearDesc& desc):m_currGear(1),m_gearDesc(desc)
	{}
	virtual~VehicleGear()
	{}

	int GetCurrGear()const{return m_currGear;}

	void GearUp();
	void GearDown();

	float GetCurrRatio()const;
	float GetRatio(int gear)const;

	const VehicleGearDesc& GetDesc()const
	{
		return m_gearDesc;
	}
};

}
}

#endif
