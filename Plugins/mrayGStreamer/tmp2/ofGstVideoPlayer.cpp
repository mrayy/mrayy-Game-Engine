/*
* ofGstVideoUtils.cpp
*
*  Created on: 16/01/2011
*      Author: arturo
*/

#include "stdafx.h"
#include "ofGstVideoPlayer.h"
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include "ILogManager.h"
#include "INetwork.h"
#include <gst/app/gstappsrc.h>

#include "CMySrc.h"

ofGstVideoPlayer::ofGstVideoPlayer(){
	nFrames = 0;
	internalPixelFormat = video::EPixel_R8G8B8;
	bIsStream = false;
	bIsAllocated = false;
	threadAppSink = false;
	videoUtils.setSinkListener(this);

	m_readData[0].client = network::INetwork::getInstance().createUDPClient();
	m_readData[1].client = network::INetwork::getInstance().createUDPClient();

}

ofGstVideoPlayer::~ofGstVideoPlayer(){
	close();


	delete m_readData[0].client;
	m_readData[0].client = 0;

	delete m_readData[1].client;
	m_readData[1].client = 0;
}

bool ofGstVideoPlayer::setPixelFormat(video::EPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
	return true;
}

video::EPixelFormat ofGstVideoPlayer::getPixelFormat(){
	return internalPixelFormat;
}

bool ofGstVideoPlayer::loadMovie(core::string name){
	close();
	if (name.find("file://", 0) != core::string::npos){
		bIsStream = false;
	}
	else if (name.find("://", 0) == core::string::npos){
		//name = "file://" + ofToDataPath(name, true);
		bIsStream = false;
	}
	else{
		bIsStream = true;
	}
	core::string message = "ofGstVideoPlayer - loadMovie(): loading \"" +name + "\"";
	gLogManager.log(message, ELL_INFO,EVL_Heavy);

	ofGstUtils::startGstMainLoop();

#if GST_VERSION_MAJOR==0
	GstElement * gstPipeline = gst_element_factory_make("playbin2", "player");
#else
	GstElement * gstPipeline = gst_element_factory_make("playbin", "player");
#endif
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "app_sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
	gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink), 8);
	gst_app_sink_set_drop(GST_APP_SINK(gstSink), true);
	gst_base_sink_set_max_lateness(GST_BASE_SINK(gstSink), -1);

#if GST_VERSION_MAJOR==0
	int bpp;
	core::string mime;
	switch (internalPixelFormat){
	case video::EPixel_LUMINANCE8:
		mime = "video/x-raw-gray";
		bpp = 8;
		break;
	case video::EPixel_R8G8B8:
		mime = "video/x-raw-rgb";
		bpp = 24;
		break;
	case video::EPixel_R8G8B8A8:
	case video::EPixel_B8G8R8A8:
		mime = "video/x-raw-rgb";
		bpp = 32;
		break;
	default:
		mime = "video/x-raw-rgb";
		bpp = 24;
		break;
	}

	GstCaps *caps = gst_caps_new_simple(mime.c_str(),
		"bpp", G_TYPE_INT, bpp,
		"depth", G_TYPE_INT, 24,
		"endianness", G_TYPE_INT, 4321,
		"red_mask", G_TYPE_INT, 0xff0000,
		"green_mask", G_TYPE_INT, 0x00ff00,
		"blue_mask", G_TYPE_INT, 0x0000ff,
		"alpha_mask", G_TYPE_INT, 0x000000ff,
		NULL);
#else
	int bpp;
	string mime = "video/x-raw";
	string format;
	switch (internalPixelFormat){
	case OF_PIXELS_MONO:
		format = "GRAY8";
		bpp = 8;
		break;
	case OF_PIXELS_RGB:
		format = "RGB";
		bpp = 24;
		break;
	case OF_PIXELS_RGBA:
		format = "RGBA";
		bpp = 32;
		break;
	case OF_PIXELS_BGRA:
		format = "BGRA";
		bpp = 32;
		break;
	default:
		format = "RGB";
		bpp = 24;
		break;
	}

	GstCaps *caps = gst_caps_new_simple(mime.c_str(),
		"format", G_TYPE_STRING, format.c_str(),
		/*"bpp", G_TYPE_INT, bpp,
		"depth", G_TYPE_INT, 24,
		"endianness",G_TYPE_INT,4321,
		"red_mask",G_TYPE_INT,0xff0000,
		"green_mask",G_TYPE_INT,0x00ff00,
		"blue_mask",G_TYPE_INT,0x0000ff,
		"alpha_mask",G_TYPE_INT,0x000000ff,*/
		NULL);
