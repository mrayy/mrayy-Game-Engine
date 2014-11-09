

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
public:
	GstNetworkVideoPlayer();
	virtual ~GstNetworkVideoPlayer();

	//set ip address for the target host
	// videoport: port for the video stream, video rtcp is allocated as videoPort+1 and videoPort+2
	void SetIPAddress(const core::string& ip, int videoPort,bool rtcp);
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

	//defined by the source video stream
	virtual void SetImageFormat(video::EPixelFormat fmt){}
	virtual video::EPixelFormat GetImageFormat() ;

	virtual bool GrabFrame() ;
	virtual bool HasNewFrame() ;
	virtual ulong GetBufferID() ;// incremented once per frame

	virtual const ImageInfo* GetLastFrame() ;

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

	virtual bool GrabFrame() { return m_player->GrabFrame(); }
	virtual bool HasNewFrame() { return m_player->HasNewFrame(); }
	virtual ulong GetBufferID() { return m_player->GetBufferID(); }

	virtual const ImageInfo* GetLastFrame() { return m_player->GetLastFrame(); }
};

}
}

#endif // GstNetworkVideoPlayer_h__
