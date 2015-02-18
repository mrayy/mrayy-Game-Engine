

/********************************************************************
	created:	2013/11/27
	created:	27:11:2013   12:21
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GStreamerTestState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GStreamerTestState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GStreamerTestState__
#define __GStreamerTestState__

#include "IRenderingState.h"
#include "GStreamVideoProvider.h"


namespace mray
{
namespace TBee
{


class GStreamerTestState :public IRenderingState
{
protected:

	GStreamVideoProvider* m_providers;
	video::ITexturePtr m_remoteTex;

public:
	GStreamerTestState();
	virtual~GStreamerTestState();

	virtual void InitState();


	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc, ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}


#endif
