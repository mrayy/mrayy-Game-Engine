

/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   11:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IGoalFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IGoalFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGoalFactory_h__
#define IGoalFactory_h__

#include "CompileConfig.h"

namespace mray
{
namespace AI
{
	class IGoal;
	class IAIArchitect;

class MRAY_AI_DLL IGoalFactory
{
private:
protected:
public:
	IGoalFactory(){}
	virtual ~IGoalFactory(){}
	
	virtual core::string GetType()=0;
	virtual IGoal* CreateGoal(IAIArchitect*arch)=0;
};



}
}

#endif // IGoalFactory_h__
