

/********************************************************************
	created:	2014/02/28
	created:	28:2:2014   5:09
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\FlyingTelubeeRenderState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	FlyingTelubeeRenderState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FlyingTelubeeRenderState__
#define __FlyingTelubeeRenderState__


#include "IEyesRenderingBaseState.h"

namespace mray
{
namespace TBee
{
	class GstStereoStreamVideoSource;

class FlyingTelubeeRenderState :public IEyesRenderingBaseState
{
protected:
	GstStereoStreamVideoSource *m_cameraSource;

	int m_frameCounter;
	int m_fps;
	float m_timeAcc;

	int m_port1, m_port2;

	virtual void _RenderUI(const math::rectf& rc, math::vector2d& pos);
public:
	FlyingTelubeeRenderState();
	virtual~FlyingTelubeeRenderState();

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