#endif


	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);

	if (threadAppSink){
		GstElement * appQueue = gst_element_factory_make("queue", "appsink_queue");
		g_object_set(G_OBJECT(appQueue), "leaky", 0, "silent", 1, (void*)NULL);
		GstElement* appBin = gst_bin_new("app_bin");
		gst_bin_add(GST_BIN(appBin), appQueue);
		GstPad* appQueuePad = gst_element_get_static_pad(appQueue, "sink");
		GstPad* ghostPad = gst_ghost_pad_new("app_bin_sink", appQueuePad);
		gst_object_unref(appQueuePad);
		gst_element_add_pad(appBin, ghostPad);

		gst_bin_add_many(GST_BIN(appBin), gstSink, NULL);
		gst_element_link_many(appQueue, gstSink, NULL);

		g_object_set(G_OBJECT(gstPipeline), "video-sink", appBin, (void*)NULL);
	}
	else{
		g_object_set(G_OBJECT(gstPipeline), "video-sink", gstSink, (void*)NULL);
	}

#ifdef TARGET_WIN32
	GstElement *audioSink = gst_element_factory_make("directsoundsink", NULL);
	g_object_set(G_OBJECT(gstPipeline), "audio-sink", audioSink, (void*)NULL);

#endif


	videoUtils.setPipelineWithSink(gstPipeline, gstSink, bIsStream);
	videoUtils.startPipeline();
	if (!bIsStream) return allocate(bpp);
	else return true;
}

