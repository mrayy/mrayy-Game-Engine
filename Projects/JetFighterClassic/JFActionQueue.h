


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   16:15
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFActionQueue.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFActionQueue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFActionQueue__
#define __JFActionQueue__

#include "JFIAction.h"

namespace mray
{
namespace game
{

class JFActionQueue:public JFIAction
{
private:
protected:
	std::list<JFIAction*> m_actions;
public:
	JFActionQueue();
	virtual~JFActionQueue();

	void AddAction(JFIAction*a);
	void ErruptActions();

	void OnErrupt();
	bool Execute(float dt);
};


}
}

#endif
