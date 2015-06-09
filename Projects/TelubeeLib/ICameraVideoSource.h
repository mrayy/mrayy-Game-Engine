

/********************************************************************
	created:	2014/02/09
	created:	9:2:2014   19:10
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\ICameraVideoSource.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	ICameraVideoSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ICameraVideoSource__
#define __ICameraVideoSource__

#include "ITexture.h"


namespace mray
{
namespace TBee
{

class ICameraVideoSource
{
protected:
public:
	ICameraVideoSource(){}
	virtual~ICameraVideoSource(){}

	virtual void Init(){}
	virtual void Open(){}
	virtual void Close(){}
	virtual bool Blit(int eye=-1){ return false; }
	virtual math::vector2d GetEyeScalingFactor(int i) { return math::vector2d(1, 1); }
	virtual math::vector2d GetEyeResolution(int i) = 0;
	virtual video::ITexturePtr GetEyeTexture(int i) = 0;
	virtual math::rectf GetEyeTexCoords(int i) = 0;
	virtual void LoadFromXML(xml::XMLElement* e){}

	virtual float GetCaptureFrameRate(int i) = 0;

	virtual bool IsLocal() = 0;

	virtual void SetIsStereo(bool stereo){}
	virtual bool IsStereo(){ return false; }

};

}
}

#endif
