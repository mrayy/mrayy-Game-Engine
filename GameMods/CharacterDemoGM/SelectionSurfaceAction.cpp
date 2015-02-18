
#include "stdafx.h"
#include "SelectionSurfaceAction.h"
#include "CHouse.h"
#include "ISceneNode.h"



namespace mray
{
namespace GameMod
{

SelectionSurfaceAction::SelectionSurfaceAction():game::IGameAction(mT("SelectionSurfaceAction"))
{
	m_owner=0;
}
SelectionSurfaceAction::~SelectionSurfaceAction()
{
}

void SelectionSurfaceAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner)
	{
		scene::ISceneNode*node=dynamic_cast<scene::ISceneNode*>(m_owner->GetShape());
		if(!node)
			return;
		scene::SMesh*mesh= node->getMesh();
		if(!mesh )
			return;
		node->updateAbsoluteTransformation();
		CHouse::getInstance().SetSelectorMesh(mesh,&node->getAbsoluteTransformation());
	}
}

void SelectionSurfaceAction::Update(float dt)
{
}


}
}
