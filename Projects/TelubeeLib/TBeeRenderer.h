

/********************************************************************
	created:	2014/01/19
	created:	19:1:2014   2:05
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\TBeeRenderer.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	TBeeRenderer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TBeeRenderer__
#define __TBeeRenderer__


#include "TBeeCommon.h"

#include "ListenerContainer.h"

namespace mray
{
namespace TBee
{
	class TBeeRendererData;
	class TBeeRenderer;

	class ITBeeRendererListener
	{
	public: 
		virtual void OnRendererDraw(TBeeRenderer* r,const math::rectf& vp,video::IRenderTarget* rt,ETargetEye eye){}
	};

class TBeeRenderer :public ListenerContainer<ITBeeRendererListener*>
{
protected:
	TBeeRendererData* m_data;
	friend class TBeeRendererData;

	DECLARE_FIRE_METHOD(OnRendererDraw, (TBeeRenderer* r, math::rectf& vp, video::IRenderTarget* rt, ETargetEye eye), (r, vp, rt, eye))
public:
	TBeeRenderer();
	virtual~TBeeRenderer();

	int GetViewportCount();
	scene::ViewPort* GetViewport(int i);

	void Init(video::RenderWindow* rw);

	void Render(scene::ViewPort* vp);


	video::IRenderTargetPtr GetEyeImage(int i);


	void Update(float dt);
};

}
}


#endif
