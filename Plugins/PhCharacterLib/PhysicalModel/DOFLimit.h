

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
#include "PhysicalJointDoubleLimitSoftDesc.h"

namespace mray{
namespace xml
{
	class XMLElement;
}


namespace PhysicsBasedCharacter
{

	class DOFLimit
	{
	private:
	protected:
	public:
		DOFLimit(){
			xlimitType=ylimitType=zlimitType=physics::ED6Joint_Motion_Locked;
			swing1Type=swing2Type=twistType=physics::ED6Joint_Motion_Locked;
			collision = false;
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
		PhysicalJointDoubleLimitSoftDesc swing1Limit;
		PhysicalJointDoubleLimitSoftDesc swing2Limit;
		physics::PhysicalJointLimitSoftPairDesc twistLimit;
		bool collision;
	};
}
}


#endif //___DOFLimit___
