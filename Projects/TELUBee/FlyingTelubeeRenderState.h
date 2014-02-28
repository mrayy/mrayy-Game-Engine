

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
	class GstSingleNetVideoSource;

class FlyingTelubeeRenderState :public IEyesRenderingBaseState
{
protected:
	GstSingleNetVideoSource *m_cameraSource;
	std::string m_hostIp;
	int m_hostPort;
	int m_localPort;

	int m_frameCounter;
	int m_fps;
	float m_timeAcc;

	virtual void _RenderUI(const math::rectf& rc);
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
