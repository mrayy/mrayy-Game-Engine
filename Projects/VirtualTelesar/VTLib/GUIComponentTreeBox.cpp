

#include "stdafx.h"
#include "GUIComponentTreeBox.h"
#include "GameEntity.h"
#include "IGameComponent.h"



namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUIComponentTreeBox,GUITreeBox);

GUIComponentTreeBox::GUIComponentTreeBox(IGUIManager*mngr)
	:GUITreeBox(mngr),m_ent(0)
{
}
GUIComponentTreeBox::~GUIComponentTreeBox()
{
}

void GUIComponentTreeBox::_AddNode(GUITreeNode* node,game::IGameComponent* c)
{
	if(!c)
		return;
	GUITreeNode* n=0;
	if(node)
	{
		n=node->AddItem(c->GetName());
	}else
		n=AddItem(c->GetName());

	const std::list<IObjectComponent*>& comps=c->GetComponentsList();
	std::list<IObjectComponent*>::const_iterator it=comps.begin();
	for (;it!=comps.end();++it)
	{
		_AddNode(n,dynamic_cast<game::IGameComponent*>(*it));
	}

}

void GUIComponentTreeBox::SetEntity(game::GameEntity* e)
{
	ClearItems();
	m_ent=e;
	if(!m_ent)
		return;
	const std::list<IObjectComponent*>& comps=e->GetComponentsList();
	std::list<IObjectComponent*>::const_iterator it=comps.begin();
	for (;it!=comps.end();++it)
	{
		_AddNode(0,dynamic_cast<game::IGameComponent*>(*it));
	}
}

void GUIComponentTreeBox::AddImageSet(video::ImageSetCRef imageSet)
{
	m_imageSets.push_back(imageSet);
}

}
}

