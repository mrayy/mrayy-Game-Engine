

/********************************************************************
	created:	2008/12/06
	created:	6:12:2008   21:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\mrayAI\IGraphLink.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI\mrayAI
	file base:	IGraphLink
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGraphLink___
#define ___IGraphLink___

#include <mString.h>
#include "compileConfig.h"



namespace mray{
namespace AI{

class IGraphNode;

class MRAY_AI_DLL IGraphLink
{

public:
	int ID;
	//core::string Name; 

	IGraphNode* m_begin;
	IGraphNode* m_end;

	IGraphLink();
	IGraphLink(IGraphNode* begin,IGraphNode* end,int id/*,const core::string &name*/);

	virtual~IGraphLink(){}

	virtual bool isBidirectional()=0;
	virtual float getWeightBeginEnd()=0;
	virtual float getWeightEndBegin()=0;

	IGraphNode* getNode(int id);
	bool isBegin(IGraphNode* node);
	IGraphNode*getOtherNode(int id);
	IGraphNode*getOtherNode(IGraphNode* a);
};

typedef std::list<IGraphLink*> GraphLinksList;

}
}

#endif //___IGraphLink___
