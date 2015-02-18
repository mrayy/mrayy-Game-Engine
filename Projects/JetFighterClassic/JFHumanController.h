


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   18:42
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFHumanController.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFHumanController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFHumanController__
#define __JFHumanController__

#include "JFIController.h"

namespace mray
{
namespace game
{

	class JFHumanController;

enum EJFActions
{
	EJF_Up,
	EJF_Down,
	EJF_Left,
	EJF_Right,

	EJF_Shoot,

	EJF_NextWeapon,
	EJF_PrevWeapon,

	EJF_Turbo,
	EJF_Brake
};

class EventActionPair
{
protected:
	EKEY_CODE m_key;
	EJFActions m_action;
public:
	EventActionPair(EKEY_CODE k,EJFActions a):m_key(k),m_action(a)
	{}
	virtual bool Process(KeyboardEvent* e,JFHumanController* c);

};

class JFHumanController:public JFIController
{
private:
protected:
	std::list<EventActionPair> m_actions;
public:
	JFHumanController();
	virtual~JFHumanController();

	virtual void onEvent(Event* event);

	void DoAction(EJFActions action,bool on);

};

}
}

#endif

