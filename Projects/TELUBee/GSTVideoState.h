

/********************************************************************
	created:	2014/01/30
	created:	30:1:2014   16:36
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GSTVideoState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GSTVideoState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GSTVideoState__
#define __GSTVideoState__

#include "IEyesRenderingBaseState.h"



namespace mray
{
	namespace video
	{
		class ICameraVideoGrabber;
	}
namespace TBee
{
	class GStreamVideoProvider;
	class GstFileVideoSource;

class GSTVideoState :public IEyesRenderingBaseState
{
protected:

	GstFileVideoSource* m_videoSource;
	
	int m_frameCounter;
	int m_fps;
	float m_timeAcc;

	float m_scaling;

	virtual void _RenderUI(const math::rectf& rc,math::vector2d& pos);
public:
	GSTVideoState();
	virtual~GSTVideoState();

	virtual void InitState();

	virtual bool OnEvent(Event* e, const math::rectf &rc);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc, ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);

};

}
}


#endif
