


/********************************************************************
	created:	2012/10/29
	created:	29:10:2012   18:50
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\BaseAppState.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	BaseAppState
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___BaseAppState___
#define ___BaseAppState___


#include "IState.h"


namespace mray
{
namespace TBee
{

	class IApplicationState;

class BaseAppState:public IState
{
protected:
	IApplicationState* m_state;

public:
	BaseAppState(const core::string&name,IApplicationState* st):IState(name),m_state(st)
	{}
	virtual~BaseAppState();
	

	IApplicationState* GetState(){return m_state;}

	virtual void onEnter(IState*prev);
	virtual void onUpdate();
	virtual void onExit();


	
};

}
}

#endif
