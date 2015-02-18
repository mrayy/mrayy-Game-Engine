
#include "stdafx.h"
#include "AIFoodArch.h"

#include "AIActor.h"


namespace mray
{
namespace GameMod
{

AIFoodArch::AIFoodArch(AI::AIActor*owner):AIObstacleObject(owner,EAT_Food)
{
}
AIFoodArch::~AIFoodArch()
{
}

void AIFoodArch::Update(float dt)
{
	AIObstacleObject::Update(dt);
}

bool AIFoodArch::OnMessageTelegram(const AI::AIMessageTelegram& msg)
{
	return false;
}


}
}