static void
pad_added_cb(GstElement* rtpbin, GstPad* new_pad, GstElement* depay)
{
	GstPad* sinkpad;
	GstPadLinkReturn lres;
	g_print("new payload on pad: %s\n", GST_PAD_NAME(new_pad));
	sinkpad = gst_element_get_static_pad(depay, "sink");
	assert(sinkpad);
	lres = gst_pad_link(new_pad, sinkpad);
	assert(lres == GST_PAD_LINK_OK);
	gst_object_unref(sinkpad);
}
#ifdef OLD_IMPL
bool	ofGstVideoPlayer::Connect(const core::string& ip, int remotePort, int localPort)
{

	GstElement * gstPipeline, *rtp_src, *rtcp_src, *rtcp_sink, *depayloader, *h264_decoder, *csp_filter, *screen_sink, *rtp_bin;
	GstPadLinkReturn lres;
	GstPad* srcpad,
		*sinkpad;
	GstCaps* caps;
	GstBus* bus;


	close();
	
	core::string message = "ofGstVideoPlayer - Connect(): connecting to \"" + ip + "\"";
	gLogManager.log(message, ELL_INFO, EVL_Heavy);

	ofGstUtils::startGstMainLoop();
	bIsStream = true;
	bRemote = true;

	// Create pipeline and attach a callback to it's message bus
	gstPipeline =  gst_pipeline_new("pipeline0");
	g_signal_connect(gstPipeline, "deep-notify", G_CALLBACK(gst_object_default_deep_notify), NULL);
	bus = gst_pipeline_get_bus(GST_PIPELINE(gstPipeline));
	//	gst_bus_add_watch( bus, ( GstBusFunc )gstreamer_bus_callback, pipeline );
	gst_object_unref(GST_OBJECT(bus));

	// Create elements
	rtp_src = gst_element_factory_make("udpsrc", "rtp_src");
	rtcp_src = gst_element_factory_make("udpsrc", "rtcp_src");
	rtcp_sink = gst_element_factory_make("udpsink", "rtcpsink");
	depayloader = gst_element_factory_make("rtph264depay", "h264_deplayloader");
	h264_decoder = gst_element_factory_make("ffdec_h264", "h264_decoder");
	csp_filter = gst_element_factory_make("ffmpegcolorspace", "csp_filter");
	 screen_sink = gst_element_factory_make("appsink", NULL);//( "autovideosink", "screen_sink" );
	rtp_bin = gst_element_factory_make("gstrtpbin", "rtpbin");

	assert(rtp_src);
	assert(rtcp_src);
	assert(rtcp_sink);
	assert(depayloader);
	assert(h264_decoder);
	assert(csp_filter);
	assert(screen_sink);
	assert(rtp_bin);

#if 0
	g_object_set(G_OBJECT(rtp_src), "sockfd", call->rtp_socket, NULL);
	g_object_set(G_OBJECT(rtp_src), "port", (int)call->local_video_port, NULL);
	g_object_set(G_OBJECT(rtp_src), "multicast-group", call->local_ip, NULL);

	g_object_set(G_OBJECT(rtcp_src), "sockfd", call->rtcp_socket, NULL);
	g_object_set(G_OBJECT(rtcp_src), "multicast-group", call->local_ip, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "sockfd", call->rtcp_socket, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "port", call->remote_video_port + 1, "host", call->remote_ip, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "async", FALSE, "sync", FALSE, NULL);
#else

	g_object_set(rtp_src, "port", localPort, NULL);
	g_object_set(G_OBJECT(rtcp_src), "port", localPort, NULL);
	g_object_set(G_OBJECT(rtcp_sink), "port", remotePort, "host", ip.c_str(), NULL);
	g_object_set(G_OBJECT(rtcp_sink), "async", FALSE, "sync", FALSE, NULL);
#endif
	//g_object_set( G_OBJECT( rtp_bin ), "latency", 100, NULL );

	// Add elements to the pipeline. This has to be done prior to linking them
	gst_bin_add_many(GST_BIN(gstPipeline), rtp_src, rtcp_src, rtcp_sink, depayloader, h264_decoder, csp_filter, NULL);

	//gstSink = gst_element_factory_make("appsink", NULL);
	caps = gst_caps_new_simple("video/x-raw-rgb",
		"depth", G_TYPE_INT, 24,
		NULL);
	gst_app_sink_set_caps(GST_APP_SINK(screen_sink), caps);
	gst_caps_unref(caps);
	gst_base_sink_set_sync(GST_BASE_SINK(screen_sink), false);

	g_object_set(G_OBJECT(gstPipeline), "video-sink", screen_sink, (void*)NULL);
#if 0
	if (!gst_element_link_filtered(rtp_src, depayloader, caps))
	{
		assert(!"error linking CAPS\n");
	}

	gst_caps_unref(caps);
#endif

	/* Specify the RTP stream that we expect */
	caps = gst_caps_new_simple("application/x-rtp",
		"clock-rate", G_TYPE_INT, 90000,
		"payload", G_TYPE_INT, 96,
		"encoding-name", G_TYPE_STRING, "H264",
		NULL);
	g_object_set(rtp_src, "caps", caps, NULL);
	gst_caps_unref(caps);

	if (!gst_element_link_many(depayloader, h264_decoder, csp_filter, /*screen_sink,*/ NULL))
	{
		assert(!"error linking elements\n");
	}

	gst_bin_add(GST_BIN(gstPipeline), rtp_bin);

	// now link all to the rtpbin, start by getting an RTP sinkpad for session 0
	srcpad = gst_element_get_static_pad(rtp_src, "src");
	sinkpad = gst_element_get_request_pad(rtp_bin, "recv_rtp_sink_0");
	lres = gst_pad_link(srcpad, sinkpad);
	assert(lres == GST_PAD_LINK_OK);
	gst_object_unref(srcpad);

	// get an RTCP sinkpad in session 0
	srcpad = gst_element_get_static_pad(rtcp_src, "src");
	sinkpad = gst_element_get_request_pad(rtp_bin, "recv_rtcp_sink_0");
	lres = gst_pad_link(srcpad, sinkpad);
	assert(lres == GST_PAD_LINK_OK);
	gst_object_unref(srcpad);
	gst_object_unref(sinkpad);

	// get an RTCP srcpad for sending RTCP back to the sender
	srcpad = gst_element_get_request_pad(rtp_bin, "send_rtcp_src_0");
	sinkpad = gst_element_get_static_pad(rtcp_sink, "sink");
	lres = gst_pad_link(srcpad, sinkpad);
	assert(lres == GST_PAD_LINK_OK);
	gst_object_unref(sinkpad);

	g_signal_connect(rtp_bin, "pad-added", G_CALLBACK(pad_added_cb), depayloader);
	/* give some stats when we receive RTCP */
	//	g_signal_connect( rtp_bin, "on-ssrc-active", G_CALLBACK( on_ssrc_active_cb ),depayloader );

	gst_element_set_state(gstPipeline, GST_STATE_PLAYING);
	bIsStream = true;
	videoUtils.setPipelineWithSink(gstPipeline, screen_sink, bIsStream);
	videoUtils.startPipeline();
	 return true;
}
#else

#define MAX_PACKET 65536
static char TmpPacket[2][MAX_PACKET];

#define BUFF_SIZE (1024)
typedef ofGstVideoPlayer::CReadData gst_app_t;
static gboolean read_data_tobuffer(gst_app_t *app,GstBuffer** b)
{
	if (!app->client)
	{
		gst_app_src_end_of_stream((GstAppSrc*)app-> src);
		return false;
	}
	GstBuffer *buffer;
	guint8 *ptr;
	gint size;
	uint len = BUFF_SIZE;
	network::NetAddress address = app->address;
	len = MAX_PACKET;
	network::UDPClientError err = app->client->RecvFrom(TmpPacket[app->index], &len, &address);
	if (err != network::UDP_SOCKET_ERROR_NONE || len == 0)
	{
		printf("Failed to read UDP packet!\n");
		return false;
	}
//	printf("UDP Packet received:%d\n", len);
	ptr = (guint8*)g_malloc(len);
	memcpy(ptr, TmpPacket[app->index], len);
	g_assert(ptr);
	size = len;
	buffer = gst_buffer_new();
	GST_BUFFER_MALLOCDATA(buffer) = ptr;
	GST_BUFFER_SIZE(buffer) = size;
	GST_BUFFER_DATA(buffer) = GST_BUFFER_MALLOCDATA(buffer);

	//printf("%d -> %d\n", app->index, size);
	*b = buffer;

	return TRUE;
}

