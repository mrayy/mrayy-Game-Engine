
/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   21:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavAStarSearch.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavAStarSearch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavAStarSearch___
#define ___NavAStarSearch___

#include "IGraphSearch.h"
#include "IGraphNode.h"
#include <vector>

namespace mray{
namespace AI{

class NavMesh;

class NavAStarSearch:public IGraphSearch
{
private:
protected:
	NavMesh* m_world;
	GraphNodesList m_openList;
	std::vector<bool> m_openListDirect;

	IGraphNode* m_begin;
	IGraphNode* m_end;
	ICostFunction* m_costFunc;

	GraphNodesList m_closeList;
	std::vector<bool> m_closeListDirect;
	std::vector<float> m_costs;
	std::vector<float> m_actualCosts;
	std::vector<IGraphLink*> m_parents;

	GraphLinksList m_path;

	ESearchResult m_lastRes;

	void cleanUp();
	void insertToList(GraphNodesList&l,IGraphNode*node);
	void removeFromList(GraphNodesList&l,IGraphNode*node);

	void createPath(int begin,int end);

public:
	NavAStarSearch(NavMesh*world);
	virtual~NavAStarSearch();

	virtual void SetupForNewSearch(IGraphNode*begin,IGraphNode*end,ICostFunction*h);
	virtual ESearchResult Search(IGraphNode*begin,IGraphNode*end,ICostFunction*h);

	virtual ESearchResult DoSearchStep();
	virtual ESearchResult GetLastState();

	bool GeneratePath();

	GraphLinksList& GetPath(){return m_path;}
};

}
}


#endif //___NavAStarSearch___
