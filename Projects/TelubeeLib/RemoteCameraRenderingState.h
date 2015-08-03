

/********************************************************************
created:	2012/10/10
created:	10:10:2012   18:39
filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\RemoteCameraRenderingState.h
file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
file base:	RemoteCameraRenderingState
file ext:	h
author:		MHD YAMEN SARAIJI

purpose:
*********************************************************************/
#ifndef ___RemoteCameraRenderingState___
#define ___RemoteCameraRenderingState___


#include "IEyesRenderingBaseState.h"


namespace mray
{
namespace video
{
class ICameraVideoGrabber;
}
namespace TBee
{
	class GstStereoNetVideoSource;
class RemoteCameraRenderingState :public IEyesRenderingBaseState
{
protected:
	GstStereoNetVideoSource *m_cameraSource;
	std::string m_hostIp;
	int m_hostPort;
	int m_localPort;

	int m_frameCounter;
	int m_fps;
	float m_timeAcc;

	virtual void _RenderUI(const math::rectf& rc, math::vector2d&pos, ETargetEye eye);
public:
	RemoteCameraRenderingState(const core::string& name);
	virtual~RemoteCameraRenderingState();

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