static gboolean read_data(gst_app_t *app)
{
	GstFlowReturn ret;

	GstBuffer *buffer;
	if (read_data_tobuffer(app, &buffer))
	{
		ret = gst_app_src_push_buffer((GstAppSrc*)(app->src), buffer);
		if (ret != GST_FLOW_OK){
			ret = gst_app_src_end_of_stream((GstAppSrc*)app->src);
			return FALSE;
		}
	}
	return TRUE;

}
GstFlowReturn read_buffer_data(GstMySrc * sink, gpointer data, GstBuffer ** buffer)
{
	read_data_tobuffer((ofGstVideoPlayer::CReadData*)data,buffer);
	return GST_FLOW_OK;
}

static void start_feed(GstAppSrc *source, guint size, gpointer d)
{
	ofGstVideoPlayer::CReadData * app = (ofGstVideoPlayer::CReadData*)d;
	if (app->sourceID == 0) {
		GST_DEBUG("start feeding");
		app->sourceID = g_idle_add( (GSourceFunc)read_data , app);
	}
}


static void stop_feed(GstAppSrc *source, gpointer d)
{
	ofGstVideoPlayer::CReadData * app = (ofGstVideoPlayer::CReadData*)d;
	if (app->sourceID != 0) {
		GST_DEBUG("stop feeding");
		g_source_remove(app->sourceID);
		app->sourceID = 0;
	}
}

static void on_pad_added(GstElement *element, GstPad *pad,gpointer data)
{
	GstPad *sinkpad;
	GstElement *depay = (GstElement *)data;

	/* We can now link this pad with the vorbis-decoder sink pad */
	g_print("Dynamic pad created, linking parse/depay\n");

	sinkpad = gst_element_get_static_pad(depay, "sink");

	gst_pad_link(pad, sinkpad);

	gst_object_unref(sinkpad);

}

bool	ofGstVideoPlayer::Connect(const core::string& ip, int videoPort, int audioPort, int localAudioPort)
{
	GstElement * gstPipeline, *src,*mpg,*depay,*dec,*sink;
	GstCaps* caps;
	GstBus* bus;


	close();

	core::string message = "ofGstVideoPlayer - Connect(): connecting to \"" + ip + "\"";
	gLogManager.log(message, ELL_INFO, EVL_Heavy);

	//ofGstUtils::startGstMainLoop();
	bIsStream = true;
	bRemote = true;
	GError *err=0;
	// Create pipeline and attach a callback to it's message bus
	/*
	GstElement* src = gst_element_factory_make("appsrc", "src");
	GstElement* pay = gst_element_factory_make("appsrc", "src");
	GstElement* dec = gst_element_factory_make("appsrc", "src");
	GstElement* mpg = gst_element_factory_make("appsrc", "src");
	GstElement* sink = gst_element_factory_make("appsrc", "src");*/


	core::string capsStr = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)THEORA,sampling=(string)\"YCbCr-4:2:0\", delivery-method=(string)inline, payload=(int)96";// , width = (string)1280, height = (string)800, configuration = (string)\"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\\=\\=\" , ssrc = (uint)566098589, clock-base = (uint)363761639, seqnum-base = (uint)19709";

	const char configuration[] =
		"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\\=\\=";
		
	//capsStr = "video/x-theora";												   
	core::string gstString = "appsrc name=src !" + capsStr + "! .recv_rtp_sink  gstrtpsession name = session !rtptheoradepay name = depay " // "//+ capsStr+//" 
		" !theoradec name = dec ! ffmpegcolorspace ! appsink name=sink";// sync = false";

	gstString = "appsrc name=src ! application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=H264 "
		"!  rtph264depay name=depay ! ffdec_h264 name=dec"
		"!video/x-raw-rgb!ffmpegcolorspace ! appsink name=sink sync=false";

