

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   19:20
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\ITransitionBlender.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	ITransitionBlender
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ITransitionBlender__
#define __ITransitionBlender__

#include <ITexture.h>


namespace mray
{
namespace TBee
{
class ITransitionBlender
{
protected:
public:
	ITransitionBlender(){}
	virtual~ITransitionBlender(){}
	virtual void Reset()=0;
	virtual void SetSpeed(float s)=0;

	virtual void BlendImages(video::ITextureCRef t1,video::ITextureCRef t2,const math::rectf& rc)=0;

	virtual bool DoneBlending()=0;
	virtual void Update(float dt)=0;

	virtual void LoadSettings(const core::string& tabName)=0;
};

}
}

#endif
