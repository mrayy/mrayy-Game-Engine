


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   16:15
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFIAction.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFIAction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFIAction__
#define __JFIAction__


namespace mray
{
namespace game
{

class JFIAction
{
private:
protected:
public:
	JFIAction()
	{}
	virtual~JFIAction(){}

	virtual void OnErrupt()=0;
	virtual bool Execute(float dt)=0;
};


}
}

#endif
