
/********************************************************************
	created:	2013/06/04
	created:	4:6:2013   15:18
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\BoneComponent.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	BoneComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __BoneComponent__
#define __BoneComponent__



#include "IMovableComponent.h"


namespace mray
{
namespace scene
{
	class BoneNode;
}
namespace game
{

class MRAY_GAME_DLL BoneComponent:public IMovableComponent
{
	DECLARE_RTTI;
private:
protected:
	core::string m_targetNode;
	core::string m_ownerNode;
	core::string m_boneName;
	scene::BoneNode* m_attachedBone;
public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);
	DECLARE_PROPERTY_TYPE(OwnerNode,core::string,);
	DECLARE_PROPERTY_TYPE(BoneName,core::string,);
public:
	BoneComponent(GameEntityManager*);
	virtual~BoneComponent();

	virtual bool InitComponent();
	
	bool SetTargetNode(const core::string& name);
	const core::string& GetTargetNode(){return m_targetNode;}

	bool SetOwnerNode(const core::string& name);
	const core::string& GetOwnerNode(){return m_ownerNode;}

	bool SetBoneName(const core::string& name);
	const core::string& GetBoneName(){return m_boneName;}

	virtual IObject* GetAttachedObject();
	virtual scene::IMovable* GetMovableObject();
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

};

DECLARE_GAMECOMPONENT_FACTORY(BoneComponent);


}
}

#endif
