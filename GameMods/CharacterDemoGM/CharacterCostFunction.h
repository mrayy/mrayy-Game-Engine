


/********************************************************************
	created:	2010/07/19
	created:	19:7:2010   21:45
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CharacterCostFunction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CharacterCostFunction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CharacterCostFunction_h__
#define CharacterCostFunction_h__

#include "ICostFunction.h"
#include "INavWorld.h"

namespace mray
{
namespace GameMod
{

class CharacterCostFunction:public AI::ICostFunction
{
private:
protected:
	std::map<int,float> m_weightMap;
	AI::INavWorld* m_navWorld;
public:
	CharacterCostFunction(AI::INavWorld* navWorld);
	virtual ~CharacterCostFunction();

	void ChangeWeight(const core::string& attr,float weight);

	virtual float getHeuristic(AI::IGraphNode*a,AI::IGraphNode*b);
	virtual float getWeight(AI::IGraphLink*link);
};

}
}

#endif // CharacterCostFunction_h__
