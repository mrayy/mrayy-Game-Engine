
#include "stdafx.h"
#include "AIObstacleObject.h"
#include "AIActor.h"


namespace mray
{
namespace GameMod
{

AIObstacleObject::AIObstacleObject(AI::AIActor*owner):AIGameArch(owner,EAT_Obstacle)
{
	Init();
}
AIObstacleObject::AIObstacleObject(AI::AIActor*owner,EAgentType type):AIGameArch(owner,type)
{
	Init();
}
AIObstacleObject::~AIObstacleObject()
{
	delete m_obstacle;
}


void AIObstacleObject::Init()
{
	m_obstacle=new AI::AgentObstacle(m_owner->GetPhysicalProp(),0);
	AddComponent(m_obstacle);
	AddComponent(m_owner->GetPhysicalProp());
}

AI::AgentObstacle* AIObstacleObject::GetObstacle()
{
	return m_obstacle;
}

void AIObstacleObject::Update(float dt)
{
	AIGameArch::Update(dt);
}

bool AIObstacleObject::OnMessageTelegram(const AI::AIMessageTelegram& msg)
{
	return false;
}

}
}

