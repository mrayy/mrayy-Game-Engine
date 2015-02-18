

/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   18:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraVideo.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraVideo
	file ext:	h
	author:		m_ohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraVideo___
#define ___TheoraVideo___


#include <IVideoSource.h>
#include <IThread.h>
#include <IStream.h>
#include <ISound.h>
#include <ImageInfo.h>

namespace mray{
namespace video{


	class TheoraManager;

class TheoraVideo:public IVideoSource
{
private:
protected:


	TheoraVideoClip* m_clip;

	ImageInfo m_imageData;
	math::vector2di m_size;
	bool m_hasNewFrame;
	ulong m_bufferID;

protected:
	virtual uint calcSizeInternal();
public:
	TheoraVideo(TheoraVideoClip*clip);
	virtual~TheoraVideo();

	TheoraVideoClip* getClip(){return m_clip;}

	void play();
	void stop();
	void pause();

	EVideoStatus getStatus();

	virtual bool loadVideo(const  core::string&fileName);

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual void SetFrameSize(int w,int h);
	virtual const math::Point2di& GetFrameSize();

	virtual float getTime();
	virtual float getPercentage();
	virtual void setLoop(bool loop);
	virtual bool isLoop();
	virtual bool isDone();
	virtual float getDuration();


	virtual void setSpeedFactor(float speed);
	virtual float getSpeedFactor();
	virtual bool seek(float time);
	virtual bool GrabFrame();
	virtual bool HasNewFrame();
	virtual ulong GetBufferID(){ return m_bufferID; }

	virtual const ImageInfo* GetLastFrame(){return &m_imageData;}
	void createVideoBuffers();


};

}
}


#endif //___TheoraVideo___
