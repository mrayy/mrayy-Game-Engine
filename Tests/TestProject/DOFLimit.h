

/********************************************************************
	created:	2009/10/16
	created:	16:10:2009   15:50
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\DOFLimit.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	DOFLimit
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DOFLimit___
#define ___DOFLimit___

#include <IPhysical3DDef.h>

namespace mray{
namespace xml
{
	class XMLElement;
}


class DOFLimit
{
private:
protected:
public:
	DOFLimit(){
		xlimitType=ylimitType=zlimitType=physics::ED6Joint_Motion_Locked;
		swing1Type=swing2Type=twistType=physics::ED6Joint_Motion_Locked;
	}
	virtual~DOFLimit(){}

	void Serialize(xml::XMLElement*elem);
public:
	physics::ED6JointMotion	xlimitType;
	physics::ED6JointMotion	ylimitType;
	physics::ED6JointMotion	zlimitType;
	physics::PhysicalJointLimitSoftDesc linearLimitDesc;
	physics::ED6JointMotion	swing1Type;
	physics::ED6JointMotion	swing2Type;
	physics::ED6JointMotion	twistType;
	physics::PhysicalJointLimitSoftDesc swing1Limit;
	physics::PhysicalJointLimitSoftDesc swing2Limit;
	physics::PhysicalJointLimitSoftPairDesc twistLimit;
	math::vector3d frameRot;
};

}


#endif //___DOFLimit___
