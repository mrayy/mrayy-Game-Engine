

/********************************************************************
	created:	2009/07/15
	created:	15:7:2009   20:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXFixedJoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXFixedJoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXFixedJoint___
#define ___PhysXFixedJoint___


#include <IPhysicalJoint.h>
#include "IPhysicalNode.h"
#include "CompileConfig.h"
#include "IPhysXJoint.h"

class NxJoint;

namespace mray{
namespace physics{

class MRAY_PHYSX_DLL PhysXFixedJoint:public IFixedJoint3D,public IPhysXJoint
{
private:
protected:
	NxJoint*m_joint;
public:

	PhysXFixedJoint(IPhysicManager*c,NxJoint*joint);
	virtual~PhysXFixedJoint();

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

	virtual void SaveToDesc(IPhysicalJointFixedDesc* desc);
	virtual void LoadFromDesc(const IPhysicalJointFixedDesc* desc);
};

}
}

#endif //___PhysXFixedJoint___
