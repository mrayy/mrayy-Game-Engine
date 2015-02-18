/********************************************************************
	created:	2009/02/10
	created:	10:2:2009   0:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IVideoSource.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IVideoSource
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	interface for video textures
*********************************************************************/

#ifndef ___IVideoSource___
#define ___IVideoSource___

#include "mString.h"
#include "GCPtr.h"

#include "ITexture.h"
#include "IResource.h"
#include "IVideoGrabber.h"

namespace mray{
namespace video{

enum EVideoStatus
{
	EVS_Playing,
	EVS_Paused,
	EVS_Stopped
};

class IVideoSource:public IResource,public IVideoGrabber
{
public:

	IVideoSource(){}
	virtual~IVideoSource(){}

	virtual bool loadVideo(const  core::string&fileName)=0;

	virtual bool seek(float time)=0;

	virtual void setSpeedFactor(float speed)=0;
	virtual float getSpeedFactor()=0;

	virtual float getDuration()=0;

	virtual float getTime()=0;
	virtual float getPercentage()=0;
	virtual void setLoop(bool loop)=0;
	virtual bool isLoop()=0;
	virtual bool isDone()=0;

	virtual void play()=0;
	virtual void stop()=0;
	virtual void pause()=0;

	virtual EVideoStatus getStatus()=0;
};

MakeSharedPtrType(IVideoSource);

}
}


#endif //___IVideoSource___

