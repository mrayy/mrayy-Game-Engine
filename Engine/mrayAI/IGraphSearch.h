

/********************************************************************
	created:	2008/12/07
	created:	7:12:2008   12:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IGraphSearch.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IGraphSearch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGraphSearch___
#define ___IGraphSearch___

#include "IGraphLink.h"
#include "ICostFunction.h"

namespace mray{
namespace AI{


	enum ESearchResult
	{
		ESR_NotFound,
		ESR_FullFound,
		ESR_NotComplete
	};

class IGraphSearch
{
public:

	IGraphSearch(){}
	virtual~IGraphSearch(){}

	virtual void SetupForNewSearch(IGraphNode*begin,IGraphNode*end,ICostFunction*h)=0;
	virtual ESearchResult Search(IGraphNode*begin,IGraphNode*end,ICostFunction*h)=0;
	virtual ESearchResult DoSearchStep()=0;
	virtual ESearchResult GetLastState()=0;
	virtual GraphLinksList& GetPath()=0;
	virtual bool GeneratePath()=0;//force to generate what been found sofar
};

}
}

#endif //___IGraphSearch___