//	gstString = "videotestsrc name=src !video/x-raw-rgb,width=1280,height=800! ffmpegcolorspace! vp8enc quality=0 speed=5 name=enc ! rtpvp8pay ! rtpvp8depay! vp8dec name=dec ! ffmpegcolorspace ! video/x-raw-rgb! appsink name=sink sync=false";
//	gstString = "appsrc name=src !application/x-rtp,media=video,clock-rate=90000,payload=96,encoding-name=\"VP8-DRAFT-IETF-01\" !  rtpvp8depay! vp8dec name=dec ! ffmpegcolorspace ! video/x-raw-rgb! appsink name=sink sync=false";
//	gstString = "appsrc name=src !application/x-rtp,media=video,clock-rate=90000,payload=96,encoding-name=VP8-DRAFT-IETF-01 !  rtpvp8depay!  appsink name=sink caps=video/x-vp8,width=1280,height=800 sync=false";

	gstString = "appsrc name=src ! application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=H264 !  rtph264depay name=depay ! ffdec_h264 name=dec skip-frame=5 max-threads=0 ! ffmpegcolorspace ! video/x-raw-rgb  ! appsink name=sink sync=false";
	/*
	 "appsrc name=src !  application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=THEORA ! rtptheoradepay name=depay ! video/x-theora !theoradec name = dec "//! rtptheoradepay name=depay"//
	 //"encoding-name=h264 ! rtph264depay caps=video/x-h264 name=depay queue-delay=0 max-threads=0! ffdec_h264 name=dec"
	 "  ! ffmpegcolorspace ! appsink name=sink sync=true"*/


	gstString = "appsrc name=src ! application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=H264 !  rtph264depay name=depay !"
		" ffdec_h264 name=dec skip-frame=5 max-threads=0 ! ffmpegcolorspace ! video/x-raw-rgb  ! tee name=videoTee " //"appsink name=sink sync=false";
		" appsrc name=audioSrc ! vorbisdec ! audioconvert ! audio/x-raw-int ! tee name=audioTee "
		"appsink name=sink sync=false "
		"fakesink name=audioSink sync=false "
		" videoTee. ! queue ! sink."
		" audioTee. ! queue ! audioSink.";

	//skip-frame=5 
	gstString = "appsrc name=src ! application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=H264 !  rtph264depay name=depay !"
		" ffdec_h264 name=dec max-threads=0 ! ffmpegcolorspace ! video/x-raw-rgb  ! appsink name=sink sync=false " //"appsink name=sink sync=false";
		" mysrc name=audioSrc !  audio/x-flac, channels=1, rate=8000! flacdec ! audio/x-raw-int,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1 ! audioconvert ! autoaudiosink name=audioSink sync=false ";
	//! audio/x-raw-int,endianness=1234,signed=true,width=16,depth=16,rate=441000,channels=2  
	//! ! vorbisdec  audio/x-vorbis
	//oggdemux
	//
	// Create elements
	// 
	//skip-frame=5 
	gstString = "appsrc name=src ! application/x-rtp, media=video, clock-rate=90000, payload=96, encoding-name=VP8-DRAFT-IETF-01 !  rtph264depay name=depay !"
		"  vp8dec name=dec ! ffmpegcolorspace  ! appsink name=sink sync=false "; // "appsink name=sink sync=false";
	//	" dshowaudiosrc !  audioconvert ! autoaudiosink name=audioSink sync=false ";


	gstString = "gst-launch -v gstrtpbin name=rtpbin "
		"mysrc name=rtpsrc caps=application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)VP8-DRAFT-IETF-01  ! rtpbin.recv_rtp_sink_0 "
		"rtpbin. ! rtpvp8depay ! vp8dec ! ffmpegcolorspace ! appsink name=sink sync=false"
		"mysrc name=rtcpsrc ! rtpbin.recv_rtcp_sink_0 "
		"rtpbin.send_rtcp_src_0 ! mysink name=rtcpsink  sync=false async=false "; // "appsink name=sink sync=false";
#if 0 
	gstPipeline = gst_pipeline_new("pipeline0");
	src = gst_element_factory_make("appsrc", "src");
	depay = gst_element_factory_make("rtpvp8depay", "depay");//rtptheoradepay,rtph264depay,rtph264depay
	dec = gst_element_factory_make("vp8dec", "dec");//theoradec//ffdec_h264,ffdec_h264
	mpg = gst_element_factory_make("ffmpegcolorspace", "mpg");
	sink = gst_element_factory_make("appsink", "sink");//( "autovideosink", "screen_sink" );
	GstElement* capsFilter = gst_element_factory_make("capsfilter", NULL);
	if (!src || !capsFilter || !mpg || !depay || !dec || !sink)
	{
		gLogManager.log("GstVideoPlayer()- elements not created", ELL_WARNING);
		return false;
	}
	gst_bin_add_many(GST_BIN(gstPipeline), src, capsFilter, depay, dec, mpg, sink, NULL);
