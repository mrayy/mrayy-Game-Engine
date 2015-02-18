

/********************************************************************
	created:	2012/08/01
	created:	1:8:2012   15:23
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IPhysicalJointComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IPhysicalJointComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IPhysicalJointComponent___
#define ___IPhysicalJointComponent___



#include "IGameComponent.h"

namespace mray
{
	namespace physics
	{
		class IPhysicalJoint;
		class IPhysicalJoint3DDesc;
	}
namespace game
{

	class IPhysicsComponent;
class MRAY_GAME_DLL IPhysicalJointComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	IPhysicsComponent* m_targetComp1;
	IPhysicsComponent* m_targetComp2;

	bool m_removeNodeOnDestroy;

	core::string m_targetNode1;
	core::string m_targetNode2;


	virtual void _OnTargetChanged();
	virtual void _JointRemoved()=0;

public:
	DECLARE_PROPERTY_TYPE(TargetNode1,core::string,);
	DECLARE_PROPERTY_TYPE(TargetNode2,core::string,);
public:
	IPhysicalJointComponent();
	virtual~IPhysicalJointComponent();

	virtual bool InitComponent();

	virtual physics::IPhysicalJoint3DDesc* GetJointDesc()=0;

	virtual math::vector3d GetCurrentAngles()=0;
	virtual float GetCurrentAngle(int axis)=0;
	virtual float GetCurrentLinearDistance(int axis)=0;

	virtual void SetRotationDrive(const math::vector3d& angles)=0;
	virtual math::vector3d TransformToJointSpaceAngles(const math::vector3d& angles)=0;
	virtual void SetTwistDrive(float v)=0;
	virtual void SetSwing1Drive(float v)=0;
	virtual void SetSwing2Drive(float v)=0;

	virtual void SetPositionDrive(const math::vector3d& angles)=0;
	virtual void SetXDrive(float v)=0;
	virtual void SetYDrive(float v)=0;
	virtual void SetZDrive(float v)=0;


	virtual void OnAttachedToComponent(IObjectComponent* owner);
	virtual void OnRemovedFromComponent(IObjectComponent* owner);

	virtual bool SetTargetNameA(const core::string& name);
	const core::string& GetTargetNameA()const;

	virtual bool SetTargetNameB(const core::string& name);
	const core::string& GetTargetNameB()const;

	virtual IObject* GetAttachedObject();

	IPhysicsComponent* GetTargetComponentA(){return m_targetComp1;}
	IPhysicsComponent* GetTargetComponentB(){return m_targetComp2;}

	virtual physics::IPhysicalJoint* GetJoint()=0;
};

}
}

#endif
