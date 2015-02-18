
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   16:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ShapedTrigger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ShapedTrigger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ShapedTrigger___
#define ___ShapedTrigger___

#include "ITrigger.h"
#include "ITriggerShape.h"

namespace mray{
namespace AI{

	class AIActor;

class ShapedTrigger:public ITrigger
{
private:
protected:
	ITriggerShape* m_shape;

	virtual bool TestCondition(AIActor*actor)=0;
	virtual void Execute(AIActor*actor)=0;
public:
	ShapedTrigger();
	virtual~ShapedTrigger();

	void SetShape(ITriggerShape*shape);
	ITriggerShape* GetShape();

	virtual void TestTrigger();
};

}
}

#endif //___ShapedTrigger___
