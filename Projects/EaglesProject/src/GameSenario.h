
/********************************************************************
	created:	2009/04/08
	created:	8:4:2009   23:53
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\GameSenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	GameSenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameSenario___
#define ___GameSenario___

#include "IScenarioNode.h"
#include "GameLevel.h"

namespace mray{
namespace gameMod{

class GameSenario:public IScenarioNode
{
private:
protected:
	GCPtr<GameLevel> m_gameLevel;
public:
	GameSenario();
	virtual~GameSenario();

	virtual void update(float dt);

	virtual void onEvent(GCPtr<Event> event);

	void setLevel(GCPtr<GameLevel> gameLevel);

	virtual int isDone();

	virtual void onEnter();

	virtual void onExit();
};

}
}

#endif //___GameSenario___
