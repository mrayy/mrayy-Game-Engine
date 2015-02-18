

/********************************************************************
	created:	2009/07/15
	created:	15:7:2009   20:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXSphericalJoint3D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXSphericalJoint3D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXSphericalJoint3D___
#define ___PhysXSphericalJoint3D___

#include <IPhysicalJoint.h>
#include "IPhysicalNode.h"
#include "CompileConfig.h"
#include "IPhysXJoint.h"

class NxJoint;
class NxSphericalJointDesc;

namespace mray{
namespace physics{
	class IPhysicalJointSphericalDesc;

class MRAY_PHYSX_DLL PhysXSphericalJoint3D:public ISphericalJoint3D ,public IPhysXJoint
{
	NxJoint*m_joint;
public:

	PhysXSphericalJoint3D(IPhysicManager*c,NxJoint*joint);
	virtual~PhysXSphericalJoint3D();

	NxJoint*getPhysXJoint(){return m_joint;}

	IPhysicalNode* getNode1();
	IPhysicalNode* getNode2();

	const math::quaternion& getInitialNode2Orintation(){return m_node2RelativeQuat;}
	math::quaternion getCurrentNode2Orintation(){return getNode2LocalOrintation();}
	//Sets the point where the two nodes are attached, specified in global coordinates.
	void setGlobalAnchor(const math::vector3d &v);


	math::vector3d getGlobalAnchor();

	//Sets the direction of the joint's primary axis, specified in global coordinates.
	void setGlobalAxis(const math::vector3d &v);

	math::vector3d getGlobalAxis();

	EJoint3DState getJointState();

	void setBreakable(float maxForce,float maxTorque);

	// Sets the solver extrapolation factor.
	void setSolverExtrapolationFactor(float f);

	float getSolverExtrapolationFactor();

	void setUseAccelerationSpring(bool u);
	bool getUseAccelerationSpring();

	void setLimitPoint(const math::vector3d&p,bool pointOnNode2=true);

	// returns true if point is on node2
	bool getLimitPoint(math::vector3d&p);


	bool addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, float restitution = 0.0f) ;

	void clearLimitPlanes();

	void setProjectionMode(EJointProjectionMode m);
	EJointProjectionMode getProjectionMode();

	virtual void SaveToDesc(IPhysicalJointSphericalDesc* desc);
	virtual void LoadFromDesc(const IPhysicalJointSphericalDesc* desc);

	static void ConvertToDesc(const IPhysicalJointSphericalDesc* srcDesc,NxSphericalJointDesc* dstDesc);
	static void ConvertToDesc(const NxSphericalJointDesc* srcDesc,IPhysicalJointSphericalDesc* dstDesc);
};


}
}


#endif //___PhysXSphericalJoint3D___
