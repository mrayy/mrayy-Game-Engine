
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   20:27
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalsTypeEnum.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalsTypeEnum
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalsTypeEnum___
#define ___GoalsTypeEnum___

#include "AIDefaultGoalsTypes.h"

namespace mray{
namespace AI{

enum
{
	EGoal_StartID=EGoal_NumberOfGoals,
	EGoal_Steer,
	EGoal_FollowPath,
	EGoal_GoToPoint,
	EGoal_TraverseEdge,

	EGoal_Hide,

	EGoal_Wait,

	EGoal_TestAgentGoalsCount
};

}
}


#endif //___GoalsTypeEnum___
