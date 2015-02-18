#pragma once
/*
#include "ofConstants.h"
#ifndef TARGET_ANDROID
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"
#include "ofEvents.h"
*/

#include "mString.h"
//#define GST_DISABLE_DEPRECATED
#include <gst/gst.h>
#include <gst/gstpad.h>
#include "videoCommon.h"
#include "ImageInfo.h"
#include "IMutex.h"

class ofGstAppSink;
typedef struct _GstElement GstElement;
typedef struct _GstBuffer GstBuffer;
typedef struct _GstMessage GstMessage;

//-------------------------------------------------
//----------------------------------------- ofGstUtils
//-------------------------------------------------
using namespace mray;

class ofGstUtils{
public:
	ofGstUtils();
	virtual ~ofGstUtils();

	bool 	setPipelineWithSink(const core::string &pipeline, const core::string & sinkname="sink", bool isStream=false);
	bool 	setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream=false);

	void 	play();
	void 	stop();
	void 	setPaused(bool bPause);
	bool 	isPaused(){return bPaused;}
	bool 	isLoaded(){return bLoaded;}
	bool 	isPlaying(){return bPlaying;}

	float	getPosition();
	float 	getSpeed();
	float 	getDuration();
	ulong  getDurationNanos();
	bool  	getIsMovieDone();

	void 	setPosition(float pct);
	void 	setVolume(float volume);
	void 	setLoopState(int state);
	int	getLoopState(){return loopMode;}
	void 	setSpeed(float speed);

	void 	setFrameByFrame(bool bFrameByFrame);
	bool	isFrameByFrame();

	GstElement 	* getPipeline();
	GstElement 	* getSink();
	unsigned long getMinLatencyNanos();
	unsigned long getMaxLatencyNanos();

	virtual void close();

	void setSinkListener(ofGstAppSink * appsink);

	// callbacks to get called from gstreamer
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn preroll_cb(GstBuffer * buffer);
	virtual GstFlowReturn buffer_cb(GstBuffer * buffer);
#else
	virtual GstFlowReturn preroll_cb(GstSample * buffer);
	virtual GstFlowReturn buffer_cb(GstSample * buffer);
#endif
	virtual void 		  eos_cb();

	static void startGstMainLoop();
protected:
	ofGstAppSink * 		appsink;
	bool				isStream;

private:
	void 				gstHandleMessage();
	void				update();
	bool				startPipeline();

	bool 				bPlaying;
	bool 				bPaused;
	bool				bIsMovieDone;
	bool 				bLoaded;
	bool 				bFrameByFrame;
	int				loopMode;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;

	float				speed;
	gint64				durationNanos;
	bool				isAppSink;
};





//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------

class ofGstVideoUtils: public ofGstUtils
{
public:

	ofGstVideoUtils();
	virtual ~ofGstVideoUtils();

	bool 			setPipeline(const core::string& pipeline, video::EPixelFormat f, bool isStream=false, int w=-1, int h=-1);

	bool 			allocate(int w, int h, int bpp);

	bool 			isFrameNew();
	unsigned char * getPixels();
	video::ImageInfo*	getPixelsRef();
	void 			update();

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
	GstFlowReturn preroll_cb(GstBuffer * buffer);
	GstFlowReturn buffer_cb(GstBuffer * buffer);
#else
	GstFlowReturn preroll_cb(GstSample * buffer);
	GstFlowReturn buffer_cb(GstSample * buffer);
#endif
	void			eos_cb();


	video::ImageInfo		pixels;				// 24 bit: rgb
	video::ImageInfo	backPixels;
	video::ImageInfo	eventPixels;
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


//-------------------------------------------------
//----------------------------------------- appsink listener
//-------------------------------------------------

class ofGstAppSink{
public:
	virtual ~ofGstAppSink(){}
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn on_preroll(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
#else
	virtual GstFlowReturn on_preroll(GstSample * buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(GstSample * buffer){
		return GST_FLOW_OK;
	}
#endif
	virtual void			on_eos(){}

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){return false;};

	// pings when enough data has arrived to be able to get sink properties
	virtual void on_stream_prepared(){};
};


