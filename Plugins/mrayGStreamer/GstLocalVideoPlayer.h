

#ifndef __GSTLOCALVIDEOPLAYER__
#define __GSTLOCALVIDEOPLAYER__

#include "IGStreamerPlayer.h"
#include "IVideoGrabber.h"

namespace mray
{
namespace video
{

class GstLocalVideoPlayerImpl;

class GstLocalVideoPlayer :public IGStreamerPlayer
{
protected:
	GstLocalVideoPlayerImpl* m_impl;

	GstPipelineHandler* GetPipeline();

public:
	GstLocalVideoPlayer();
	virtual ~GstLocalVideoPlayer();

	void SetCameras(int cam0, int cam1);
	void SetResolution(int width, int height, int fps = 30);
	bool CreateStream();

	virtual bool IsStream();

	void SetVolume(float vol);

	virtual void Play();
	virtual void Pause();
	virtual void Stop();
	virtual bool IsLoaded();
	virtual bool IsPlaying();
	virtual void Close();

	//defined by the source video stream
	virtual void SetFrameSize(int w, int h) {}
	virtual const math::vector2di& GetFrameSize();

	virtual float GetCaptureFrameRate();


	//defined by the source video stream
	virtual void SetImageFormat(video::EPixelFormat fmt){}
	virtual video::EPixelFormat GetImageFormat();

	virtual bool GrabFrame();
	virtual bool HasNewFrame();
	virtual ulong GetBufferID();// incremented once per frame

	virtual const ImageInfo* GetLastFrame();

};

class GstLocalVideoPlayerGrabber :public IVideoGrabber
{
	GstLocalVideoPlayer* m_player;
public:
	GstLocalVideoPlayerGrabber(GstLocalVideoPlayer * p)
	{
		m_player = p;
	}
	virtual void SetFrameSize(int w, int h) { m_player->SetFrameSize(w, h); }
	virtual const math::vector2di& GetFrameSize() { return m_player->GetFrameSize(); }

	virtual void SetImageFormat(video::EPixelFormat fmt)  { m_player->SetImageFormat(fmt); }
	virtual video::EPixelFormat GetImageFormat() { return m_player->GetImageFormat(); }

	virtual bool GrabFrame() { return m_player->GrabFrame(); }
	virtual bool HasNewFrame() { return m_player->HasNewFrame(); }
	virtual ulong GetBufferID() { return m_player->GetBufferID(); }
	virtual float GetCaptureFrameRate() { return m_player->GetCaptureFrameRate(); }

	virtual const ImageInfo* GetLastFrame(int i) { return m_player->GetLastFrame(); }
};

}
}


#endif
