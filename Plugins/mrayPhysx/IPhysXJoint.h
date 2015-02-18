

/********************************************************************
	created:	2009/07/16
	created:	16:7:2009   13:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\IPhysXJoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	IPhysXJoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysXJoint___
#define ___IPhysXJoint___


#include "compileConfig.h"

class NxJoint;
class NxJointDesc;

namespace mray{
namespace physics{
	class IPhysicalJoint3DDesc;

class MRAY_PHYSX_DLL IPhysXJoint
{
private:
protected:
	NxJoint*m_joint;
	math::quaternion m_node2RelativeQuat;
public:
	IPhysXJoint(NxJoint*j);

	virtual NxJoint* getPhysXJoint()=0;

	math::quaternion getNode2LocalOrintation();


	static void ConvertToDesc(const IPhysicalJoint3DDesc* srcDesc,NxJointDesc* dstDesc);
	static void ConvertToDesc(const NxJointDesc* srcDesc,IPhysicalJoint3DDesc* dstDesc);
};

}
}

#endif //___IPhysXJoint___
