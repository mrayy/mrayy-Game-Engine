

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   18:39
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\LocalCameraRenderingState.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	LocalCameraRenderingState
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___LocalCameraRenderingState___
#define ___LocalCameraRenderingState___


#include "IRobotControlState.h"
#include "IRenderTarget.h"
#include "ParsedShaderPP.h"


namespace mray
{
	namespace video
	{
		class ICameraVideoGrabber;
	}
	namespace TBee
	{
		class LocalCameraVideoSource;
	}
	using namespace TBee;
	namespace AugTel
{
class LocalCameraRenderingState:public TBee::IRobotControlState
{
protected:

	LocalCameraVideoSource* m_cameraSource;
	
	int m_VerticalShift;

	bool m_capturing;
	int m_clickCount;
	math::vector2d m_firstClick;
	math::vector2d m_BoxSize;
	GUI::IGUIRenderer* m_guiRenderer;

	void _RenderUI(const math::rectf& rc, math::vector2d&pos, ETargetEye eye);
public:
	LocalCameraRenderingState(const core::string& name);
	virtual~LocalCameraRenderingState();

	virtual void InitState();
	void SetCameraInfo(ETargetEye eye,int id);

	video::ICameraVideoGrabber* GetCamera(ETargetEye eye);


	virtual bool OnEvent(Event* e, const math::rectf& rc);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);
	virtual bool CanSleep(){ return true; }

};

}
}

#endif
