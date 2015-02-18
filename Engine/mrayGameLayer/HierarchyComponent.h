

/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   23:18
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\HierarchyComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	HierarchyComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___HierarchyComponent___
#define ___HierarchyComponent___

#include "IGameComponent.h"

namespace mray
{
namespace game
{
	class IPhysicalJointComponent;
	class IPhysicsComponent;
	class IPhysicalJointComponent;

class HierarchyComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:

	struct AttachmentInfo;

	struct ChainInfo
	{
		ChainInfo()
		{
			next=0;
			link=0;
		}
		~ChainInfo()
		{
			delete next;
		}
		AttachmentInfo* link;
		ChainInfo* next;
	};

	struct AttachmentInfo
	{
		AttachmentInfo()
		{
			start=0;
			joint=0;
			parent=0;
		}
		~AttachmentInfo()
		{
			std::list<ChainInfo*>::iterator it= children.begin();
			for(;it!=children.end();++it)
				delete *it;
		}
		AttachmentInfo* parent;
		IPhysicalJointComponent* joint;
		IPhysicsComponent* comp;
		std::list<ChainInfo*> children;
		ChainInfo* start;
	};
	/*
	struct GUIElementInfo
	{
		IGameComponent* comp;
		GUI::IGUIElement* elem;
	};
	std::vector<GUIElementInfo> m_guiElements;*/
	typedef std::map<IPhysicsComponent*,AttachmentInfo*> AttachmentMap;
	AttachmentMap m_attachments;

	bool m_innerCollision;

	//std::vector<game::IPhysicalJointComponent*> m_joints;

	std::vector<game::IPhysicsComponent*> m_physList;


	game::GameEntityManager* m_mngr;
//	void _Update(AttachmentInfo*ifo);
public:
	HierarchyComponent(game::GameEntityManager*mngr);
	HierarchyComponent();
	virtual~HierarchyComponent();

	virtual bool InitComponent();
	IPhysicsComponent* InsertPair(IPhysicalJointComponent* joint);

	void SetInited(){m_inited=true;}

//	virtual void Update(float dt);
	
//	void ClearGUIElements();
//	void GenerateGUIElements(GUI::IGUIManager*mngr);

	bool SetInnerCollision(bool s){m_innerCollision=s;return true;}
	bool GetInnerCollision(){return m_innerCollision;}
};
DECLARE_GAMECOMPONENT_FACTORY(HierarchyComponent);

}
}

#endif
