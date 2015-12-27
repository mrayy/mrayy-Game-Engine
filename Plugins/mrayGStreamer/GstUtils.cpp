#include "stdafx.h"



#include "GstUtils.h"
//#include "ofUtils.h"

#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

#include <glib-object.h>
#include <glib.h>
#include <algorithm>

#include <ILogManager.h>
#include <StringConverter.h>
#include <PixelUtil.h>
#include <IThreadManager.h>
#include "CMySrc.h"
#include "CMySink.h"
#include "IGstAppListener.h"
#include "GStreamerCore.h"

#define OF_LOOP_NONE 0
#define  OF_LOOP_NORMAL 1
#define OF_LOOP_PALINDROME 2

namespace mray
{
namespace video
{

class GstUtilsImpl
{
public:
	GstUtilsImpl()
	{

		bLoaded = false;
		speed = 1;
		bPaused = false;
		bIsMovieDone = false;
		bPlaying = false;
		loopMode = OF_LOOP_NONE;
		bFrameByFrame = false;

		gstPipeline = NULL;
		gstSink = NULL;

		durationNanos = 0;

		isAppSink = false;
		frameID = 0;
	}
	// callbacks to get called from gstreamer
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn preroll_cb(GstBuffer * buffer);
	virtual GstFlowReturn buffer_cb(GstBuffer * buffer);
#else
	virtual GstFlowReturn preroll_cb(GstSample * buffer);
	virtual GstFlowReturn buffer_cb(GstSample * buffer);
#endif
	virtual void 		  eos_cb();

	static bool			busFunction(GstBus * bus, GstMessage * message, GstUtilsImpl * app);
	bool				gstHandleMessage(GstBus * bus, GstMessage * message);

	void stop();
	void close();
	void play();

	void setPaused(bool _bPause);
	void 	setSpeed(float speed);

	bool 				bPlaying;
	bool 				bPaused;
	bool				bIsMovieDone;
	bool 				bLoaded;
	bool 				bFrameByFrame;
	int			loopMode;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;

	float				speed;
	gint64				durationNanos;
	bool				isAppSink;

	IGstAppListener * 		appsink;
	bool				isStream;

	ulong frameID;

