

/********************************************************************
	created:	2009/03/08
	created:	8:3:2009   16:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\INodeSelectQuery.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	INodeSelectQuery
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___INodeSelectQuery___
#define ___INodeSelectQuery___

#include "GCPtr.h"



namespace mray{
namespace scene{

	class IMovable;

	struct QueryResultNode
	{
		QueryResultNode(){
			dist=0;
			node=0;
		}
		QueryResultNode(IMovable* n,float d){
			node=n;
			dist=d;
		}
		IMovable* node;
		float dist;
	};
	typedef std::list<QueryResultNode> NodesQueryList;

class INodeSelectQuery
{
public:
	INodeSelectQuery(){}
	virtual~INodeSelectQuery(){}

	virtual QueryResultNode query(NodesQueryList&outList)=0;
	virtual const NodesQueryList&  query(QueryResultNode&best)=0;
};

}
}


#endif //___INodeSelectQuery___
