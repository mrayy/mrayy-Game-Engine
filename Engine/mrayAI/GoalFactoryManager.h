
/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   11:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GoalFactoryManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GoalFactoryManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalFactoryManager_h__
#define GoalFactoryManager_h__


#include "ISingleton.h"
#include "CompileConfig.h"

#include <map>

namespace mray
{
namespace AI
{
	class IGoalFactory;
	class IGoal;
	class IAIArchitect;

class MRAY_AI_DLL GoalFactoryManager:public ISingleton<GoalFactoryManager>
{
private:
protected:
	typedef std::map<core::string,IGoalFactory*> FactoryMap;
	FactoryMap m_factories;
public:
	GoalFactoryManager();
	virtual ~GoalFactoryManager();

	void ClearFactories();

	IGoal* CreateGoal(const core::string&name,IAIArchitect*arch);

	void AddFactory(IGoalFactory* f);
	void RemoveFactory(const core::string&name);
};

}
}

#endif // GoalFactoryManager_h__