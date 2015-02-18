



/********************************************************************
created:	2013/02/21
created:	21:2:2013   19:09
filename: 	F:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SpringTouchSensorComponent.h
file path:	F:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
file base:	SpringTouchSensorComponent
file ext:	h
author:		MHD Yamen Saraiji
	
purpose:	
*********************************************************************/

#ifndef __SpringTouchSensorComponent__
#define __SpringTouchSensorComponent__


#include "IVTComponent.h"
#include "IFeedbackControl.h"


namespace mray
{
	namespace physics
	{
		class IPhysicUserContactReport;
		class PhysicContactPair;
		class IPhysicalNode;
	}
namespace game
{
	class PhysicsComponent;
}
namespace VT
{
	class SpringTouchSensorComponentImpl;
class SpringTouchSensorComponent:public IVTComponent,public IFeedbackControl
{
	DECLARE_RTTI;
protected:
	SpringTouchSensorComponentImpl* m_data;

	void _ChangeStatus(physics::IPhysicalNode* node,bool on);
public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);
	DECLARE_PROPERTY_TYPE(TargetShape,core::string,);
	DECLARE_PROPERTY_TYPE(TargetGG,core::string,);
	DECLARE_PROPERTY_TYPE(ModulatorPath,core::string,);

	DECLARE_PROPERTY_TYPE(Channel,int,);
public:
	SpringTouchSensorComponent(game::GameEntityManager*);
	virtual ~SpringTouchSensorComponent();

	virtual bool InitComponent();

	bool SetTargetNodeName(const core::string& name);
	const core::string& GetTargetNodeName();

	bool SetTargetShapeName(const core::string& name);
	const core::string& GetTargetShapeName();

	bool SetModulatorPath(const core::string& name);
	const core::string& GetModulatorPath();

	bool SetTargetGG(const core::string& name);
	const core::string& GetTargetGG();


	bool SetChannel(int p);
	int GetChannel();
	game::PhysicsComponent* GetTargetNode();

	virtual void PreUpdate();
	virtual void Update_old(float dt);
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

	virtual void OnDestroy();

	core::string GetDebugString();

	IVTComponent* GetSensorComponent(){return this;}
	//callbacks from the reporter
	void _OnStartTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnEndTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnSlide(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
};
DECLARE_GAMECOMPONENT_FACTORY(SpringTouchSensorComponent);

}
}

#endif
