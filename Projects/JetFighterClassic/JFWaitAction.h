


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   16:36
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFWaitAction.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFWaitAction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFWaitAction__
#define __JFWaitAction__

#include "JFIAction.h"

namespace mray
{
namespace game
{

class JFWaitAction:public JFIAction
{
private:
protected:
	float m_time;
public:
	JFWaitAction(float t):m_time(t)
	{}
	virtual~JFWaitAction(){}

	void OnErrupt()
	{
	}
	bool Execute(float dt)
	{
		m_time-=dt;
		return m_time<=0;
	}
};

}
}

#endif
