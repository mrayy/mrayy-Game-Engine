
/********************************************************************
	created:	2008/12/06
	created:	6:12:2008   21:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\mrayAI\IGraphNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI\mrayAI
	file base:	IGraphNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGraphNode___
#define ___IGraphNode___


#include <mString.h>
#include "compileConfig.h"
#include "IGraphLink.h"

namespace mray{
namespace AI{


class IGraphNode
{
protected:
	uint m_ID;
//	core::string m_Name; 
public:
	GraphLinksList links;


	IGraphNode(uint id/*,const core::string&name*/){
		m_ID=id;
		//m_Name=name;
	}
	virtual~IGraphNode(){}

	virtual uint getID(){return m_ID;}
	virtual void setID(uint id){m_ID=id;}
 
// 	virtual const core::string& getName(){return m_Name;}
// 	virtual void setName(const core::string& name){m_Name=name;}
};

typedef std::list<IGraphNode*> GraphNodesList;

}
}


#endif //___IGraphNode___
