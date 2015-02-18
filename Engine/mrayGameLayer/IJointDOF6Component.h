

/********************************************************************
	created:	2013/01/25
	created:	25:1:2013   14:49
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\IJointDOF6Component.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IJointDOF6Component
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IJointDOF6Component__
#define __IJointDOF6Component__

#include "IPhysicalJointComponent.h"


namespace mray
{
namespace game
{

class IJointDOF6Component:public IPhysicalJointComponent
{
protected:
public:
	IJointDOF6Component()
	{}
	virtual~IJointDOF6Component(){}


	virtual	math::vector3d GetCurrentAngles()=0;
	virtual	float GetCurrentAngle(int axis)=0;
	virtual	float GetCurrentLinearDistance(int axis)=0;

	virtual	void SetRotationDrive(const math::vector3d& angles)=0;
	virtual	void SetTwistDrive(float v)=0;
	virtual	void SetSwing1Drive(float v)=0;
	virtual	void SetSwing2Drive(float v)=0;

	virtual	void SetPositionDrive(const math::vector3d& angles)=0;
	virtual	void SetXDrive(float v)=0;
	virtual	void SetYDrive(float v)=0;
	virtual	void SetZDrive(float v)=0;
};

}
}


#endif
