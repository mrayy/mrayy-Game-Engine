
/********************************************************************
	created:	2013/10/28
	created:	28:10:2013   23:09
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\OculusDetectState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	OculusDetectState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusDetectState__
#define __OculusDetectState__


#include "IRenderingState.h"
#include "GUIOculusDetectImpl.h"

namespace mray
{
namespace TBee
{

class OculusDetectState:public IRenderingState,public IDelegateContainer
{
	static OculusDetectState* s_instance;
protected:
	GUI::IGUIManager* m_guiManager;
	GUIOculusDetectImpl* m_statePanel;
public:
	OculusDetectState();
	virtual~OculusDetectState();

	virtual void InitState();

	void SetState(GUIOculusDetectImpl::EState s){m_statePanel->SetState(s);}

	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);
	virtual void Update(float dt);

	virtual void LoadFromXML(xml::XMLElement* e);

	static OculusDetectState* Instance();
};

}
}


#endif
