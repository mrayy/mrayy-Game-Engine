

#include "stdafx.h"
#include "VehicleMotor.h"
#include "VehicleGear.h"


namespace mray
{
namespace physics
{

VehicleMotor::VehicleMotor(const VehicleMotorDesc& desc)
:m_motorDesc(desc),m_RPM(0)
{
	m_maxTorque=0;
	m_maxTorquePos=-1;
	for(int i=0;i<m_motorDesc.torqueCurve.getKeysCount();++i)
	{
		const core::CKeyableAttr<float>::SKey* k=m_motorDesc.torqueCurve.getKeyAtIndex(i);
		if(k->value>m_maxTorque)
		{
			m_maxTorque=k->value;
			m_maxTorquePos=i;
		}
	}
}
VehicleMotor::~VehicleMotor()
{
}

void VehicleMotor::SetRPM(float rpm)
{
	m_RPM=rpm;
}
float VehicleMotor::GetRPM()const
{
	return m_RPM;
}

float VehicleMotor::GetTorque()
{
	return m_motorDesc.torqueCurve.getInterpolatedKey(m_RPM);
}

VehicleMotor::EGearChangeResult VehicleMotor::ChangeGears(const VehicleGear* gears,float threshold)
{
	int g=gears->GetCurrGear();
	if(m_RPM > m_motorDesc.maxRPMToGearUp && g<gears->GetDesc().forwardGearRatio.size())
		return EGearUp;
	if(m_RPM<m_motorDesc.minRPMToGearDown && g>1)
		return EGearDown;

	return ENone;
}



}
}
