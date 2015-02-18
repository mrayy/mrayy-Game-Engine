

/********************************************************************
	created:	2013/04/18
	created:	18:4:2013   18:41
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\IPhysical6DOFJointComponent.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IPhysical6DOFJointComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IPhysical6DOFJointComponent__
#define __IPhysical6DOFJointComponent__

#include "IPhysicalJointComponent.h"


namespace mray
{
namespace game
{

class IPhysical6DOFJointComponent:public IPhysicalJointComponent
{
protected:
	
public:
	IPhysical6DOFJointComponent(){}
	virtual~IPhysical6DOFJointComponent(){}


	virtual math::vector3d GetCurrentAngles()=0;
	virtual float GetCurrentAngle(int axis)=0;
	virtual float GetCurrentLinearDistance(int axis)=0;

	virtual void SetRotationDrive(const math::vector3d& angles)=0;
	virtual math::vector3d GetRotationDrive()=0;

	virtual void SetTwistDrive(float v)=0;
	virtual void SetSwing1Drive(float v)=0;
	virtual void SetSwing2Drive(float v)=0;

	virtual void SetPositionDrive(const math::vector3d& v)=0;
	virtual math::vector3d GetPositionDrive()=0;

	virtual void SetXDrive(float v)=0;
	virtual void SetYDrive(float v)=0;
	virtual void SetZDrive(float v)=0;

};

}
}


#endif
