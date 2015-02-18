

/********************************************************************
	created:	2010/05/06
	created:	6:5:2010   12:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IActorQuery.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IActorQuery
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IActorQuery_h__
#define IActorQuery_h__

#include "AIActor.h"
#include "AgentObstacle.h"

namespace mray
{
namespace AI
{

class IActorQuery
{
private:
protected:
public:
	IActorQuery(){}
	virtual ~IActorQuery(){}
	
	virtual const AIActorList& GetActorsList()=0;
	virtual const ObstacleList& GetObstaclesList()=0;
};

}
}

#endif // IActorQuery_h__
