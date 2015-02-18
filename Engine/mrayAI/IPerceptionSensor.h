
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   15:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IPerceptionSensor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IPerceptionSensor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPerceptionSensor___
#define ___IPerceptionSensor___

#include "IAIComponent.h"

namespace mray{
namespace AI{

	class AIActor;

class IPerceptionSensor:public IAIComponent
{
	DECLARE_RTTI;
private:
protected:
public:
	IPerceptionSensor()
	{}
	virtual~IPerceptionSensor(){}

	virtual void Update(float dt)=0;

	virtual AIActor* GetOwnerAgent()=0;

};

}
}


#endif //___IPerceptionSensor___
