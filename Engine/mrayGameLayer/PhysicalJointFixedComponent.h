

/********************************************************************
	created:	2012/08/01
	created:	1:8:2012   16:00
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\PhysicalJointFixedComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	PhysicalJointFixedComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicalJointFixedComponent___
#define ___PhysicalJointFixedComponent___

#include "IPhysicalJointComponent.h"
#include "IPhysical3DDef.h"

namespace mray
{
	namespace physics
	{
		class IPhysicManager;
		class IFixedJoint3D;
	}
namespace game
{

class MRAY_GAME_DLL PhysicalJointFixedComponent:public IPhysicalJointComponent
{
	DECLARE_RTTI;
protected:

	physics::IPhysicManager* m_phManager;

	physics::IFixedJoint3D* m_joint;

	physics::IPhysicalJointFixedDesc m_jointDesc;

	bool m_removeNodeOnDestroy;

	virtual void _OnTargetChanged();
	void _JointRemoved()
	{
		m_joint=0;
	}
public:
	PhysicalJointFixedComponent(GameEntityManager*mngr);
	PhysicalJointFixedComponent(physics::IPhysicManager* phManager);
	virtual~PhysicalJointFixedComponent();
	

	physics::IPhysicalJoint* GetJoint();
	physics::IFixedJoint3D* GetDOF6Joint(){return m_joint;}
	void SetJoint(physics::IFixedJoint3D* node);

	virtual math::vector3d TransformToJointSpaceAngles(const math::vector3d& angles){return math::vector3d::Zero;}
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

	virtual math::vector3d GetRotationDrive(){return 0;}
	virtual math::vector3d GetPositionDrive(){return 0;}

	void SetFixedJointDesc(const physics::IPhysicalJointFixedDesc& d){m_jointDesc=d;}
	const physics::IPhysicalJointFixedDesc& GetFixedJointDesc(){return m_jointDesc;}

	virtual physics::IPhysicalJoint3DDesc* GetJointDesc(){return &m_jointDesc;}


	xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
	xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
DECLARE_GAMECOMPONENT_FACTORY(PhysicalJointFixedComponent);

}
}

#endif
