

/********************************************************************
	created:	2012/02/12
	created:	12:2:2012   22:01
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\PhysicsComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	PhysicsComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicsComponent__
#define __PhysicsComponent__

#include "IPhysicsComponent.h"
#include "IPhysicalNode.h"

namespace mray
{
namespace physics
{
	class IPhysicManager;
}
namespace game
{

	class ICollisionComponent;


class MRAY_GAME_DLL PhysicsComponent:public IPhysicsComponent
{
	DECLARE_RTTI;
private:
protected:
	physics::IPhysicManager* m_phManager;
	physics::IPhysicalNode* m_node;
	bool m_removeNodeOnDestroy;

	physics::PhysicalNodeDesc m_nodeDesc;
	physics::PhysicalBodyDesc m_bodyDesc;
	bool m_isDynamic;
	core::string m_physicsModel;

	math::vector3d m_position;
	math::vector3d m_rotation;

	std::list<ICollisionComponent*> m_collShapes;


	void _Rebuild();

	void _OnTargetNodeChanged();

	void _addProperties();
public:
// 	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);
// 	DECLARE_PROPERTY_TYPE(TargetBone,core::string,);
// 	DECLARE_PROPERTY_TYPE(InheritPosition,bool,);
// 	DECLARE_PROPERTY_TYPE(InheritRotation,bool,);

	DECLARE_PROPERTY_TYPE(PhysicsModel,core::string,);
	DECLARE_PROPERTY_TYPE(IsDynamic,bool,);
	DECLARE_PROPERTY_TYPE(Density,float,);
	DECLARE_PROPERTY_TYPE(ActorGroupID,physics::GroupID,);

	DECLARE_PROPERTY_TYPE(Position,math::vector3d,);
	DECLARE_PROPERTY_TYPE(Rotation,math::vector3d,);
public:
	PhysicsComponent(GameEntityManager*mngr);
	PhysicsComponent(physics::IPhysicManager* phManager);
	virtual~PhysicsComponent();

	SetGetFunction(bool,IsDynamic,m_isDynamic,_Rebuild());
	SetGetFunction(float,Density,m_nodeDesc.density,_Rebuild());
	SetGetFunction(physics::GroupID ,ActorGroupID,m_nodeDesc.actorGroupID,_Rebuild());
	SetGetFunction(float ,Mass,m_bodyDesc.mass,_Rebuild());

	virtual bool SetName(const core::string& name);

	bool SetPhysicsModel(const core::string& path);
	const core::string& GetPhysicsModel()
	{
		return m_physicsModel;
	}

	bool InitComponent();

	bool AddComponent(IObjectComponent* comp);
	void RemoveComponent(IObjectComponent* comp);

	void SetRemoveNodeOnDestroy(bool r){m_removeNodeOnDestroy=r;}
	bool IsRemoveNodeOnDestroy()const{return m_removeNodeOnDestroy;}

	void OnAttachedToComponent(IObjectComponent* owner);
	void OnRemovedFromComponent(IObjectComponent* owner);

	virtual bool IsUnique()const{return false;}

	virtual bool SetEnabled(bool enabled);

	physics::IPhysicalNode* GetPhysicalNode(){return m_node;}
	void SetPhysicalNode(physics::IPhysicalNode* node);


	IObject* GetAttachedObject(){return m_node;}

	bool SetPosition(const math::vector3d& v);
	const math::vector3d& GetPosition();

	bool SetRotation(const math::vector3d& v);
	const math::vector3d& GetRotation();

	void Update(float dt);

	physics::PhysicalBodyDesc* GetBodyDesc(){return &m_bodyDesc;}
	physics::PhysicalNodeDesc* GetNodeDesc(){return &m_nodeDesc;}


	xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
	xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
DECLARE_GAMECOMPONENT_FACTORY(PhysicsComponent);


}
}

#endif

