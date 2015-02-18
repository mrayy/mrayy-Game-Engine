
#include "stdafx.h"
#include "CameraStartAction.h"
#include "CHouse.h"
#include "GameLevel.h"
#include "GameEntityManager.h"



namespace mray
{
namespace GameMod
{

CameraStartAction::CameraStartAction():game::IGameAction(mT("CameraStartAction"))
{
	m_switched=false;
}
CameraStartAction::~CameraStartAction()
{
}

void CameraStartAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(ent)
	{
		m_startPos=m_owner->GetShape()->getPosition();
		m_startRot=m_owner->GetShape()->getOrintation();
	}
}

void CameraStartAction::Update(float dt)
{
	if(m_owner){
		if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode()){
			m_startPos=m_owner->GetShape()->getPosition();
			m_startRot=m_owner->GetShape()->getOrintation();
			m_switched=false;
			((scene::IRenderable*)m_owner->GetShape())->setVisible(true);
		}
		else{
			if(!m_switched){
				m_switched=true;
				((scene::IRenderable*)m_owner->GetShape())->setVisible(false);
				scene::SCameraNode*cam= CHouse::getInstance().GetCamera();
				if(cam)
				{
					cam->position=m_startPos;
					cam->setOrintation(m_startRot);
				}
			}
		}
	}
}


}
}
