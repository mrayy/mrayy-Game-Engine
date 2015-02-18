
/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   20:23
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\SenarioState.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	SenarioState
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SenarioState___
#define ___SenarioState___

#include <IState.h>
#include <GCPtr.h>
#include "IScenarioNode.h"

namespace mray{
namespace gameMod{

class SenarioState:public IState
{
private:
protected:
	GCPtr<IScenarioNode> m_node;
public:
	SenarioState(GCPtr<IScenarioNode> node,const core::string&name);
	virtual~SenarioState();

	GCPtr<IScenarioNode> getNode(){return m_node;}

	virtual void onEnter(IState*prev);
	virtual void onUpdate();
	virtual void onExit();
};

}
}


#endif //___SenarioState___
