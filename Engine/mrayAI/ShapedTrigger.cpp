#include "stdafx.h"

#include "ShapedTrigger.h"
#include "AISystem.h"
#include "TriggerSystem.h"


namespace mray{
namespace AI{

ShapedTrigger::ShapedTrigger()
:m_shape(0)
{
}
ShapedTrigger::~ShapedTrigger()
{
}

void ShapedTrigger::SetShape(ITriggerShape*shape)
{
	m_shape=shape;
}
ITriggerShape* ShapedTrigger::GetShape()
{
	return m_shape;
}

void ShapedTrigger::TestTrigger()
{
	if(!m_shape)return;

	AIActorList actors;
	m_system->GetAISystem()->QueryActors(m_shape->GetAABB(),actors);
	AIActorList::iterator it=actors.begin();
	for(;it!=actors.end();++it){
		AIActor*actor=*it;
		if(m_shape->IsActorInside(actor)){
			if(TestCondition(actor)){
				Execute(actor);
			}
		}
	}
}


}
}
