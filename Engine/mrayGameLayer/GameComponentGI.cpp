

#include "stdafx.h"
#include "GameComponentGI.h"
#include "IGameComponent.h"
#include "StreamWriter.h"




namespace mray
{
namespace game
{

GameComponentGI::GameComponentGI(IGameComponent* owner):IGhostInterface(owner->getObjectType()),m_owner(owner)
{
}
GameComponentGI::~GameComponentGI()
{
}

void GameComponentGI::CreateWrite(OS::StreamWriter* stream)
{
	IGameComponent* parent= dynamic_cast<IGameComponent*>(m_owner->GetOwnerComponent());
	(*stream)<<m_owner->GetID()<<(parent? parent->GetID():0);
}


}
}
