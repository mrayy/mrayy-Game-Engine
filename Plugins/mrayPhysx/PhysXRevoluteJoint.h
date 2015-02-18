
/********************************************************************
	created:	2009/07/16
	created:	16:7:2009   1:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXRevoluteJoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXRevoluteJoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXRevoluteJoint___
#define ___PhysXRevoluteJoint___

#include <IPhysicalJoint.h>
#include "IPhysicalNode.h"
#include "CompileConfig.h"
#include "IPhysXJoint.h"

class NxJoint;
class NxRevoluteJointDesc;

namespace mray{
namespace physics{

	class IPhysicalJointRevoluteDesc;

class MRAY_PHYSX_DLL PhysXRevoluteJoint:public IRevoluteJoint3D,public IPhysXJoint
{
private:
protected:
	NxJoint*m_joint;
public:

	PhysXRevoluteJoint(IPhysicManager*c,NxJoint*joint);
	virtual~PhysXRevoluteJoint();

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


	bool getLimit(PhysicalJointLimitPairDesc&d);

	void setMotor(const PhysicalMotor3DDesc&d);
	bool getMotor(PhysicalMotor3DDesc&d);

	void setSpring(const PhysicalSpringDesc&d);
	bool getSpring(PhysicalSpringDesc&d);

	float getAngle();
	float getVelocity();

	void setProjectionMode(EJointProjectionMode m);
	EJointProjectionMode getProjectionMode();



	virtual void SaveToDesc(IPhysicalJointRevoluteDesc* desc);
	virtual void LoadFromDesc(const IPhysicalJointRevoluteDesc* desc);

	static void ConvertToDesc(const IPhysicalJointRevoluteDesc* srcDesc,NxRevoluteJointDesc* dstDesc);
	static void ConvertToDesc(const NxRevoluteJointDesc* srcDesc,IPhysicalJointRevoluteDesc* dstDesc);
};

}
}


#endif //___PhysXRevoluteJoint___
