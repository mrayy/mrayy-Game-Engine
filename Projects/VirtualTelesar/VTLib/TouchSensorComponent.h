

/********************************************************************
	created:	2012/08/07
	created:	7:8:2012   1:12
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\TouchSensorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	TouchSensorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___TouchSensorComponent___
#define ___TouchSensorComponent___


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

	//feedback result type
	class TouchSensorFBType
	{
	public:
		math::vector3d point;//touch point
		math::vector3d normal;//touch normal
		float force;//touch force
	};
	class TouchSensorFeedback:public IFeedbackResult
	{
	public:
		TouchSensorFBType value;
		void* GetData()
		{
			return &value;
		}
		core::string ToString()
		{
			core::string ret=mT("[")+core::StringConverter::toString(value.point)+mT("]");
			ret+=mT("[")+core::StringConverter::toString(value.normal)+mT("]");
			ret+=mT("[")+core::StringConverter::toString(value.force)+mT("]");
			return ret;
		}
	};

class TouchSensorComponent:public IVTComponent,public IFeedbackControl
{
	DECLARE_RTTI;
public:

protected:
	game::GameEntityManager* m_mngr;
	physics::IPhysicUserContactReport* m_reporter;

	game::PhysicsComponent* m_targetNode;
	core::string m_targetNodeName;
	core::string m_targetShapeName;

	std::vector<TouchSensorFBType> m_feedback;
public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);
	DECLARE_PROPERTY_TYPE(TargetShape,core::string,);

public:
	TouchSensorComponent(game::GameEntityManager*);
	virtual~TouchSensorComponent();

	virtual bool InitComponent();
	
	bool SetTargetNodeName(const core::string& name);
	const core::string& GetTargetNodeName();

	bool SetTargetShapeName(const core::string& name);
	const core::string& GetTargetShapeName();

	game::PhysicsComponent* GetTargetNode(){return m_targetNode;}

	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

	virtual void OnDestroy();

	IVTComponent* GetSensorComponent(){return this;}
// 	void GetFeedbackResult(std::vector<IFeedbackResult*>& lst);
// 	void ClearFeedbackResults();


public:
	//callbacks from the reporter
	void _OnStartTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnEndTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
	void _OnSlide(physics::PhysicContactPair* pair,physics::IPhysicalNode* other);
};
DECLARE_GAMECOMPONENT_FACTORY(TouchSensorComponent);

}
}

#endif