	GstBus * bus;
};


//-------------------------------------------------
//----------------------------------------- gstUtils
//-------------------------------------------------


// called when the appsink notifies us that there is a new buffer ready for
// processing

static GstFlowReturn on_new_buffer_from_source(GstAppSink * elt, void * data){
#if GST_VERSION_MAJOR==0
	GstBuffer *buffer = gst_app_sink_pull_buffer(GST_APP_SINK(elt));
#else
	GstSample *buffer = gst_app_sink_pull_sample(GST_APP_SINK(elt));
#endif
	return (GstFlowReturn)((GstUtils*)data)->buffer_cb(buffer);
}

static GstFlowReturn on_new_preroll_from_source(GstAppSink * elt, void * data){
#if GST_VERSION_MAJOR==0
	GstBuffer *buffer;
#else
	GstSample *buffer;
#endif
	buffer = gst_app_sink_pull_preroll(GST_APP_SINK(elt));
	return (GstFlowReturn)((GstUtils*)data)->preroll_cb(buffer);
}

static void on_eos_from_source(GstAppSink * elt, void * data){
	((GstUtils*)data)->eos_cb();
}



GstUtils::GstUtils() {
	m_impl = new GstUtilsImpl();

	m_impl->isStream = false;

	m_impl->appsink = NULL;

	video::GStreamerCore::Ref();

}

GstUtils::~GstUtils() {
	close();
	delete m_impl;
	video::GStreamerCore::Unref();
}

#if GST_VERSION_MAJOR==0
GstFlowReturn GstUtilsImpl::preroll_cb(GstBuffer * buffer){
#else
GstFlowReturn GstUtilsImpl::preroll_cb(GstSample * buffer){
#endif
	bIsMovieDone = false;
	//if (appsink) return (GstFlowReturn)appsink->on_preroll(buffer);
	 return GST_FLOW_OK;
}


#if GST_VERSION_MAJOR==0
GstFlowReturn GstUtilsImpl::buffer_cb(GstBuffer * buffer){
#else
GstFlowReturn GstUtilsImpl::buffer_cb(GstSample * buffer){
#endif
	bIsMovieDone = false;
	++frameID;
	if (appsink) return (GstFlowReturn)appsink->OnBuffer(buffer);
	else return GST_FLOW_OK;
}

void GstUtilsImpl::eos_cb(){
	bIsMovieDone = true;
	if (appsink) appsink->OnEOS();
}

bool GstUtils::setPipelineWithSink(const std::string& pipeline, const std::string& sinkname, bool isStream){
	if (isPlaying())
	{
		stop();
	}

	gchar* pipeline_string =
		g_strdup((pipeline).c_str());

	GError * error = NULL;
	m_impl->gstPipeline = gst_parse_launch(pipeline_string, &error);

	gLogManager.log("GstUtils - setPipelineWithSink(): gstreamer pipeline: " + core::string(pipeline_string), ELL_INFO);
	if (error != NULL){
		gLogManager.log("GstUtils - setPipelineWithSink(): couldn't create pipeline: " + core::string(error->message), ELL_WARNING);
		return false;
	}

	if (sinkname != ""){
		m_impl->gstSink = gst_bin_get_by_name(GST_BIN(m_impl->gstPipeline), sinkname.c_str());

		if (!m_impl->gstSink){
			gLogManager.log("GstUtils - setPipelineWithSink(): couldn't get sink from string pipeline", ELL_WARNING);
		}
	}

	return setPipelineWithSink(m_impl->gstPipeline, m_impl->gstSink, isStream);
}

bool GstUtils::setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream_){
	if (isPlaying())
	{
		stop();
	}

	m_impl->gstPipeline = pipeline;
	m_impl->gstSink = sink;
	m_impl->isStream = isStream_;

	if (m_impl->gstSink){
		//	gst_base_sink_set_sync(GST_BASE_SINK(m_impl->gstSink), true);
	}

	if (m_impl->gstSink && core::string(gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(gst_element_get_factory(m_impl->gstSink)))) == "appsink"){
		m_impl->isAppSink = true;
	}
	else{
		m_impl->isAppSink = false;
	}

	return true;
}

void GstUtils::setFrameByFrame(bool _bFrameByFrame){
	m_impl->bFrameByFrame = _bFrameByFrame;
	if (m_impl->gstSink){
		g_object_set(G_OBJECT(m_impl->gstSink), "sync", !m_impl->bFrameByFrame, (void*)NULL);
	}
}

bool GstUtils::isFrameByFrame(){
	return m_impl->bFrameByFrame;
}

bool GstUtils::startPipeline(){

	m_impl->bPaused = true;
	m_impl->speed = 1.0f;
	m_impl->frameID = 0;

	if (gst_element_set_state(GST_ELEMENT(m_impl->gstPipeline), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
		gLogManager.log("GstUtils - startPipeline(): unable to set pipeline to ready", ELL_WARNING);
		return false;
	}

	if (gst_element_get_state(GST_ELEMENT(m_impl->gstPipeline), NULL, NULL, 10 * GST_SECOND) == GST_STATE_CHANGE_FAILURE){
		gLogManager.log("GstUtils - startPipeline(): unable to get pipeline ready status", ELL_WARNING);
		return false;
	}/**/

	// pause the pipeline
	if (gst_element_set_state(GST_ELEMENT(m_impl->gstPipeline), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
		gLogManager.log("GstUtils - startPipeline(): unable to pause pipeline", ELL_WARNING);
		return false;
	}

	// wait for paused state to query the duration
	if (!m_impl->isStream){
		GstState state = GST_STATE_PAUSED;
		if (gst_element_get_state(m_impl->gstPipeline, &state, NULL, 2 * GST_SECOND) == GST_STATE_CHANGE_FAILURE){
			gLogManager.log("GstUtils - startPipeline(): unable to get pipeline paused state", ELL_WARNING);
			return false;
		}
		m_impl->bPlaying = true;
		m_impl->bLoaded = true;
	}

	m_impl->bus = gst_pipeline_get_bus(GST_PIPELINE(m_impl->gstPipeline));

	if (m_impl->bus){
		gst_bus_add_watch(m_impl->bus, (GstBusFunc)m_impl->busFunction, m_impl);
	}



	if (m_impl->isAppSink){
		gLogManager.log("GstUtils - startPipeline(): attaching callbacks", ELL_INFO, EVL_Heavy);
		// set the appsink to not emit signals, we are using callbacks instead
		// and frameByFrame to get buffers by polling instead of callback
		g_object_set(G_OBJECT(m_impl->gstSink), "emit-signals", FALSE, "sync", !m_impl->bFrameByFrame, (void*)NULL);
		//gst_app_sink_set_drop(GST_APP_SINK(gstSink),1);
		//gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink),2);

		if (!m_impl->bFrameByFrame){
			GstAppSinkCallbacks gstCallbacks;
			gstCallbacks.eos = &on_eos_from_source;
			gstCallbacks.new_preroll = &on_new_preroll_from_source;
#if GST_VERSION_MAJOR==0
			gstCallbacks.new_buffer = &on_new_buffer_from_source;
#else
			gstCallbacks.new_sample = &on_new_buffer_from_source;
#endif

			gst_app_sink_set_callbacks(GST_APP_SINK(m_impl->gstSink), &gstCallbacks, this, NULL);
		}

	}

	if (!m_impl->isStream){
		setSpeed(1.0);
	}


	return true;
}

void GstUtils::play(){
	m_impl->play();
}

void GstUtilsImpl::play(){
	setPaused(false);

	//this is if we set the speed first but it only can be set when we are playing.
	if (!isStream) setSpeed(speed);
}

void GstUtils::setPaused(bool _bPause){
	m_impl->setPaused(_bPause);
}

void GstUtilsImpl::setPaused(bool _bPause){
	bPaused = _bPause;
	//timeLastIdle = ofGetElapsedTimeMillis();
	if (bLoaded){
		if (bPlaying){
			if (bPaused){
				gst_element_set_state(gstPipeline, GST_STATE_PAUSED);
			}
			else{
				gst_element_set_state(gstPipeline, GST_STATE_PLAYING);
			}
		}
		else{
			GstState state = GST_STATE_PAUSED;
			gst_element_set_state(gstPipeline, state);
			gst_element_get_state(gstPipeline, &state, NULL, 2 * GST_SECOND);
			if (!bPaused){
				gst_element_set_state(gstPipeline, GST_STATE_PLAYING);
			}
			bPlaying = true;
		}
	}
}

bool 	GstUtils::isPaused(){ return m_impl->bPaused; }
bool 	GstUtils::isLoaded(){ return m_impl->bLoaded; }
bool 	GstUtils::isPlaying(){ return m_impl->bPlaying; }
void GstUtils::stop(){
	m_impl->stop();
}
void GstUtilsImpl::stop(){
	if (!bPlaying) return;
	GstState state = GST_STATE_PAUSED;
	if (!bPaused){
		gst_element_set_state(gstPipeline, state);
		gst_element_get_state(gstPipeline, &state, NULL, 2 * GST_SECOND);
	}
	state = GST_STATE_READY;
	gst_element_set_state(gstPipeline, state);
	gst_element_get_state(gstPipeline, &state, NULL, 2 * GST_SECOND);
	bPlaying = false;
	bPaused = true;

}

float GstUtils::getPosition(){
	if (m_impl->gstPipeline){
		gint64 pos = 0;
#if GST_VERSION_MAJOR==0
		GstFormat format = GST_FORMAT_TIME;
		if (!gst_element_query_position(GST_ELEMENT(m_impl->gstPipeline), &format, &pos)){
#else
		if (!gst_element_query_position(GST_ELEMENT(m_impl->gstPipeline), GST_FORMAT_TIME, &pos)){
#endif

			gLogManager.log("GstUtils - getPosition(): couldn't query position", ELL_WARNING);
			return -1;
		}
		return (float)pos / (float)m_impl->durationNanos;
	}
	else{
		return -1;
	}
}

float GstUtils::getSpeed(){
	return m_impl->speed;
}

float GstUtils::getDuration(){
	return (float)getDurationNanos() / (float)GST_SECOND;
}

ulong GstUtils::getDurationNanos(){
	GstFormat format = GST_FORMAT_TIME;

#if GST_VERSION_MAJOR==0
	if (!gst_element_query_duration(getPipeline(), &format, &m_impl->durationNanos))
#else
	if (!gst_element_query_duration(getPipeline(), format, &m_impl->durationNanos))
#endif
		gLogManager.log("GstUtils - getDurationNanos(): couldn't query time duration", ELL_WARNING);
	return m_impl->durationNanos;

}

bool  GstUtils::getIsMovieDone(){
	if (m_impl->isAppSink){
		return gst_app_sink_is_eos(GST_APP_SINK(m_impl->gstSink));
	}
	else{
		return m_impl->bIsMovieDone;
	}
}

void GstUtils::setPosition(float pct){
	//pct = CLAMP(pct, 0,1);// check between 0 and 1;
	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags)(GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH);
	gint64 pos = (guint64)((double)pct*(double)m_impl->durationNanos);

	/*if(bPaused){
	seek_lock();
	gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
	posChangingPaused=true;
	seek_unlock();
	}*/
	if (m_impl->speed > 0){
		if (!gst_element_seek(GST_ELEMENT(m_impl->gstPipeline), m_impl->speed, format,
			flags,
			GST_SEEK_TYPE_SET,
			pos,
			GST_SEEK_TYPE_SET,
			-1)) {
			gLogManager.log("GstUtils - setPosition(): unable to seek", ELL_WARNING);
		}
	}
	else{
		if (!gst_element_seek(GST_ELEMENT(m_impl->gstPipeline), m_impl->speed, format,
			flags,
			GST_SEEK_TYPE_SET,
			0,
			GST_SEEK_TYPE_SET,
			pos)) {
			gLogManager.log("GstUtils - setPosition(): unable to seek", ELL_WARNING);
		}
	}
}

void GstUtils::setVolume(float volume){
	gdouble gvolume = volume;
	g_object_set(G_OBJECT(m_impl->gstPipeline), "volume", gvolume, (void*)NULL);
}

void GstUtils::setLoopState(int state){
	m_impl->loopMode = state;
}

int	GstUtils::getLoopState(){ return m_impl->loopMode; }
void GstUtils::setSpeed(float _speed){
	m_impl->setSpeed(_speed);
}
void GstUtilsImpl::setSpeed(float _speed){

	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags)(GST_SEEK_FLAG_SKIP | GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH);
	gint64 pos;

	if (_speed == 0){
		gst_element_set_state(gstPipeline, GST_STATE_PAUSED);
		return;
	}
#if GST_VERSION_MAJOR==0
	if (!gst_element_query_position(GST_ELEMENT(gstPipeline), &format, &pos) || pos < 0){
		//gLogManager.log("GstUtils") << "setSpeed(): couldn't query position";
		return;
	}
#else
	if (!gst_element_query_position(GST_ELEMENT(gstPipeline), format, &pos) || pos<0){
		//gLogManager.log("GstUtils") << "setSpeed(): couldn't query position";
		return;
	}
#endif

	speed = _speed;
	//pos = (float)gstData.lastFrame * (float)fps_d / (float)fps_n * GST_SECOND;

	if (!bPaused)
		gst_element_set_state(gstPipeline, GST_STATE_PLAYING);

	if (speed > 0){
		if (!gst_element_seek(GST_ELEMENT(gstPipeline), speed, format,
			flags,
			GST_SEEK_TYPE_SET,
			pos,
			GST_SEEK_TYPE_SET,
			-1)) {
			gLogManager.log("GstUtils - setSpeed(): unable to change speed", ELL_WARNING);
		}
	}
	else{
		if (!gst_element_seek(GST_ELEMENT(gstPipeline), speed, format,
			flags,
			GST_SEEK_TYPE_SET,
			0,
			GST_SEEK_TYPE_SET,
			pos)) {
			gLogManager.log("GstUtils - setSpeed(): unable to change speed", ELL_WARNING);
		}
	}

	gLogManager.log("GstUtils - setSpeed(): speed changed to " + core::StringConverter::toString(speed), ELL_INFO, EVL_Heavy);

}

void GstUtilsImpl::close(){
	if (bPlaying){
		stop();
	}
	if (bLoaded){
		gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
		gst_element_get_state(gstPipeline, NULL, NULL, 2 * GST_SECOND);
		// gst_object_unref(gstSink); this crashes, why??

		gst_object_unref(gstPipeline);
		gstPipeline = NULL;
		gstSink = NULL;
	}

	bLoaded = false;
}

static core::string getName(GstState state){
	switch (state){
	case   GST_STATE_VOID_PENDING:
		return "void pending";
	case   GST_STATE_NULL:
		return "null";
	case   GST_STATE_READY:
		return "ready";
	case   GST_STATE_PAUSED:
		return "paused";
	case   GST_STATE_PLAYING:
		return "playing";
	default:
		return "";
	}
}

bool GstUtilsImpl::busFunction(GstBus * bus, GstMessage * message, GstUtilsImpl * gstUtils){
	return gstUtils->gstHandleMessage(bus, message);
}

bool GstUtilsImpl::gstHandleMessage(GstBus * bus, GstMessage * msg){
//	if (appsink && appsink->on_message(msg)) return true;

	// 	gLogManager.log("GstUtils - gstHandleMessage(): got " << GST_MESSAGE_TYPE_NAME(msg)
	// 		<< " message from " << GST_MESSAGE_SRC_NAME(msg));

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_BUFFERING:
		gint pctBuffered;
		gst_message_parse_buffering(msg, &pctBuffered);
		//gLogManager.log("GstUtils - gstHandleMessage(): buffering " << pctBuffered;
		/*if(pctBuffered<100){
		gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
		}else if(!bPaused){
		gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
		}*/
		break;

#if GST_VERSION_MAJOR==0
	case GST_MESSAGE_DURATION:{
									GstFormat format = GST_FORMAT_TIME;
									gst_element_query_duration(gstPipeline, &format, &durationNanos);
	}break;
#else
	case GST_MESSAGE_DURATION_CHANGED:
		gst_element_query_duration(gstPipeline, GST_FORMAT_TIME, &durationNanos);
		break;

#endif

	case GST_MESSAGE_STATE_CHANGED:{
										GstState oldstate, newstate, pendstate;
										gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
										if (isStream && newstate == GST_STATE_PAUSED && !bPlaying){
											bLoaded = true;
											bPlaying = true;
											if (!bPaused){
												//   ofLogVerbose("GstUtils") << "gstHandleMessage(): setting stream pipeline to play";
												play();
											}
										}

										//   ofLogVerbose("GstUtils") << "gstHandleMessage(): " << GST_MESSAGE_SRC_NAME(msg) << " state changed from "
										//   << getName(oldstate) << " to " << getName(newstate) << " (" + getName(pendstate) << ")";
	}break;

	case GST_MESSAGE_ASYNC_DONE:
		//ofLogVerbose("GstUtils") << "gstHandleMessage(): async done";
		break;

	case GST_MESSAGE_WARNING:
	{
								GError *err;
								gchar *debug;
								gst_message_parse_warning(msg, &err, &debug);
								gLogManager.log("GstUtils - HandleMessage(): Warning- " + core::string(debug), ELL_WARNING);
								// 								ofLogVerbose("GstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
								// 									<< gst_element_get_name(GST_MESSAGE_SRC(msg)) << "  reported: " << err->message;

								g_error_free(err);
								g_free(debug);

	}break;
	case GST_MESSAGE_ERROR: {
								GError *err;
								gchar *debug;
								gst_message_parse_error(msg, &err, &debug);

								gLogManager.log("GstUtils - HandleMessage(): Error- " + core::string(debug), ELL_WARNING);
								// 								ofLogVerbose("GstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
								// 									<< gst_element_get_name(GST_MESSAGE_SRC(msg)) << "  reported: " << err->message;

								g_error_free(err);
								g_free(debug);

								gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

	}break;

	case GST_MESSAGE_EOS:
		//	ofLogVerbose("GstUtils") << "gstHandleMessage(): end of the stream";
		bIsMovieDone = true;


//		if (appsink && !isAppSink) appsink->on_eos();

		switch (loopMode){

		case OF_LOOP_NORMAL:{
								GstFormat format = GST_FORMAT_TIME;
								GstSeekFlags flags = (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT);
								gint64 pos;
#if GST_VERSION_MAJOR==0
								gst_element_query_position(GST_ELEMENT(gstPipeline), &format, &pos);
#else
								gst_element_query_position(GST_ELEMENT(gstPipeline), format, &pos);
#endif
								if (!gst_element_seek(GST_ELEMENT(gstPipeline),
									speed,
									format,
									flags,
									GST_SEEK_TYPE_SET,
									0,
									GST_SEEK_TYPE_SET,
									durationNanos)) {
									gLogManager.log("GstUtils - gstHandleMessage(): unable to seek", ELL_WARNING);
								}
		}break;

		case OF_LOOP_PALINDROME:{
									GstFormat format = GST_FORMAT_TIME;
									GstSeekFlags flags = (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT);
									gint64 pos;
#if GST_VERSION_MAJOR==0
									gst_element_query_position(GST_ELEMENT(gstPipeline), &format, &pos);
#else
									gst_element_query_position(GST_ELEMENT(gstPipeline), format, &pos);
#endif
									float loopSpeed;
									if (pos > 0)
										loopSpeed = -speed;
									else
										loopSpeed = speed;
									if (!gst_element_seek(GST_ELEMENT(gstPipeline),
										loopSpeed,
										GST_FORMAT_UNDEFINED,
										flags,
										GST_SEEK_TYPE_NONE,
										0,
										GST_SEEK_TYPE_NONE,
										0)) {
										gLogManager.log("GstUtils - gstHandleMessage(): unable to seek", ELL_WARNING);
									}
		}break;

		default:
			break;
		}

		break;

	default:
		//ofLogVerbose("GstUtils") << "gstHandleMessage(): unhandled message from " << GST_MESSAGE_SRC_NAME(msg);
		break;
	}
	return true;
}

GstElement 	* GstUtils::getPipeline(){
	return m_impl->gstPipeline;
}

GstElement 	* GstUtils::getSink(){
	return m_impl->gstSink;
}

GstElement 	* GstUtils::getGstElementByName(const core::string & name){
	return gst_bin_get_by_name(GST_BIN(m_impl->gstPipeline), name.c_str());
}

void GstUtils::setSinkListener(IGstAppListener * appsink_){
	m_impl->appsink = appsink_;
}



#if GST_VERSION_MAJOR==0

int GstUtils::preroll_cb(GstBuffer * buffer)
{
	return m_impl->preroll_cb(buffer);
}
int GstUtils::buffer_cb(GstBuffer * buffer)
{
	return m_impl->buffer_cb(buffer);
}

#else

int GstUtils::preroll_cb(GstSample * buffer)
{
	return m_impl->preroll_cb(buffer);
}
int GstUtils::buffer_cb(GstSample * buffer)
{
	return m_impl->buffer_cb(buffer);
}
#endif

void GstUtils::eos_cb()
{
	return m_impl->eos_cb();
}

ulong GstUtils::getMinLatencyNanos(){
	GstClockTime minlat = 0, maxlat = 0;
	GstQuery * q = gst_query_new_latency();
	if (gst_element_query(m_impl->gstPipeline, q)) {
		gboolean live;
		gst_query_parse_latency(q, &live, &minlat, &maxlat);
	}
	gst_query_unref(q);
	return minlat;
}

ulong GstUtils::getMaxLatencyNanos(){
	GstClockTime minlat = 0, maxlat = 0;
	GstQuery * q = gst_query_new_latency();
	if (gst_element_query(m_impl->gstPipeline, q)) {
		gboolean live;
		gst_query_parse_latency(q, &live, &minlat, &maxlat);
	}
	gst_query_unref(q);
	return maxlat;
}

void GstUtils::close()
{
	m_impl->close();
}




//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------



GstVideoUtils::GstVideoUtils(){
	bIsFrameNew = false;
	bHavePixelsChanged = false;
	bBackPixelsChanged = false;
	buffer = 0;
	prevBuffer = 0;
#if GST_VERSION_MAJOR==1
	GstMapInfo initMapinfo = { 0, };
	mapinfo = initMapinfo;
#endif
	mutex = OS::IThreadManager::getInstance().createMutex();

}

GstVideoUtils::~GstVideoUtils(){
	close();
}

void GstVideoUtils::close(){
	GstUtils::close();
	mutex->lock();

	pixels.clear();
	backPixels.clear();
	bIsFrameNew = false;
	bHavePixelsChanged = false;
	bBackPixelsChanged = false;
#if GST_VERSION_MAJOR==0
	if (prevBuffer) gst_buffer_unref(prevBuffer);
	if (buffer) gst_buffer_unref(buffer);
#else
	if (prevBuffer) gst_sample_unref(prevBuffer);
	if (buffer) gst_sample_unref(buffer);
#endif
	prevBuffer = 0;
	buffer = 0;
	mutex->unlock();

}

bool GstVideoUtils::isFrameNew(){
	return bIsFrameNew;
}


ulong	GstVideoUtils::getFrameID()
{
	return m_impl->frameID;
}

unsigned char * GstVideoUtils::getPixels(){
	return pixels.imageData;
}

video::ImageInfo* GstVideoUtils::getPixelsRef(){
	return &pixels;
}

bool GstVideoUtils::grabFrame(){
	if (isLoaded()){
		if (!isFrameByFrame()){
			mutex->lock();
			bHavePixelsChanged = bBackPixelsChanged;
			if (bHavePixelsChanged){
				bBackPixelsChanged = false;
				math::Swap(pixels.imageData, backPixels.imageData);

#if GST_VERSION_MAJOR==0
				if (prevBuffer) gst_buffer_unref(prevBuffer);
#else
				if (prevBuffer) gst_sample_unref(prevBuffer);
#endif
				prevBuffer = buffer;
			}

			mutex->unlock();
		}
		else{
#if GST_VERSION_MAJOR==0
			GstBuffer *buffer;

			//get the buffer from appsink
			if (isPaused()) buffer = gst_app_sink_pull_preroll(GST_APP_SINK(getSink()));
			else buffer = gst_app_sink_pull_buffer(GST_APP_SINK(getSink()));

			if (buffer){
				if (pixels.imageDataSize != 0){
					if (prevBuffer) gst_buffer_unref(prevBuffer);
					pixels.setData(GST_BUFFER_DATA(buffer), pixels.Size, pixels.format);

					prevBuffer = buffer;
					bHavePixelsChanged = true;
				}
			}
		}
#else
			GstBuffer *buffer;
			GstSample * sample;

			//get the buffer from appsink
			if (isPaused()){
				sample = gst_app_sink_pull_preroll(GST_APP_SINK(getSink()));
			}
			else{
				sample = gst_app_sink_pull_sample(GST_APP_SINK(getSink()));
			}
			buffer = gst_sample_get_buffer(sample);

			if (buffer){
				if (pixels.imageDataSize != 0){
					if (prevBuffer) gst_sample_unref(prevBuffer);
					gst_buffer_map(buffer, &mapinfo, GST_MAP_READ);

					pixels.setData(mapinfo.data, pixels.Size, pixels.format);

					prevBuffer = sample;
					bHavePixelsChanged = true;
					gst_buffer_unmap(buffer, &mapinfo);
				}
			}
		}
#endif
	}
	else{
		gLogManager.log("GstVideoUtils - update(): GstVideoUtils not loaded", ELL_WARNING,EVL_Heavy);
		bIsFrameNew = false;
		bHavePixelsChanged = false;
		return false;
	}
	bIsFrameNew = bHavePixelsChanged;
	bHavePixelsChanged = false;
	return bIsFrameNew;
}

float GstVideoUtils::getHeight(){
	return pixels.Size.y;
}

float GstVideoUtils::getWidth(){
	return pixels.Size.x;
}

bool GstVideoUtils::setPipeline(const std::string& pipeline, int bpp, bool isStream, int w, int h){
	core::string caps;

#if GST_VERSION_MAJOR==0
	if (bpp == 8)
		caps = "video/x-raw-gray, depth=8, bpp=8";
	else if (bpp == 32)
		caps = "video/x-raw-rgb, depth=24, bpp=32, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
	else
		caps = "video/x-raw-rgb, depth=24, bpp=24, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
#else
	if (bpp == 8)
		caps = "video/x-raw, format=GRAY8";
	else if (bpp == 32)
		caps = "video/x-raw, format=RGBA";
	else
		caps = "video/x-raw, format=RGB";
#endif

	if (w != -1 && h != -1){
		caps += ", width=" + core::StringConverter::toString(w) + ", height=" + core::StringConverter::toString(h);
	}

	gchar* pipeline_string =
		g_strdup((pipeline + " ! appsink name=ofappsink caps=\"" + caps + "\"").c_str()); // caps=video/x-raw-rgb

	if ((isStream && (w == -1 || h == -1)) || allocate(w, h, bpp)){
		return setPipelineWithSink(pipeline_string, "ofappsink", isStream);
	}
	else{
		return false;
	}
}

bool GstVideoUtils::allocate(int w, int h, int _bpp){

	pixels.createData(math::vector3di(w, h, 1), video::EPixel_R8G8B8);
	backPixels.createData(math::vector3di(w, h, 1), video::EPixel_R8G8B8);
	prevBuffer = 0;
	bHavePixelsChanged = 1;

	return true;
}

#if GST_VERSION_MAJOR==0
int GstVideoUtils::preroll_cb(GstBuffer * _buffer){
	guint size = GST_BUFFER_SIZE(_buffer);
	if (pixels.imageData && pixels.Size.x*pixels.Size.y * 3/*pixels.getBytesPerPixel()*/ > (int)size){
		gLogManager.log("GstVideoUtils - preproll_cb(): error preroll buffer size: " + core::StringConverter::toString(size)
			+ "!= init size: " + core::StringConverter::toString(pixels.Size.x*pixels.Size.y * 3), ELL_WARNING);
		gst_buffer_unref(_buffer);
		return GST_FLOW_ERROR;
	}
	mutex->lock();
	if (bBackPixelsChanged && buffer)
		gst_buffer_unref(buffer);
	if (!pixels.imageData)
	{
		if (m_impl->isStream && m_impl->appsink){
			m_impl->appsink->OnStreamPrepared();
		}
		else{
			gLogManager.log("GstVideoUtils = preroll_cb(): received a preroll without allocation", ELL_WARNING);
		}
	}
	if (pixels.imageData){
		buffer = _buffer;
		backPixels.setData(GST_BUFFER_DATA(buffer), pixels.Size, pixels.format);
		eventPixels.setData(GST_BUFFER_DATA(buffer), pixels.Size, pixels.format);
		bBackPixelsChanged = true;
		//ofNotifyEvent(prerollEvent, eventPixels);
	}
	mutex->unlock();
	return m_impl->preroll_cb(_buffer);
}
#else
int GstVideoUtils::preroll_cb(GstSample * sample){
	GstBuffer * _buffer = gst_sample_get_buffer(sample);
	gst_buffer_map(_buffer, &mapinfo, GST_MAP_READ);
	guint size = mapinfo.size;

	if (pixels.imageData && pixels.Size.x*pixels.Size.y * 3/*pixels.getBytesPerPixel()*/ > (int)size){
		gLogManager.log("GstVideoUtils - preproll_cb(): error preroll buffer size: " + core::StringConverter::toString(size)
			+ "!= init size: " + core::StringConverter::toString(pixels.Size.x*pixels.Size.y * 3), ELL_WARNING);
		gst_sample_unref(sample);

		return GST_FLOW_ERROR;
	}
	mutex->lock();
	if (bBackPixelsChanged && buffer) gst_sample_unref(buffer);
	if (!pixels.imageData)
	{
		if (m_impl->isStream && m_impl->appsink){
			m_impl->appsink->OnStreamPrepared();
		}
		else{
			gLogManager.log("GstVideoUtils = preroll_cb(): received a preroll without allocation", ELL_WARNING);
		}
	}
	if (pixels.imageData){
		buffer = sample;
		backPixels.setData(mapinfo.data, pixels.Size, pixels.format);
		eventPixels.setData(mapinfo.data, pixels.Size, pixels.format);
		bBackPixelsChanged = true;
		//ofNotifyEvent(prerollEvent, eventPixels);
	}
	mutex->unlock();
	gst_buffer_unmap(_buffer, &mapinfo);
	return m_impl->preroll_cb(sample);
}
#endif

#if GST_VERSION_MAJOR==0
int GstVideoUtils::buffer_cb(GstBuffer * _buffer){

	guint size;

	size = GST_BUFFER_SIZE(_buffer);
	if (pixels.imageData && pixels.Size.x*pixels.Size.y * 3 > (int)size){
		//	gLogManager.log("GstVideoUtils") << "buffer_cb(): error on new buffer, buffer size: " << size
		//	<< " != init size: " << pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel();
		gst_buffer_unref(_buffer);
		return GST_FLOW_ERROR;
	}
	mutex->lock();
	if (bBackPixelsChanged && buffer)
		gst_buffer_unref(buffer);
	if (pixels.imageDataSize){
		buffer = _buffer;
		backPixels.setData(GST_BUFFER_DATA(buffer), pixels.Size, pixels.format);
		eventPixels.setData(GST_BUFFER_DATA(buffer), pixels.Size, pixels.format);
		bBackPixelsChanged = true;
		mutex->unlock();
		//	ofNotifyEvent(bufferEvent, eventPixels);
	}
	else{
		if (m_impl->isStream && m_impl->appsink){
			m_impl->appsink->OnStreamPrepared();
		}
		else{
			gLogManager.log("received a preroll without allocation", ELL_WARNING);
		}
		mutex->unlock();
	}

	return m_impl->buffer_cb(buffer);
}
#else
int GstVideoUtils::buffer_cb(GstSample * sample){
	GstBuffer * _buffer = gst_sample_get_buffer(sample);
	gst_buffer_map(_buffer, &mapinfo, GST_MAP_READ);
	guint size = mapinfo.size;

	if (pixels.imageData && pixels.Size.x*pixels.Size.y * 3/*pixels.getBytesPerPixel()*/ > (int)size){
		gLogManager.log("GstVideoUtils - preproll_cb(): error preroll buffer size: " + core::StringConverter::toString(size)
			+ "!= init size: " + core::StringConverter::toString(pixels.Size.x*pixels.Size.y * 3), ELL_WARNING);
		gst_sample_unref(sample);

		return GST_FLOW_ERROR;
	}

	mutex->lock();
	if (bBackPixelsChanged && buffer) gst_sample_unref(buffer);

	buffer = sample;
	if (pixels.imageDataSize){
		backPixels.setData(mapinfo.data, pixels.Size, pixels.format);
		eventPixels.setData(mapinfo.data, pixels.Size, pixels.format);
		bBackPixelsChanged = true;
		mutex->unlock();
		//	ofNotifyEvent(bufferEvent, eventPixels);
	}
	else{
		if (m_impl->isStream && m_impl->appsink){
			m_impl->appsink->OnStreamPrepared();
		}
		else{
			gLogManager.log("received a preroll without allocation", ELL_WARNING);
		}
		mutex->unlock();
	}
	gst_buffer_unmap(_buffer, &mapinfo);
}
#endif

void GstVideoUtils::eos_cb(){
	m_impl->eos_cb();
	// 	ofEventArgs args;
	// 	ofNotifyEvent(eosEvent, args);
}
}
}