#else
	gstPipeline = gst_parse_launch(gstString.c_str(), &err);

	src = gst_bin_get_by_name(GST_BIN(gstPipeline), "src");
	GstMySrc* audioSrc = GST_MySRC(gst_bin_get_by_name(GST_BIN(gstPipeline), "audioSrc"));
	depay = gst_bin_get_by_name(GST_BIN(gstPipeline), "depay");//rtptheoradepay,rtph264depay
	dec = gst_bin_get_by_name(GST_BIN(gstPipeline), "dec");//theoradec//ffdec_h264
	mpg = gst_bin_get_by_name(GST_BIN(gstPipeline), "mpg");
	sink = gst_bin_get_by_name(GST_BIN(gstPipeline), "sink");//( "autovideosink", "screen_sink" );

	GstElement* rtpsrc = gst_bin_get_by_name(GST_BIN(gstPipeline), "rtpsrc");
	GstElement* rtcpsrc = gst_bin_get_by_name(GST_BIN(gstPipeline), "rtcpsrc");
	GstElement* rcpsink = gst_bin_get_by_name(GST_BIN(gstPipeline), "rtcpsink");


	caps = gst_caps_new_simple("application/x-rtp",
		"media", G_TYPE_STRING, "video",
		"clock-rate", G_TYPE_INT, 90000,
		"payload", G_TYPE_INT, 96,
		"encoding-name", G_TYPE_STRING, "VP8-DRAFT-IETF-01",//"VP8-DRAFT-IETF-01",//"H264",//THEORA
		//		"framerate", GST_TYPE_FRACTION
		//		"sampling", G_TYPE_STRING, "YCbCr-4:2:0",
		// 		"width", G_TYPE_STRING, "1280",
		// 		"height", G_TYPE_STRING, "800",
		// 		"clock-base", G_TYPE_UINT, 1482474968,
		// 		"seqnum-base", G_TYPE_UINT, 447,
		// 		"delivery-method",G_TYPE_STRING, "inline",
		// 		"configuration", G_TYPE_STRING, configuration,
		NULL);

	//g_object_set(depay, "ssrc", 1234567, NULL);

//	gst_app_src_set_caps(GST_APP_SRC(src), caps);
	gst_caps_unref(caps);
#endif
	g_signal_connect(gstPipeline, "deep-notify", G_CALLBACK(gst_object_default_deep_notify), NULL);


// 	caps = gst_caps_from_string(capsStr.c_str());
// 	gst_app_src_set_caps(GST_APP_SRC(src), caps);
// 	gst_caps_unref(caps);

		

	caps = gst_caps_new_simple("video/x-vp8",//theora
//		"ssrc", G_TYPE_UINT, 1234567,
		NULL);
	//gst_app_src_set_caps(GST_APP_SRC(src), caps);
//	g_object_set(depay, "caps", caps, NULL);
	gst_caps_unref(caps);

	int bpp;
	core::string mime;
	switch (internalPixelFormat){
	case video::EPixel_LUMINANCE8:
		mime = "video/x-raw-gray";
		bpp = 8;
		break;
	case video::EPixel_R8G8B8:
		mime = "video/x-raw-rgb";
		bpp = 24;
		break;
	case video::EPixel_R8G8B8A8:
	case video::EPixel_B8G8R8A8:
		mime = "video/x-raw-rgb";
		bpp = 32;
		break;
	default:
		mime = "video/x-raw-rgb";
		bpp = 24;
		break;
	}

	caps = gst_caps_new_simple(mime.c_str(),
		"bpp", G_TYPE_INT, bpp,
		"depth", G_TYPE_INT, 24,
		"endianness", G_TYPE_INT, 4321,
		"red_mask", G_TYPE_INT, 0xff0000,
		"green_mask", G_TYPE_INT, 0x00ff00,
		"blue_mask", G_TYPE_INT, 0x0000ff,
		"alpha_mask", G_TYPE_INT, 0x000000ff,
		NULL);
//	gst_app_sink_set_caps(GST_APP_SINK(sink), caps);
	gst_caps_unref(caps);
	/**/
	//g_object_set(G_OBJECT(gstPipeline), "video-sink", sink, (void*)NULL);
	/*
	caps = gst_caps_new_simple("video/x-h264",
		NULL);
	g_object_set(depay,"caps", caps,NULL);
	gst_caps_unref(caps);*/
#if 0
	if (!gst_element_link_many(src, /*capsFilter,*/ depay, dec, mpg, sink, NULL))
	{
		gLogManager.log("GstVideoPlayer()- elements not linked", ELL_WARNING);
		return false;
	}
