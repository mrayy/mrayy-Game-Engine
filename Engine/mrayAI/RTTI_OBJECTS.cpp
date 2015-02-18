#include "StdAfx.h"
#include "IAIArchitect.h"

#include "IAIComponent.h"
#include "AIMovmentTracker.h"
#include "DecisionTree.h"
#include "FiniteStateMachine.h"
#include "Thinking.h"
#include "ActorMemory.h"
#include "SensorActuator.h"
#include "SensorSet.h"
#include "VisionSensor.h"
#include "WorldSensor.h"
#include "IAIPhysics.h"
#include "SteerController.h"
#include "AgentObstacle.h"
#include "PathFindPlanner.h"
#include "RBSComponent.h"
#include "AgentWorldSensor.h"
#include "ForceFieldDOS.h"
#include "FuzzyRaycastDOS.h"
#include "LookAheadDOS.h"
#include "SymbolMemory.h"
#include "AIFuzzyComponent.h"
		  
		  
namespace mray
{
namespace AI
{
IMPLEMENT_RTTI(IAIArchitect,IObject)
IMPLEMENT_RTTI(AIActor,IObject)
IMPLEMENT_RTTI(IAIComponent,IObject)
IMPLEMENT_RTTI(AIMovmentTracker,IAIComponent)
IMPLEMENT_RTTI(DecisionTree,IAIComponent);
IMPLEMENT_RTTI(FiniteStateMachine,IAIComponent)
IMPLEMENT_RTTI(Thinking,IAIComponent)
IMPLEMENT_RTTI(ActorMemory,IAIComponent)
IMPLEMENT_RTTI(SensorActuator,IAIComponent);
IMPLEMENT_RTTI(SensorSet,IAIComponent);
IMPLEMENT_RTTI(VisionSensor,IAIComponent)
IMPLEMENT_RTTI(WorldSensor,IAIComponent);
IMPLEMENT_RTTI(IAIPhysics,IAIComponent)
IMPLEMENT_RTTI(SteerController,IAIComponent)
IMPLEMENT_RTTI(AgentObstacle,IAIComponent)
IMPLEMENT_RTTI(PathFindPlanner,IAIComponent)
IMPLEMENT_RTTI(RBSComponent,IAIComponent)
IMPLEMENT_RTTI(IDynamicObstacleSolver,IAIComponent)
IMPLEMENT_RTTI(SymbolMemory,IAIComponent)
IMPLEMENT_RTTI(AIFuzzyComponent,IAIComponent)
IMPLEMENT_RTTI(IPerceptionSensor,IAIComponent)

IMPLEMENT_RTTI(ForceFieldDOS,IDynamicObstacleSolver)
IMPLEMENT_RTTI(FuzzyRaycastDOS,IDynamicObstacleSolver)
IMPLEMENT_RTTI(LookAheadDOS,IDynamicObstacleSolver)


IMPLEMENT_RTTI(AgentWorldSensor,WorldSensor)



}
}