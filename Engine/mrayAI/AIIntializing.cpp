#include "stdafx.h"

#include "AIIntializing.h"
#include "SteerController.h"

#include "GoalFactoryManager.h"
#include <EnumManager.h>


#include "GoalScenario.h"

namespace mray
{
namespace AI
{


bool AIIntializing::m_isInitiated=false;

void AIIntializing::InitAI()
{
	if(m_isInitiated)
		return;
	m_isInitiated=true;

	new GoalFactoryManager();
	GoalFactoryManager::getInstance().AddFactory(new GoalScenarioFactory());

	StartRegisterEnum(ESteeringTypes);
	RegisterEnumValueA(mT("Seek"),EST_Seek);
	RegisterEnumValueA(mT("Flee"),EST_Flee);
	RegisterEnumValueA(mT("Arrive"),EST_Arrive);
	RegisterEnumValueA(mT("Pursuit"),EST_Pursuit);
	RegisterEnumValueA(mT("Pursuit"),EST_Pursuit);
	RegisterEnumValueA(mT("Hide"),EST_Hide);
	RegisterEnumValueA(mT("Wander"),EST_Wander);
	RegisterEnumValueA(mT("Seperation"),EST_Seperation);
	RegisterEnumValueA(mT("Align"),EST_Align);
	RegisterEnumValueA(mT("Cohesion"),EST_Cohesion);
	EndRegisterEnum();
}
void AIIntializing::ShutdownAI()
{
	if(!m_isInitiated)
		return;
	m_isInitiated=false;


	delete GoalFactoryManager::getInstancePtr();
	EnumManager::getInstance().removeEnum(mT("ESteeringTypes"));
}


}
}