#endif
	/*
	gst_base_sink_set_sync(GST_BASE_SINK(sink), false);
	gst_app_sink_set_max_buffers(GST_APP_SINK(sink), 8);
	gst_app_sink_set_drop(GST_APP_SINK(sink), true);
	gst_base_sink_set_max_lateness(GST_BASE_SINK(sink), -1);
	*/
	m_connectData.sink = sink;
	m_connectData.dec = dec;
	m_readData[0].src = src;
	m_readData[0].sourceID = 0;

	m_readData[1].src = GST_ELEMENT(audioSrc);
	m_readData[1].sourceID = 0;

	m_readData[0].address = network::NetAddress(ip, videoPort);
	m_readData[0].client->Open(videoPort);
	m_readData[0].index = 0;
	m_readData[0].data = &m_connectData;
	m_readData[0].src = src;

	m_readData[1].address = network::NetAddress(ip, audioPort);
	m_readData[1].client->Open(audioPort);
	m_readData[1].index = 1;
	m_readData[1].data = &m_connectData;
	m_readData[1].src = GST_ELEMENT(audioSrc);
// 	g_signal_connect(src, "need-data", G_CALLBACK(start_feed), &m_connectData);
// 	g_signal_connect(src, "enough-data", G_CALLBACK(stop_feed), &m_connectData);
// 	g_signal_connect(src, "pad-added", G_CALLBACK(on_pad_added), depay);

	GstAppSrcCallbacks srcCB;
	srcCB.need_data = &start_feed;
	srcCB.enough_data = &stop_feed;
	srcCB.seek_data = 0;
	gst_app_src_set_callbacks(GST_APP_SRC(src), &srcCB, &m_readData[0], NULL);

	//gst_app_src_set_callbacks(audioSrc, &srcCB, &m_readData[1], NULL);
	if (audioSrc)
	{
		audioSrc->data = &m_readData[1];
		audioSrc->need_buffer = read_buffer_data;
	}


	if (threadAppSink){
		GstElement * appQueue = gst_element_factory_make("queue", "appsink_queue");
		g_object_set(G_OBJECT(appQueue), "leaky", 0, "silent", 1, (void*)NULL);
		GstElement* appBin = gst_bin_new("app_bin");
		gst_bin_add(GST_BIN(appBin), appQueue);
		GstPad* appQueuePad = gst_element_get_static_pad(appQueue, "sink");
		GstPad* ghostPad = gst_ghost_pad_new("app_bin_sink", appQueuePad);
		gst_object_unref(appQueuePad);
		gst_element_add_pad(appBin, ghostPad);

		gst_bin_add_many(GST_BIN(appBin), sink, NULL);
		gst_element_link_many(appQueue, sink, NULL);

		g_object_set(G_OBJECT(gstPipeline), "video-sink", appBin, (void*)NULL);
	}
	else{
		g_object_set(G_OBJECT(gstPipeline), "video-sink", sink, (void*)NULL);
	}


	//gst_element_set_state(gstPipeline, GST_STATE_PLAYING);
	bIsStream = true;
	videoUtils.setPipelineWithSink(gstPipeline, sink, bIsStream);
	videoUtils.startPipeline();
	return true;
}
#endif
void ofGstVideoPlayer::setThreadAppSink(bool threaded){
	threadAppSink = threaded;
}


bool ofGstVideoPlayer::allocate(int bpp){
	if (bIsAllocated) return true;

	guint64 durationNanos = videoUtils.getDurationNanos();

	nFrames = 0;
	if (GstPad* pad = gst_element_get_static_pad(videoUtils.getSink(), "sink")){
#if GST_VERSION_MAJOR==0
		int width, height;
		if (gst_video_get_size(GST_PAD(pad), &width, &height)){
			if (!videoUtils.allocate(width, height, bpp)) return false;
		}
		else{
			gLogManager.log("ofGstVideoPlayer - allocate(): couldn't query width and height",ELL_WARNING);
			return false;
		}

		const GValue *framerate = gst_video_frame_rate(pad);
		fps_n = 0;
		fps_d = 0;
		if (framerate && GST_VALUE_HOLDS_FRACTION(framerate)){
			fps_n = gst_value_get_fraction_numerator(framerate);
			fps_d = gst_value_get_fraction_denominator(framerate);
			nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n / (float)fps_d;
			gLogManager.log("ofGstVideoPlayer - allocate(): framerate: " + core::StringConverter::toString(fps_n) + "/" + core::StringConverter::toString(fps_d),ELL_INFO);
		}
		else{
			gLogManager.log("ofGstVideoPlayer - allocate(): cannot get framerate, frame seek won't work",ELL_WARNING);
		}
		bIsAllocated = true;
#else
		if (GstCaps *caps = gst_pad_get_current_caps(GST_PAD(pad))){
			GstVideoInfo info;
			gst_video_info_init(&info);
			if (gst_video_info_from_caps(&info, caps)){
				if (!videoUtils.allocate(info.width, info.height, bpp)) return false;
			}
			else{
				ofLogError("ofGstVideoPlayer") << "allocate(): couldn't query width and height";
				return false;
			}

			fps_n = info.fps_n;
			fps_d = info.fps_d;
			nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n / (float)fps_d;
			gst_caps_unref(caps);
			bIsAllocated = true;
		}
		else{
			ofLogError("ofGstVideoPlayer") << "allocate(): cannot get pipeline caps";
			bIsAllocated = false;
		}
#endif
		gst_object_unref(GST_OBJECT(pad));
	}
	else{
		gLogManager.log("ofGstVideoPlayer - allocate(): cannot get sink pad",ELL_WARNING);
		bIsAllocated = false;
	}

	return bIsAllocated;
}

