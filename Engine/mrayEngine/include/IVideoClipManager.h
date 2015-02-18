

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   0:28
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IVideoClipManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IVideoClipManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IVideoClipManager___
#define ___IVideoClipManager___

#include "IVideoSource.h"

namespace mray
{
	namespace sound
	{
		class ISoundManager;
	}
	namespace OS{
		class IStream;
	}
namespace video
{

	enum EVideoOutputMode
	{
		EVideo_RGB    =  1,
		EVideo_RGBA   =  2,
		EVideo_RGBX   =  3,
		EVideo_BGR    =  6,
		EVideo_BGRA   =  7,
		EVideo_BGRX   =  8,
		EVideo_GREY   = 11,
		EVideo_GREY3  = 12,
		EVideo_GREY3A = 13,
		EVideo_GREY3X = 14,
		EVideo_YUV    = 17,
		EVideo_YUVA   = 18,
		EVideo_YUVX   = 19,
	};
class IVideoClipManager
{
protected:

public:
	IVideoClipManager(){}
	virtual~IVideoClipManager(){}
	

	virtual IVideoSourcePtr CreateVideo(const core::string& name,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0)=0;
	virtual IVideoSourcePtr CreateVideoFromFile(const core::string& fileName,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0)=0;
	virtual IVideoSourcePtr CreateVideoFromStream(OS::IStream* stream,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0)=0;
	virtual void RemoveVideo(IVideoSource* v)=0;
	virtual void ClearVideos()=0;

	virtual void setSoundManager(sound::ISoundManager*m)=0;
	virtual sound::ISoundManager* getSoundManager()=0;

	virtual void Update(float dt)=0;
};

}
}

#endif