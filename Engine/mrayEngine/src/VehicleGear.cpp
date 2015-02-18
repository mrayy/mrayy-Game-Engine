
#include "stdafx.h"
#include "VehicleGear.h"




namespace mray
{
namespace physics
{

void VehicleGear::GearUp()
{
	m_currGear=math::Min<int>(m_currGear+1,m_gearDesc.forwardGearRatio.size());
}

void VehicleGear::GearDown()
{
	m_currGear=math::Max(m_currGear,1);
}

float VehicleGear::GetCurrRatio()const
{
	return GetRatio(m_currGear);
}
float VehicleGear::GetRatio(int gear)const
{
	if(gear>0 && gear<m_gearDesc.forwardGearRatio.size())
		return m_gearDesc.forwardGearRatio[gear-1];
	if(gear==-1)
		return m_gearDesc.backwardGearRatio;
	return 0;
}

}
}