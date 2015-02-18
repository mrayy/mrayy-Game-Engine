
/********************************************************************
	created:	2009/03/22
	created:	22:3:2009   0:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ViewPort.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ViewPort
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ViewPort___
#define ___ViewPort___

#include "IPostProcessing.h"
#include "ViewportListener.h"
#include "TextureUnit.h"
#include "IRenderTarget.h"



namespace mray
{
namespace video
{
	class RenderWindow;
}
namespace scene
{
	class ISceneManager;
	class CameraNode;

class MRAY_DLL ViewPort:public video::IRenderArea,public ListenerContainer<IViewportListener*>
{
private:
protected:

	//attached camera
	CameraNode* m_camera;

	//attached render target
	video::IRenderTargetPtr m_rt;
	//texture unit which will encapsulate the render target
	video::TextureUnitPtr m_renderTargetTexUnit;
	//owner render window
	video::RenderWindow* m_renderWindow;

	core::string m_name;

	bool m_autoUpdateAbsRect;
	math::rectf m_vp;
	math::recti m_absVP;
	math::recti m_absRenderingVP;
	int m_zorder;

	video::SColor m_clearColor;

	//if true,then the viewport will not render it self into the window
	//the m_rt shouldn't be null,or this value will be ignored
	bool m_onlyRenderTarget;

	bool m_autoUpdateRenderTarget;
	bool m_rtSizeDirty;

	//is this viewport active,can we render it
	bool m_isActive;

	video::IPostProcessingPtr m_postProcessing;
	bool m_enablePostProcessing;

	bool m_isAutoClearRT;

	void _UpdateRTSize();

	DECLARE_FIRE_METHOD(onRenderBegin,(ViewPort*vp),(vp));
	DECLARE_FIRE_METHOD(onRenderDone,(ViewPort*vp),(vp));
	DECLARE_FIRE_METHOD(onSetCamera,(ViewPort*vp,CameraNode* cam),(vp,cam));
	DECLARE_FIRE_METHOD(onSetRenderTarget,(ViewPort*vp,video::IRenderTarget* rt),(vp,rt));
	DECLARE_FIRE_METHOD(onSetPostProcessing,(ViewPort*vp,video::IPostProcessing*pp),(vp,pp));

public:
	ViewPort();
	ViewPort(const core::string &name,CameraNode* camera,video::IRenderTarget* rt,video::RenderWindow* wnd,
		const math::rectf&vp,int zorder);
	virtual~ViewPort();

	video::RenderWindow* GetRenderWindow()
	{
		return m_renderWindow;
	}

	bool isAutoClearRT();
	void setAutoClearRT(bool s);

	void SetAutoUpdateRTSize(bool autoUpdate);
	bool GetAutoUpdateRTSize(){return m_autoUpdateRenderTarget;}

	bool isOnlyToRenderTarget();
	void setOnlyToRenderTarget(bool s);

	void setName(const core::string &name){m_name=name;}
	const core::string &getName(){return m_name;}

	virtual void draw();

	void updateViewPort();

	void setActive(bool a);
	bool isActive();

	void setPostProcessing(const video::IPostProcessingPtr& pp);
	void enablePostProcessing(bool e);
	bool isEnabledPostProcessing();

	const math::rectf&getRelViewPort()const;
	const math::recti&getAbsViewPort()const;
	const math::recti&getAbsRenderingViewPort()const;

	int getZOrder();
	CameraNode* getCamera();
	video::IRenderTargetCRef getRenderTarget();

	void setViewPort(const math::rectf&vp);
	void setAbsViewPort(const math::rectf&vp);

	void setZOrder(int z);
	void setCamera( CameraNode* cam);
	void setRenderTarget(video::IRenderTargetCRef rt);

	void onRenderBegin();
	void onRenderDone();

	virtual math::vector2di GetSize();

	virtual const video::ITexturePtr& GetColorTexture(int i=0);
	virtual int GetColorTextureCount() ;

	virtual void Resize(int x, int y) ;

	void SetClearColor(const video::SColor& c);
	const video::SColor& GetClearColor(){return m_clearColor;}

};

MakeSharedPtrType(ViewPort);

}
}

#endif //___ViewPort___
