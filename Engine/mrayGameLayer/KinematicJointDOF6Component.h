

/********************************************************************
	created:	2013/01/25
	created:	25:1:2013   14:49
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\KinematicJointDOF6Component.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	KinematicJointDOF6Component
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __KinematicJointDOF6Component__
#define __KinematicJointDOF6Component__


#include "IPhysical6DOFJointComponent.h"
#include "IPhysical3DDef.h"


namespace mray
{
namespace game
{

class MRAY_GAME_DLL KinematicJointDOF6Component:public IPhysical6DOFJointComponent
{
	DECLARE_RTTI
protected:


	physics::IPhysicalJointD6Desc m_jointDesc;
	IPhysicsComponent* m_parent;
	IPhysicsComponent* m_child;
	math::vector3d m_rotationDrive;
	math::vector3d m_positionDrive;

	math::matrix4x4 m_mat;

	void _JointRemoved()
	{
	}
	virtual void _OnChangeAngularDrive();
	virtual void _OnChangePositionDrive();

	virtual void _OnTargetChanged();

public:
	KinematicJointDOF6Component(GameEntityManager*mngr);
	virtual~KinematicJointDOF6Component();

	virtual math::vector3d GetCurrentAngles();
	virtual float GetCurrentAngle(int axis);
	virtual float GetCurrentLinearDistance(int axis);

	virtual math::vector3d TransformToJointSpaceAngles(const math::vector3d& angles);
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

	virtual bool InitComponent();

	virtual void Update(float dt);
	
	virtual xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);

	virtual physics::IPhysicalJoint3DDesc* GetJointDesc(){return &m_jointDesc;}


	virtual physics::IPhysicalJoint* GetJoint(){return 0;};

};

DECLARE_GAMECOMPONENT_FACTORY(KinematicJointDOF6Component);

}
}


#endif
