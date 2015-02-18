

#include "stdafx.h"
#include "JFHumanController.h"

#include "JFSpaceShip.h"

namespace mray
{
namespace game
{

bool EventActionPair::Process(KeyboardEvent* e,JFHumanController* c)
{
	if(e->key==m_key)
	{
		c->DoAction(m_action,e->press);
		return true;
	}
	return false;
}


JFHumanController::JFHumanController()
{
	m_actions.push_back(EventActionPair(KEY_UP,EJF_Up));
	m_actions.push_back(EventActionPair(KEY_DOWN,EJF_Down));
	m_actions.push_back(EventActionPair(KEY_LEFT,EJF_Left));
	m_actions.push_back(EventActionPair(KEY_RIGHT,EJF_Right));
	m_actions.push_back(EventActionPair(KEY_SPACE,EJF_Shoot));
	m_actions.push_back(EventActionPair(KEY_E,EJF_NextWeapon));
	m_actions.push_back(EventActionPair(KEY_Q,EJF_PrevWeapon));
	m_actions.push_back(EventActionPair(KEY_W,EJF_Turbo));
	m_actions.push_back(EventActionPair(KEY_S,EJF_Brake));
}
JFHumanController::~JFHumanController()
{
}
	
void JFHumanController::onEvent(Event* event)
{
	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent* e=dynamic_cast<KeyboardEvent*>(event);
		std::list<EventActionPair>::iterator it= m_actions.begin();
		for(;it!=m_actions.end();++it)
		{
			(*it).Process(e,this);
		}
	}
}
void JFHumanController::DoAction(EJFActions action,bool on)
{
	math::vector3d currentAccel=GetShip()->GetAccelState();

	switch(action)
	{
	case EJF_Up:
		currentAccel.z=(on==true? 1:0);		
		break;
	case EJF_Down:
		currentAccel.z=(on==true? -1:0);		
		break;
	case EJF_Left:
		currentAccel.x=(on==true? -1:0);		
		break;
	case EJF_Right:
		currentAccel.x=(on==true? 1:0);		
		break;
	case EJF_Brake:
		GetShip()->Brake(on);
		break;
	case EJF_Turbo:
		GetShip()->Turbo(on);
		break;
	case EJF_Shoot:
		GetShip()->Shoot(on);
		break;
	case EJF_NextWeapon:
		if(on)
			GetShip()->SetNextWeapon();
		break;
	case EJF_PrevWeapon:
		if(on)
			GetShip()->SetPrevWeapon();
		break;
	}
	GetShip()->SetAccelState(currentAccel);

}


}
}
