


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   16:21
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFKinematicAction.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFKinematicAction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFKinematicAction__
#define __JFKinematicAction__

#include "JFIAction.h"
#include "JFObject.h"

namespace mray
{
namespace game
{

class JFKinematicAction:public JFIAction
{
private:
protected:
	bool m_makeKinematic;
	JFObject* m_obj;
public:
	JFKinematicAction(JFObject* o,bool k):m_makeKinematic(k),m_obj(o)
	{}
	virtual~JFKinematicAction(){}

	virtual void OnErrupt()
	{
		m_obj->MakeKinematic(m_makeKinematic);
	}
	virtual bool Execute(float dt)
	{
		m_obj->MakeKinematic(m_makeKinematic);
		return true;
	}
};


}
}

#endif
