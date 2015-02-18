#pragma once

#include "ofGstUtils.h"
#include "IUDPClient.h"
#include "NetAddress.h"

typedef struct _GTimer		GTimer;


class ofGstVideoPlayer : public ofGstAppSink{
public:

	ofGstVideoPlayer();
	~ofGstVideoPlayer();

	/// needs to be called before loadMovie
	bool 	setPixelFormat(video::EPixelFormat pixelFormat);
	video::EPixelFormat	getPixelFormat();

	bool 	loadMovie(core::string uri);
	bool	Connect(const core::string& ip, int videoPort, int audioPort,int localAudioPort);


	void 	update();

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

	bool 			isFrameNew();

	unsigned char * getPixels();
	video::ImageInfo*	getPixelsRef();

	float 			getHeight();
	float 			getWidth();

	void setFrameByFrame(bool frameByFrame);
	void setThreadAppSink(bool threaded);

	ofGstVideoUtils * getGstVideoUtils();

protected:
	bool	allocate(int bpp);
	void	on_stream_prepared();

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){ return false; };

private:
	video::EPixelFormat	internalPixelFormat;
	unsigned long long				nFrames;
	int 				fps_n, fps_d;
	bool				bIsStream;
	bool				bRemote;
	bool				bIsAllocated;
	bool				threadAppSink;
	ofGstVideoUtils		videoUtils;
public:
	struct CConnectData
	{
		CConnectData()
		{
			 sink = dec = 0;
			audioPort = videoPort = 0;
		}
		GstElement* sink;
 		GstElement* dec;


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
		}
		int index;
		CConnectData *data;
		GstElement* src;
		network::NetAddress address;

		network::IUDPClient* client;
		int sourceID;

	};
protected:
	CConnectData m_connectData;
	CReadData m_readData[2];
};