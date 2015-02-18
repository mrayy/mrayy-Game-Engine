


/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   11:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GoalScenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GoalScenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalScenario_h__
#define GoalScenario_h__

#include "GoalPipe.h"
#include "XMLElement.h"
#include <GenericValues.h>
#include "IGoalFactory.h"

namespace mray
{
namespace AI
{
	class IAIArchitect;

class MRAY_AI_DLL GoalScenario:public GoalPipe
{
private:
protected:
	int m_id;
	core::string m_type;
	EGoalStatus m_status;

	StringValue m_requiredScenario;
	IAIArchitect* m_arch;

	IGoal* LoadGoal(xml::XMLElement*e);

	bool LoadScenario(const core::string& file);
public:
	GoalScenario(AI::IAIArchitect*arch,const core::string&scenarioToLoad);
	virtual ~GoalScenario();


	virtual void activate();
	virtual EGoalStatus process();

	virtual core::string getType();
	virtual int GetGoalType();
	virtual EGoalStatus getStatus();

	void loadXMLSettings(xml::XMLElement*e);
};


class MRAY_AI_DLL GoalScenarioFactory:public IGoalFactory
{
public:
	virtual core::string GetType()
	{
		return mT("Scenario");
	}
	virtual IGoal* CreateGoal(IAIArchitect*arch)
	{
		return new GoalScenario(arch,mT(""));
	}
};

}
}

#endif // GoalScenario_h__
