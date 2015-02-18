



/********************************************************************
created:	2012/05/15
created:	15:5:2012   16:21
filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFImmuneAction.h
file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
file base:	JFImmuneAction
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __JFImmuneAction__
#define __JFImmuneAction__

#include "JFIAction.h"
#include "JFObject.h"

namespace mray
{
namespace game
{

class JFImmuneAction:public JFIAction
{
private:
protected:
	bool m_makeImmune;
	JFObject* m_obj;
public:
	JFImmuneAction(JFObject* o,bool k):m_makeImmune(k),m_obj(o)
	{}
	virtual~JFImmuneAction(){}

	virtual void OnErrupt()
	{
		m_obj->SetImmune(m_makeImmune);
	}
	virtual bool Execute(float dt)
	{
		m_obj->SetImmune(m_makeImmune);
		return true;
	}
};


}
}

#endif


