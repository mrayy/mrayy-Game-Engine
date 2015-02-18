/********************************************************************
	created:	2012/07/13
	created:	13:7:2012   11:15
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\PhysicalJointDOF6Component.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	PhysicalJointDOF6Component
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicalJointDOF6Component___
#define ___PhysicalJointDOF6Component___


#include "IPhysical6DOFJointComponent.h"
#include "IPhysical3DDef.h"


namespace mray
{
namespace physics
{
	class IPhysicManager;
	class IDOF6Joint3D;
}
namespace game
{
	class PhysicsComponent;


	class PhysicalJointDOF6ComponentImpl;

class  MRAY_GAME_DLL PhysicalJointDOF6Component:public IPhysical6DOFJointComponent
{
	DECLARE_RTTI;
private:
protected:
	physics::IPhysicManager* m_phManager;

	physics::IDOF6Joint3D* m_joint;

	physics::IPhysicalJointD6Desc m_jointDesc;

	bool m_removeNodeOnDestroy;

	virtual void _OnTargetChanged();
	void _JointRemoved()
	{
		m_joint=0;
	}


	math::vector3d m_rotationDrive;
	math::vector3d m_positionDrive;
	virtual void _OnChangeAngularDrive();
	virtual void _OnChangePositionDrive();
public:

	PhysicalJointDOF6Component(GameEntityManager*mngr);
	PhysicalJointDOF6Component(physics::IPhysicManager* phManager);
	virtual~PhysicalJointDOF6Component();

	physics::IPhysicalJoint* GetJoint();
	physics::IDOF6Joint3D* GetDOF6Joint(){return m_joint;}
	void SetJoint(physics::IDOF6Joint3D* node);

	virtual math::vector3d TransformToJointSpaceAngles(const math::vector3d& angles);
	virtual math::vector3d GetCurrentAngles();
	virtual float GetCurrentAngle(int axis);
	virtual float GetCurrentLinearDistance(int axis);

	virtual void SetRotationDrive(const math::vector3d& angles);

	virtual void SetTwistDrive(float v);
	virtual void SetSwing1Drive(float v);
	virtual void SetSwing2Drive(float v);

	virtual void SetPositionDrive(const math::vector3d& angles);
	virtual void SetXDrive(float v);
	virtual void SetYDrive(float v);
	virtual void SetZDrive(float v);

	virtual math::vector3d GetRotationDrive(){return m_rotationDrive;}
	virtual math::vector3d GetPositionDrive(){return m_positionDrive;}


	void SetD6JointDesc(const physics::IPhysicalJointD6Desc& d){m_jointDesc=d;}
	const physics::IPhysicalJointD6Desc& GetD6JointDesc(){return m_jointDesc;}

	virtual physics::IPhysicalJoint3DDesc* GetJointDesc(){return &m_jointDesc;}

	virtual xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
DECLARE_GAMECOMPONENT_FACTORY(PhysicalJointDOF6Component);

}
}

#endif