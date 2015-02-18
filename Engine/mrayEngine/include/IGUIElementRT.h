

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   11:21
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\IGUIElementRT.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIElementRT
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IGUIElementRT__
#define __IGUIElementRT__

#include "compileconfig.h"
#include "IRenderTarget.h"

namespace mray
{
	class Event;
	class MouseEvent;
	class KeyboardEvent;
namespace scene
{
	class ISceneNode;
	class GUIManagerRT;

class MRAY_DLL IGUIElementRT:public IObject
{
	DECLARE_RTTI;
protected:
	ISceneNode* m_owner;
	bool m_enableBackClick;
	float m_resolution;
	GUIManagerRT* m_creator;

	bool m_manualRender;

	video::IRenderTargetPtr m_renderTarget;
	video::ITexturePtr m_renderTexture;
protected:
	virtual bool _OnMouseEvent(MouseEvent* e)=0;
	virtual bool _OnKeyboardEvent(KeyboardEvent* e)=0;


	void _CheckRT();
	virtual math::vector2d _GetElementSize()=0;
	virtual void _DrawElement(const math::rectf* vp) = 0;
public:
	IGUIElementRT();
	virtual~IGUIElementRT();

	void SetManualRender(bool m){m_manualRender=m;}
	bool GetManualRender(){return m_manualRender;}

	void SetCreator(GUIManagerRT* c){m_creator=c;}
	GUIManagerRT* GetCreator(){return m_creator;}

	void SetOwner(ISceneNode* m){m_owner=m;}
	ISceneNode* GetOwner(){return m_owner;}

	void SetEnableBackClick(bool e){m_enableBackClick=e;}
	bool GetEnableBackClick()const{return m_enableBackClick;}

	void SetResolution(float r){m_resolution=r;}
	float GetResolution(){return m_resolution;}


	video::IRenderTargetCRef GetRenderTarget(){return m_renderTarget;}
	video::ITextureCRef GetRenderTexture(){return m_renderTexture;}

	virtual bool OnEvent(Event* e);

	//Draw the element in background
	void DrawElement();

};

MakeSharedPtrType(IGUIElementRT)



}
}


#endif

