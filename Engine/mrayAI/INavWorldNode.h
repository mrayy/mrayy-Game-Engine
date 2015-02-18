
/********************************************************************
	created:	2010/03/08
	created:	8:3:2010   13:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\INavWorldNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	INavWorldNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___INavWorldNode___
#define ___INavWorldNode___

#include "Point3d.h"


namespace mray{
namespace AI{

	class IGraphNode;

class INavWorldNode;
typedef std::list<INavWorldNode> NavWorldNodeList;
typedef std::list<INavWorldNode*> NavWorldNodeListPtr;

class INavWorldNode
{
private:
protected:
public:

	INavWorldNode(){}
	virtual~INavWorldNode(){}

	virtual const math::vector3d& GetPosition()=0;
	virtual IGraphNode* GetGraphNode()=0;
	virtual IGraphNode* GetNearestGraphNode(const math::vector3d& pos)=0;
	virtual NavWorldNodeListPtr& GetAdjacents()=0;
	virtual int GetAttribute()=0;
};


}
}


#endif //___INavWorldNode___