void ofGstVideoPlayer::on_stream_prepared(){
	if (!bIsAllocated) allocate(24);
}

int	ofGstVideoPlayer::getCurrentFrame(){
	int frame = 0;

	// zach I think this may fail on variable length frames...
	float pos = getPosition();
	if (pos == -1) return -1;


	float  framePosInFloat = ((float)getTotalNumFrames() * pos);
	int    framePosInInt = (int)framePosInFloat;
	float  floatRemainder = (framePosInFloat - framePosInInt);
	if (floatRemainder > 0.5f) framePosInInt = framePosInInt + 1;
	//frame = (int)ceil((getTotalNumFrames() * getPosition()));
	frame = framePosInInt;

	return frame;
}

int	ofGstVideoPlayer::getTotalNumFrames(){
	return nFrames;
}

void ofGstVideoPlayer::firstFrame(){
	setFrame(0);
}

void ofGstVideoPlayer::nextFrame(){
	gint64 currentFrame = getCurrentFrame();
	if (currentFrame != -1) setFrame(currentFrame + 1);
}

void ofGstVideoPlayer::previousFrame(){
	gint64 currentFrame = getCurrentFrame();
	if (currentFrame != -1) setFrame(currentFrame - 1);
}

void ofGstVideoPlayer::setFrame(int frame){ // frame 0 = first frame...
	float pct = (float)frame / (float)nFrames;
	setPosition(pct);
}

bool ofGstVideoPlayer::isStream(){
	return bIsStream;
}

void ofGstVideoPlayer::update(){
	videoUtils.update();
}

void ofGstVideoPlayer::play(){
	videoUtils.play();
}

void ofGstVideoPlayer::stop(){
	videoUtils.stop();
}

void ofGstVideoPlayer::setPaused(bool bPause){
	videoUtils.setPaused(bPause);
}

bool ofGstVideoPlayer::isPaused(){
	return videoUtils.isPaused();
}

bool ofGstVideoPlayer::isLoaded(){
	return videoUtils.isLoaded();
}

bool ofGstVideoPlayer::isPlaying(){
	return videoUtils.isPlaying();
}

float ofGstVideoPlayer::getPosition(){
	return videoUtils.getPosition();
}

float ofGstVideoPlayer::getSpeed(){
	return videoUtils.getSpeed();
}

float ofGstVideoPlayer::getDuration(){
	return videoUtils.getDuration();
}

bool ofGstVideoPlayer::getIsMovieDone(){
	return videoUtils.getIsMovieDone();
}

void ofGstVideoPlayer::setPosition(float pct){
	videoUtils.setPosition(pct);
}

void ofGstVideoPlayer::setVolume(float volume){
	videoUtils.setVolume(volume);
}

void ofGstVideoPlayer::setLoopState(int state){
	videoUtils.setLoopState(state);
}

int ofGstVideoPlayer::getLoopState(){
	return videoUtils.getLoopState();
}

void ofGstVideoPlayer::setSpeed(float speed){
	videoUtils.setSpeed(speed);
}

void ofGstVideoPlayer::close(){
	bIsAllocated = false;
	if (m_readData[0].client)
		m_readData[0].client->Close();

	if (m_readData[1].client)
		m_readData[1].client->Close();
	videoUtils.close();
}

bool ofGstVideoPlayer::isFrameNew(){
	return videoUtils.isFrameNew();
}

unsigned char * ofGstVideoPlayer::getPixels(){
	return videoUtils.getPixels();
}

video::ImageInfo* ofGstVideoPlayer::getPixelsRef(){
	return videoUtils.getPixelsRef();
}

float ofGstVideoPlayer::getHeight(){
	return videoUtils.getHeight();
}

float ofGstVideoPlayer::getWidth(){
	return videoUtils.getWidth();
}

ofGstVideoUtils * ofGstVideoPlayer::getGstVideoUtils(){
	return &videoUtils;
}

void ofGstVideoPlayer::setFrameByFrame(bool frameByFrame){
	videoUtils.setFrameByFrame(frameByFrame);
}