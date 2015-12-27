#pragma once

#include "mString.h"

#include "videoCommon.h"
#include "IVideoGrabber.h"
#include "IUDPClient.h"
#include "NetAddress.h"
#include "IGstAppListener.h"

typedef struct _GTimer		GTimer;


typedef struct _GstElement GstElement;

namespace  mray
{
namespace video
{
	class GstVideoUtils;


class GstVideoPlayer : public IVideoGrabber,public IGstAppListener{

public:

	GstVideoPlayer();
	~GstVideoPlayer();


	bool	StartPipeline(const std::string& pipeline, bool stream);

	bool 	loadMovie(const core::string &uri);
	bool 	loadSideBySideMovie(const std::string& left, const std::string& right);
	bool	Connect(const std::string& ip, int videoPort, int audioPort, int localAudioPort);
	bool	Open2Streams( int videoPort1, int videoPort2);

	void	SetElementsAttribute(const std::string& elem, ...);

	bool 	GrabFrame();

	int		getCurrentFrame();
	int		getTotalNumFrames();

	void 	firstFrame();
	void 	nextFrame();
	void 	previousFrame();
	void 	setFrame(int frame);  // frame 0 = first frame...

	bool	isStream();

	void 	play();
	void 	stop();
	void 	setPaused(bool bPause);
	bool 	isPaused();
	bool 	isLoaded();
	bool 	isPlaying();

	float	getPosition();
	float 	getSpeed();
	float 	getDuration();
	bool  	getIsMovieDone();

	void 	setPosition(float pct);
	void 	setVolume(float volume);
	void 	setLoopState(int state);
	int 	getLoopState();
	void 	setSpeed(float speed);
	void 	close();

	unsigned char * getPixels();

	/// needs to be called before loadMovie
	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	void SetFrameSize(int w, int h){ m_frameSize.set(w, h); }
	const math::vector2di& GetFrameSize();
	virtual bool HasNewFrame() ;
	virtual ulong GetBufferID() ;// incremented once per frame

	virtual float GetCaptureFrameRate();

	virtual const ImageInfo* GetLastFrame();

	void setFrameByFrame(bool frameByFrame);
	void setThreadAppSink(bool threaded);

	int RecevedBytes()
	{
		return m_readData[0].recvBytes + m_readData[1].recvBytes;
	}

	//void SendData(GstBuffer* buffer, int index);


protected:
	bool	allocate(int bpp);
	void	OnStreamPrepared();


private:
	math::vector2di m_frameSize;
	video::EPixelFormat	internalPixelFormat;
	unsigned long long				nFrames;
	int 				fps_n, fps_d;
	bool				bIsStream;
	bool				bRemote;
	bool				bIsAllocated;
	bool				threadAppSink;
	GstVideoUtils	*	videoUtils;
public:
	struct CConnectData
	{
		CConnectData()
		{
			sink =  0;
			audioPort = videoPort = 0;
		}
		GstElement* sink;


		int audioPort;
		int videoPort;
	};

	struct CReadData
	{
		CReadData()
		{
			src = 0;
			sourceID = 0;
			client = 0;
			recvBytes = 0;
		}
		int index;
		CConnectData *data;
		GstElement* src;
		network::NetAddress address;
		int recvBytes;

		network::IUDPClient* client;
		int sourceID;

	};
protected:
	CConnectData m_connectData;
	CReadData m_readData[2];
};

}
}
