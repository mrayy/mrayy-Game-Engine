

/********************************************************************
	created:	2012/10/11
	created:	11:10:2012   0:33
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\RenderBlender.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	RenderBlender
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___RenderBlender___
#define ___RenderBlender___

#include "ITransitionBlender.h"

namespace mray
{
namespace TBee
{

class RenderBlender:public ITransitionBlender
{
protected:

	float m_time;
	float m_speed;

	bool m_staticBlending;
	float m_alpha[2];
public:
	RenderBlender();
	virtual~RenderBlender();


	void Reset();
	void SetSpeed(float s);

	void BlendImages(video::ITextureCRef t1,video::ITextureCRef t2,const math::rectf& rc);

	bool DoneBlending()
	{
		if(!m_staticBlending)
			return m_time==1.0f;
		else 
			return false;
	}
	void Update(float dt);

	void SetStaticBlending(bool e){m_staticBlending=e;}
	void SetStaticBlendingParams(float a,float b){m_alpha[0]=a;m_alpha[1]=b;}
	bool GetStaticBlending(){return m_staticBlending;}


	virtual void LoadSettings(const core::string& tabName);
	
};

}
}

#endif
