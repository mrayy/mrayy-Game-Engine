
/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   14:08
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleMotor.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleMotor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleMotor__
#define __VehicleMotor__

#include "compileconfig.h"
#include "VehicleMotorDesc.h"

namespace mray
{
namespace physics
{
	class VehicleGear;

class MRAY_DLL VehicleMotor
{
private:
protected:
	float m_RPM;
	float m_maxTorque;
	float m_maxTorquePos;

	VehicleMotorDesc m_motorDesc;
public:

	enum EGearChangeResult
	{
		ENone,
		EGearUp,
		EGearDown
	};
	VehicleMotor(const VehicleMotorDesc& desc);
	virtual~VehicleMotor();

	void SetRPM(float rpm);
	float GetRPM()const;

	float GetTorque();

	EGearChangeResult ChangeGears(const VehicleGear* gears,float threshold);

	const VehicleMotorDesc& GetDesc()const
	{
		return m_motorDesc;
	}
};


}
}

#endif
