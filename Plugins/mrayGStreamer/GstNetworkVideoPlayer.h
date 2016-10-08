

#ifndef GstNetworkVideoPlayer_h__
#define GstNetworkVideoPlayer_h__


#include "IGStreamerPlayer.h"
#include "IVideoGrabber.h"

namespace mray
{
namespace video
{

class GstNetworkVideoPlayerImpl;

class GstNetworkVideoPlayer :public IGStreamerPlayer
{
protected:
	GstNetworkVideoPlayerImpl* m_impl;
	friend class GstNetworkVideoPlayerImpl;


	GstPipelineHandler* GetPipeline();
public:
	GstNetworkVideoPlayer();
	virtual ~GstNetworkVideoPlayer();

	//set ip address for the target host
	// videoport: port for the video stream, video rtcp is allocated as videoPort+1 and videoPort+2
	void SetIPAddress(const std::string& ip, uint videoPort, bool rtcp);
	bool CreateStream();

	virtual bool IsStream() ;

	void SetVolume(float vol);

	virtual void Play() ;
	virtual void Pause();
	virtual void Stop();
	virtual bool IsLoaded() ;
	virtual bool IsPlaying();
	virtual void Close() ;

	//defined by the source video stream
	virtual void SetFrameSize(int w, int h) {}
	virtual const math::vector2di& GetFrameSize() ;

	virtual float GetCaptureFrameRate();


	//defined by the source video stream
	virtual void SetImageFormat(video::EPixelFormat fmt){}
	virtual video::EPixelFormat GetImageFormat() ;

	virtual bool GrabFrame() ;
	virtual bool HasNewFrame() ;
	virtual ulong GetBufferID() ;// incremented once per frame

	virtual const ImageInfo* GetLastFrame() ;

	virtual int GetPort(int i);

};

class GstNetworkVideoPlayerGrabber :public IVideoGrabber
{
	GstNetworkVideoPlayer* m_player;
public:
	GstNetworkVideoPlayerGrabber(GstNetworkVideoPlayer * p)
	{
		m_player = p;
	}
	virtual void SetFrameSize(int w, int h) { m_player->SetFrameSize(w, h); }
	virtual const math::vector2di& GetFrameSize() { return m_player->GetFrameSize(); }

	virtual void SetImageFormat(video::EPixelFormat fmt)  { m_player->SetImageFormat(fmt); }
	virtual video::EPixelFormat GetImageFormat() { return m_player->GetImageFormat(); }

	virtual bool GrabFrame(int i) { return m_player->GrabFrame(); }
	virtual bool HasNewFrame(int i) { return m_player->HasNewFrame(); }
	virtual ulong GetBufferID(int i) { return m_player->GetBufferID(); }
	virtual float GetCaptureFrameRate(int i) { return m_player->GetCaptureFrameRate(); }

	virtual const ImageInfo* GetLastFrame(int i) { return m_player->GetLastFrame(); }
};

}
}

#endif // GstNetworkVideoPlayer_h__
