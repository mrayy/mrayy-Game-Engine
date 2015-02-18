
#include "stdafx.h"
#include "AIBallArchitecture.h"


namespace mray
{
namespace Ball
{


AIBallArchitecture::AIBallArchitecture(game::GameEntity*entity):m_entity(entity),m_owner(m_entity->GetAIActor())
{
	m_obstacle=new AI::AgentObstacle(m_owner->GetPhysicalProp());
}
AIBallArchitecture::~AIBallArchitecture()
{
}

AI::AgentObstacle* AIBallArchitecture::GetObstacle()
{
}

 bool AIBallArchitecture::IsAIControlled()
 {
 }

 bool AIBallArchitecture::OnMessageTelegram(const AI::AIMessageTelegram&msg)
 {
 }


}
}


