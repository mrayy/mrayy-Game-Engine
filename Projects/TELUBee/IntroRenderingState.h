

/********************************************************************
	created:	2012/10/09
	created:	9:10:2012   22:41
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\IntroRenderingState.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	IntroRenderingState
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IntroRenderingState___
#define ___IntroRenderingState___


#include "IRenderingState.h"
#include "VideoGrabberTexture.h"
#include "IRenderTarget.h"

namespace mray
{
namespace TBee
{
	class IntroItem
	{
	protected:
		bool m_canSkip;
		float m_fadeAmount;
		float m_timer;
		bool m_skipped;
	public:
		IntroItem(){m_canSkip=true;m_timer=0;m_skipped=false;}
		virtual~IntroItem(){}
		bool IsCanSkip(){return m_canSkip;}
		virtual void Start();
		virtual bool IsDone(){return m_skipped==true && m_fadeAmount==0;}
		virtual void Skip(){m_skipped=true;}
		virtual void Render(const math::rectf& rc)=0;
		virtual void Update(float dt);
		virtual float GetLength()=0;

		virtual void LoadXML(xml::XMLElement* e);
	};

class IntroRenderingState:public IRenderingState
{
protected:

	std::vector<IntroItem*> m_introItems;
	int m_currentItem;

public:
	IntroRenderingState();
	virtual~IntroRenderingState();

	virtual void InitState();

	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);
	virtual video::IRenderTarget* GetLastFrame(ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);

};

}
}

#endif
