

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   1:01
	filename: 	d:\Development\mrayEngine\Plugins\mrayVideoVFW\VFWVideoManager.h
	file path:	d:\Development\mrayEngine\Plugins\mrayVideoVFW
	file base:	VFWVideoManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VFWVideoManager___
#define ___VFWVideoManager___

#include "IVideoClipManager.h"

namespace mray
{
namespace video
{

class VFWVideoManager:public IVideoClipManager
{
protected:

public:
	VFWVideoManager();
	virtual~VFWVideoManager();

	virtual IVideoSourcePtr CreateVideo(const core::string& name,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0);
	virtual IVideoSourcePtr CreateVideoFromFile(const core::string& fileName,EVideoOutputMode output_mode=EVideo_RGB,int numPrecachedOverride=0,bool usePower2Stride=0);
	virtual void RemoveVideo(IVideoSource* v);
	virtual void ClearVideos();

	virtual void setSoundManager(sound::ISoundManager*m);
	virtual sound::ISoundManager* getSoundManager();

	virtual void Update(float dt);
	
};

}
}

#endif

