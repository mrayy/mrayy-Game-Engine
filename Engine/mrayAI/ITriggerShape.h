
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   16:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ITriggerShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ITriggerShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ITriggerShape___
#define ___ITriggerShape___

#include <Box3D.h>

namespace mray{
namespace AI{

	class AIActor;

class ITriggerShape
{
private:
protected:
public:
	ITriggerShape();
	virtual~ITriggerShape();

	virtual const math::box3d& GetAABB()=0;
	virtual bool IsActorInside(AIActor*actor)=0;

};

}
}

#endif //___ITriggerShape___
