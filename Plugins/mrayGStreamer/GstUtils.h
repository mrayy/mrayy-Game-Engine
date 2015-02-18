#pragma once

#include "mString.h"

#include "videoCommon.h"
#include "ImageInfo.h"
#include "IMutex.h"



#define GST_DISABLE_DEPRECATED
#include <gst/gst.h>
#include <gst/gstpad.h>


namespace mray
{

namespace video
{

class IGstAppListener;


//-------------------------------------------------
//----------------------------------------- GstUtils
//-------------------------------------------------

class GstUtilsImpl;

class GstUtils{
public:
	GstUtils();
	virtual ~GstUtils();

	bool 	setPipelineWithSink(const core::string& pipeline, const core::string& sinkname = "sink", bool isStream = false);
	bool 	setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream = false);
	bool	startPipeline();

	void 	play();
	void 	stop();
	void 	setPaused(bool bPause);
	bool 	isPaused();
	bool 	isLoaded();
	bool 	isPlaying();

	float	getPosition();
	float 	getSpeed();
	float 	getDuration();
	ulong  getDurationNanos();
	bool  	getIsMovieDone();

	void 	setPosition(float pct);
	void 	setVolume(float volume);
	void 	setLoopState(int state);
	int	getLoopState();
	void 	setSpeed(float speed);

	void 	setFrameByFrame(bool bFrameByFrame);
	bool	isFrameByFrame();

	GstElement 	* getPipeline();
	GstElement 	* getSink();
	GstElement 	* getGstElementByName(const core::string & name);
	ulong getMinLatencyNanos();
	ulong getMaxLatencyNanos();

	virtual void close();


	void setSinkListener(IGstAppListener * appsink);


#if GST_VERSION_MAJOR==0

	virtual int preroll_cb(GstBuffer * buffer);
	virtual int buffer_cb(GstBuffer * buffer);
#else

	virtual int preroll_cb(GstSample * buffer);
	virtual int buffer_cb(GstSample * buffer);

#endif
	virtual void eos_cb();
protected:
	GstUtilsImpl* m_impl;




private:

};





//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------

class GstVideoUtils : public  GstUtils{
public:

	GstVideoUtils();
	virtual ~GstVideoUtils();

	bool 			setPipeline(const core::string& pipeline, int bpp = 24, bool isStream = false, int w = -1, int h = -1);

	bool 			allocate(int w, int h, int bpp);

	ulong	getFrameID();
	bool 			isFrameNew();
	unsigned char * getPixels();
	video::ImageInfo*	getPixelsRef();
	bool	grabFrame();

	float 			getHeight();
	float 			getWidth();

	void 			close();

	// this events happen in a different thread
	// do not use them for opengl stuff
	/*ofEvent<ofPixels> prerollEvent;
	ofEvent<ofPixels> bufferEvent;
	ofEvent<ofEventArgs> eosEvent;*/

protected:
#if GST_VERSION_MAJOR==0
	// GstFlowReturn
	virtual int preroll_cb(GstBuffer * buffer);
	virtual int buffer_cb(GstBuffer * buffer);
#else
	int preroll_cb(GstSample * buffer);
	int buffer_cb(GstSample * buffer);
#endif
	void			eos_cb();


	video::ImageInfo		pixels;				// 24 bit: rgb
	video::ImageInfo		backPixels;
	video::ImageInfo		eventPixels;
private:
	bool			bIsFrameNew;			// if we are new
	bool			bHavePixelsChanged;
	bool			bBackPixelsChanged;
	OS::IMutex*			mutex;
#if GST_VERSION_MAJOR==0
	GstBuffer * 	buffer, *prevBuffer;
#else
	GstSample * 	buffer, *prevBuffer;
	GstMapInfo mapinfo;
#endif
};

}
}

//-------------------------------------------------
//----------------------------------------- appsink listener
//-------------------------------------------------

