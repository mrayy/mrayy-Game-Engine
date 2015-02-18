
#include "stdafx.h"
#include "CharacterCostFunction.h"

#include "NavGraphLink.h"
#include "NavMeshPolygon.h"

namespace mray
{
namespace GameMod
{

CharacterCostFunction::CharacterCostFunction(AI::INavWorld* navWorld)
{
	m_navWorld=navWorld;
}
CharacterCostFunction::~CharacterCostFunction()
{
}

void CharacterCostFunction::ChangeWeight(const core::string& attr,float weight)
{
	if(m_navWorld)
	{
		int id=m_navWorld->GetAttributeID(attr);
		m_weightMap[id]=weight;
	}
}

float CharacterCostFunction::getHeuristic(AI::IGraphNode*a,AI::IGraphNode*b)
{
	AI::NavGraphNode* na=(AI::NavGraphNode*)a;
	AI::NavGraphNode* nb=(AI::NavGraphNode*)b;

	return na->position.getDist(nb->position);
}
float CharacterCostFunction::getWeight(AI::IGraphLink*link)
{
	AI::NavGraphLink*nLink=(AI::NavGraphLink*)link;
	std::map<int,float>::iterator it=m_weightMap.find(nLink->getOwnerNode()->GetAttribute());
	if(it==m_weightMap.end())
		return 1;
	return it->second;
}

}
}
