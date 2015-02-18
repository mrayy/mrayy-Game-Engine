
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   15:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IAISpacePartition.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IAISpacePartition
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	divide the space for agents query
*********************************************************************/

#ifndef ___IAISpacePartition___
#define ___IAISpacePartition___

#include <box3d.h>
#include "AIActor.h"

namespace mray{
namespace AI{

class IRenderDevice;

class IAISpacePartition
{
private:
protected:
public:
	IAISpacePartition(){}
	virtual~IAISpacePartition(){}

	virtual void SetWorldAABB(const math::box3d& box)=0;

	virtual void AddActor(AIActor*actor)=0;
	virtual void RemoveActor(AIActor*actor)=0;

	virtual void Update()=0;

	virtual void QueryActors(const math::box3d& box,AIActorList&actors)=0;
	virtual void Visualize(IRenderDevice*dev)=0;

};

}
}


#endif //___IAISpacePartition___
