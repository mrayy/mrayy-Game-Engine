

#include "stdafx.h"
#include "GstVideoProvider.h"

#define GST_DISABLE_DEPRECATED
#include <gst/gst.h>
#include <gst/gstpad.h>

#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <gst/app/gstappsrc.h>

#include <glib-object.h>
#include <glib.h>
#include <algorithm>

#include <ILogManager.h>
#include <StringConverter.h>
#include <PixelUtil.h>
#include <IThreadManager.h>
#include "IUDPClient.h"
#include "INetwork.h"

#include <time.h>

#include <windows.h>
#include "CMySink.h"
#include "CMySrc.h"


//http://cgit.freedesktop.org/gstreamer/gstreamer/tree/docs/random/porting-to-1.0.txt
namespace mray
{

	int m_frameRate = 30;
	static bool plugin_registered = false;
	static bool gst_inited = false;
class GstVideoProviderImpl
{
protected:
public:

	GstElement*			gstSrc;
	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;
	GstBus*		bus;
	double		m_time;
	bool				bPlaying;
	bool				bLoaded;
	bool				bPaused;
	bool isAppSink;

	bool m_isLocal;
	bool m_streamAudio;

	ulong m_lastBufferID;
	float m_timeAcc;
	int m_frames;
	int m_wasted;
	int m_sentBytes;
	math::vector2di m_resolution;

	int sourceid;

	CameraInfo m_cam0, m_cam1;

	GstVideoGrabber* src;

	network::IUDPClient* m_udpClient[2];
	network::NetAddress m_targetAddress;
	int m_targetPorts[2];

#define SENDING_BLOCK 4096
	char m_sendingBuffer[SENDING_BLOCK];
	int m_dataOffset;

	ulong m_counter;
public:

	static gboolean push_data(GstVideoProviderImpl *data) {
		//data->NeedData();
		GstBuffer* buffer;// = gst_buffer_new();
		if(data->NeedData(&buffer))
		{
#if GST_VERSION_MAJOR==1
			GstFlowReturn ret;
			g_signal_emit_by_name(data->gstSrc, "push-buffer", buffer, &ret);
			gst_buffer_unref (buffer);
			if(ret!=GST_FLOW_OK)
				return false;
#else
			gst_app_src_push_buffer((GstAppSrc*)(data->gstSrc), buffer);
#endif
		}

#if 0

		GstBuffer *buffer;
		GstFlowReturn ret;
		int i;
		guchar *raw;
		gint num_samples = PIXElS_COUNT * 3 * sizeof(guchar);
		gfloat freq;

		gdouble ms = g_timer_elapsed(data->timer, NULL);
		if (ms > 1.0 / 30.0) {
			/* Create a new empty buffer */
			//buffer = gst_buffer_new_and_alloc(num_samples);

			raw = (guchar*)malloc(num_samples);
			//raw = (guint8 *)GST_BUFFER_DATA(buffer)
			/* Set its timestamp and duration */

			memset(image_pixels, 0, PIXElS_COUNT * 3);
			for (int i = 50; i < 100; ++i)
			{
				for (int j = 100; j < 150; ++j)
				{
					int index = (((i + data->offset) % HEIGHT)*WIDTH + j) * 3;
					image_pixels[index] = 255;
				}
			}
			data->offset += 10;


			for (i = 0; i < num_samples; i++) {
				raw[i] = (guchar)(image_pixels[i]);
			}
			data->dataptr += num_samples;
			data->dataptr = data->dataptr%PIXElS_COUNT;
			data->num_samples += num_samples;

			buffer = gst_buffer_new();
			GST_BUFFER_MALLOCDATA(buffer) = raw;
			GST_BUFFER_SIZE(buffer) = num_samples;
			GST_BUFFER_DATA(buffer) = GST_BUFFER_MALLOCDATA(buffer);
			GST_BUFFER_TIMESTAMP(buffer) = gst_util_uint64_scale(data->num_samples, GST_SECOND, 1);
			GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale(ChunkSize, GST_SECOND, 1);

			//		g_print("Data Count=%d\n", data->num_samples);

			GstCaps* caps = gst_caps_new_simple("video/x-raw-rgb",
				"rate", G_TYPE_INT, 90000,
				"bpp", G_TYPE_INT, 24,
				"depth", G_TYPE_INT, 24,
				"width", G_TYPE_INT, WIDTH,
				"height", G_TYPE_INT, HEIGHT,
				NULL);
			//	GST_BUFFER_CAPS(buffer) = caps;
			/* Push the buffer into the appsrc */
			//ret=gst_app_src_push_buffer((GstAppSrc*)data->src, buffer);
			g_signal_emit_by_name(data->src, "push-buffer", buffer, &ret);

			/* Free the buffer now that we are done with it */
			//	gst_buffer_unref(buffer);
			//free(raw);
			if (ret != GST_FLOW_OK) {
				/* We got some error, stop sending data */
				return FALSE;
			}
			g_timer_start(data->timer);
			return TRUE;

		}
#endif

		return true;
	}


	/* This signal callback triggers when appsrc needs data. Here, we add an idle handler
	* to the mainloop to start pushing data into the appsrc */
	static void start_feed(GstAppSrc *source, guint size, gpointer d) {
		GstVideoProviderImpl* data = (GstVideoProviderImpl*)d;
		if (data->sourceid == 0) {
			data->sourceid = g_idle_add((GSourceFunc)push_data, data);
		}
	}

	/* This callback triggers when appsrc has enough data and we can stop sending.
	* We remove the idle handler from the mainloop */
	static void stop_feed(GstAppSrc *source, gpointer d) {
		GstVideoProviderImpl* data = (GstVideoProviderImpl*)d;

		if (data->sourceid != 0) {
	//		printf("Stop Feed\n");
			g_source_remove(data->sourceid);
			data->sourceid = 0;
		}
	}
	static GstFlowReturn need_buffer(GstMySrc *source, gpointer data, GstBuffer ** buffer) {
		GstVideoProviderImpl* app = (GstVideoProviderImpl*)data;
		app->NeedData(buffer);
		return GST_FLOW_OK;
	}
	/* The appsink has received a buffer */
	static GstFlowReturn new_buffer(GstMySink * sink, gpointer data,GstBuffer* buffer) {
		//GstBuffer *buffer;
		/* Retrieve the buffer */
	//	buffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
		//g_signal_emit_by_name(sink, "pull-buffer", &buffer);
		if (buffer) {
			/* The only thing we do in this example is print a * to indicate a received buffer */
			//	Sleep(10);
			((GstVideoProviderImpl*)data)->SendData(buffer,0);
			//gst_buffer_unref(buffer);
		}
		return GST_FLOW_OK;
	}

	static GstFlowReturn new_audiobuffer(GstMySink * sink, gpointer data, GstBuffer* buffer) {
		//GstBuffer *buffer;
		/* Retrieve the buffer */
		//	buffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
		//g_signal_emit_by_name(sink, "pull-buffer", &buffer);
		if (buffer) {
			/* The only thing we do in this example is print a * to indicate a received buffer */
			//	Sleep(10);
			((GstVideoProviderImpl*)data)->SendData(buffer,1);
			//gst_buffer_unref(buffer);
		}
		return GST_FLOW_OK;
	}
	/* The appsink has received a buffer */
	static GstFlowReturn new_preroll(GstAppSink * sink, void * data) {
#if GST_VERSION_MAJOR == 0
		GstBuffer *buffer;

		/* Retrieve the buffer */
		buffer = gst_app_sink_pull_preroll(GST_APP_SINK(sink));
		if (buffer) {
			/* The only thing we do in this example is print a * to indicate a received buffer */

			//((GstVideoProviderImpl*)data)->SendData(buffer,0);
			gst_buffer_unref(buffer);
		}
#else 
		GstSample *sample;
		GstBuffer* buffer;

		/* Retrieve the buffer */
		sample = gst_app_sink_pull_preroll(GST_APP_SINK(sink));
		if (sample) {
			buffer = gst_sample_get_buffer(sample);
			/* The only thing we do in this example is print a * to indicate a received buffer */

			//((GstVideoProviderImpl*)data)->SendData(buffer,0);
			gst_buffer_unref(buffer);
			gst_sample_unref(sample);
		}
#endif
		return GST_FLOW_OK;
	}
public:
	uchar *pixels;

	int pixelsCount;
	GstVideoProviderImpl()
	{


		if (!gst_inited){
			gst_init(NULL, NULL);

			gst_inited = true;
			gLogManager.log("ofGstUtils - gstreamer inited", ELL_INFO, EVL_Heavy);
		}
		
		if (!plugin_registered){
			gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
				"mysink", (char*)"Element application sink",
				_GstMySinkClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
				"");
			gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
				"mysrc", (char*)"Element application src",
				_GstMySrcClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
				"");
			plugin_registered = true;
		}

		bPlaying = false;
		bLoaded = false;
		bPaused = false;
		sourceid = 0;
		m_time = 0;
		m_dataOffset = 0;

		m_udpClient[0] = 0;
		m_udpClient[1] = 0;
		pixels = 0;
		pixelsCount = 0;

		m_isLocal = false;
		m_streamAudio = true;

		m_cam0.index = 0;
		m_cam1.index = 1;
		m_resolution.set(1280, 720);
	}
	virtual~GstVideoProviderImpl()
	{
		Stop();
		delete[]pixels;
	}

	void SetCameras(const CameraInfo&  cam0, const CameraInfo&  cam1)
	{
		m_cam0 = cam0;
		m_cam1 = cam1;
	}
	void SetTargetResolution(const math::vector2di& res)
	{
		m_resolution = res;
	}

	bool NeedData(GstBuffer** b)
	{
		double t = gEngine.getTimer()->getSeconds();
		double ms = t-m_time;
		if (ms < 1000.0 / (m_frameRate))
		{
			//printf("-");
			return false;
		}

		GstBuffer* buffer;
		src->Lock();

		if (src->GetGrabber()->GetBufferID()==m_lastBufferID)
		{
			m_wasted++;
			//printf("-");
			src->Unlock();
			return false;
		}
		const video::ImageInfo* image = src->GetGrabber()->GetLastFrame();
#if GST_VERSION_MAJOR ==0
		if (!image->imageDataSize)
		{
			src->Unlock();
			return false;
		}
		m_lastBufferID = src->GetGrabber()->GetBufferID();
		if (image->imageDataSize != pixelsCount)
		{
			pixelsCount = image->imageDataSize;
			pixels = new uchar[image->imageDataSize];
		}
		memcpy(pixels, image->imageData, pixelsCount);
#else
		pixelsCount = image->imageDataSize;
		buffer = gst_buffer_new_and_alloc(pixelsCount);
#endif
		//	GST_BUFFER_SIZE(buffer) = 1280 * 720 * 3;
		//	GST_BUFFER_DATA(buffer) = pixels;

		src->Unlock();

#if GST_VERSION_MAJOR ==0
		buffer = gst_buffer_new();
		GST_BUFFER_SIZE(buffer) = pixelsCount;
		GST_BUFFER_DATA(buffer) = pixels;
#else
		GstMapInfo mapinfo;
		gst_buffer_map(buffer, &mapinfo, GST_MAP_WRITE);
		memcpy(mapinfo.data, image->imageData, pixelsCount);
		gst_buffer_unmap(buffer, &mapinfo);

		//gst_buffer_insert_memory(buffer,0,)
#endif

		m_timeAcc += ms;
		m_time = t;
		m_frames++;
		if (m_timeAcc >= 1000)
		{
			printf("%d F/%d B, ", m_frames, m_sentBytes);
			m_timeAcc -= 1000;
			m_frames = 0;
			m_wasted = 0;
			m_sentBytes = 0;
		}

		//time_t  now = time(0);

		//memcpy(GST_BUFFER_DATA(buffer), image->imageData, image->imageDataSize);
		// 		GST_BUFFER_TIMESTAMP(buffer) = gst_util_uint64_scale(m_counter, GST_SECOND, m_frameRate);//m_counter * GST_SECOND/20;// 
		// 
		// 		++m_counter;
		// 		GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale(1, GST_SECOND, m_frameRate) * 2;


		GstFlowReturn ret=GST_FLOW_OK;
		//g_signal_emit_by_name(gstSrc, "push-buffer", buffer, &ret);
		//gst_buffer_unref(buffer);
		//	printf("trying\n");
#if 0

		time_t  now = time(0);

		//memcpy(GST_BUFFER_DATA(buffer), image->imageData, image->imageDataSize);
		GST_BUFFER_TIMESTAMP(buffer) = gst_util_uint64_scale(m_counter, GST_SECOND, m_frameRate);//m_counter * GST_SECOND/20;// 

		++m_counter;
		GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale(1, GST_SECOND, m_frameRate) * 2;

		GstFlowReturn ret;
		//g_signal_emit_by_name(gstSrc, "push-buffer", buffer, &ret);
		ret = gst_app_src_push_buffer((GstAppSrc*)(gstSrc), buffer);


#endif
		//printf("+ ");
		*b = GST_BUFFER_CAST(buffer);

		//gst_buffer_unref(buffer);
		/* Free the buffer now that we are done with it */
		if (ret != GST_FLOW_OK) {
			/* We got some error, stop sending data */
			return FALSE;
		}


		return true;
	}

	bool NeedData()
	{
		GstBuffer* buffer;// = gst_buffer_new();
		return NeedData(&buffer);
	}
	bool NeedDataApp()
	{
		GstBuffer* buffer;// = gst_buffer_new();
		return NeedData(&buffer);
	}

	void SendData(GstBuffer* buffer,int index)
	{
#if GST_VERSION_MAJOR==0
		guint8* data = GST_BUFFER_DATA(buffer);
		int size = GST_BUFFER_SIZE(buffer);
#else
		GstMapInfo mapinfo;
		gst_buffer_map(buffer, &mapinfo, GST_MAP_READ);

		guint8* data = mapinfo.data;
		int size = mapinfo.size;
#endif
		network::NetAddress addr;
		addr.address = m_targetAddress.address;
		addr.port = m_targetPorts[index];
		m_udpClient[index]->SendTo(&addr, (char*)data, size);
		m_sentBytes += size;
//		printf("data sent: %d\n", size);

		printf("%d - ",size);
#if GST_VERSION_MAJOR==1
		gst_buffer_unmap(buffer, &mapinfo);
#endif
		return;
		int remaining = size;
		int offset = 0;
		while (remaining > 0)
		{
			int bytes=math::Min<int>(SENDING_BLOCK-m_dataOffset,remaining);
			memcpy(m_sendingBuffer, data + offset, bytes);
			remaining -= bytes;
			offset += bytes;
			m_dataOffset += bytes;
			if (m_dataOffset >= SENDING_BLOCK)
			{
				m_udpClient[index]->SendTo(&m_targetAddress, (char*)m_sendingBuffer, SENDING_BLOCK);
				m_dataOffset = 0;
			}
		}
	}

	void EnableAudio(bool a)
	{
		m_streamAudio = a;
	}
	void SetNetworkType(bool isLocal)
	{
		m_isLocal = isLocal;
	}
	void SetDataSource(GstVideoGrabber* src)
	{
		this->src = src;
	}
	void StreamDataTo(const network::NetAddress& target, int videoPort, int audioPort)
	{
		m_targetAddress = target;
		m_targetPorts[0] = videoPort;
		m_targetPorts[1] = audioPort;
	}

	bool Start(EStreamingQuality quality)
	{
		close();
		startGstMainLoop();
		GError* err = 0;
		m_frameRate = 15;
		m_timeAcc = 0;
		m_frames = 0;
		core::string gstString = "appsrc name=src   ! ffmpegcolorspace name = mpeg  ! theoraenc"// bitrate=8192, speed-level=1, keyframe-auto=false, keyframe-freq=20"// 
			"!rtptheorapay name = pay ! .send_rtp_sink gstrtpsession .send_rtp_src "// "
			//x264enc profile=high444 speed-preset=ultrafast bitrate=1024 byte-stream=true name = enc !rtph264pay queue-delay=0 name = pay"
			"  ! appsink name = sink ";// !appsink name = sink sync = false", NULL); sync = false

		//gstString = "appsrc name=src  ! ffmpegcolorspace name = mpeg ! videorate !  x264enc name=enc speed-preset=ultrafast sliced-threads=true  ! rtph264pay name=pay !  appsink name=sink ";//bitrate=1024  
		//gstString = "appsrc name=src  ! ffmpegcolorspace name = mpeg ! theoraenc name=enc !   appsink name=sink sync=false";//rtptheorapay name=pay !
		//gstString = "appsrc name=src ! ffmpegcolorspace!  vp8enc  name=enc ! rtpvp8pay name=pay !  appsink name=sink sync=false";
		// profile=high  pass=0
		gstString = "mysrc name=src  ! ffmpegcolorspace! videorate!   x264enc  name=enc profile=baseline pass=qual quantizer=10 speed-preset=ultrafast bitrate=6144 sliced-threads=true ! rtph264pay mtu=1400 ! fakesink name=sink1 sync=false";
		//gstString = "appsrc name=src !  video/x-raw-rgb, width=1408,height=1152,framerate=25/1 !ffmpegcolorspace !  ffenc_h263 name=enc !  rtph263pay name=pay ! rtph263depay ! ffdec_h263! autovideosink name=sink ";//bitrate=1024  
		// 
		core::string pass = "pass1";
		float quanizer = 20;
		int bitrate = 2048;
		int mu = 1400;
		switch (quality)
		{
		case EStreamingQuality::UltraLow:
			pass = "pass1";
			bitrate = 500;
			m_frameRate = 25;

			quanizer = 1;
			break;
		case EStreamingQuality::Low:
			pass = "pass1";
			bitrate = 1000;
			m_frameRate = 25;

			quanizer = 10;

			break;
		case EStreamingQuality::Medium:
			pass = "qual";
			quanizer = 20;
			bitrate = 1500;
			m_frameRate = 25;

			quanizer = 5;

			break;
		case EStreamingQuality::High:
			pass = "qual";
			quanizer = 10;
			bitrate = 2000;
			m_frameRate = 30;


			break;
		case EStreamingQuality::UltraHigh:
			pass = "qual";
			quanizer = 5;
			bitrate = 4000;
			m_frameRate = 30;

			quanizer = 1;

			break;
		default:
			break;
		}
		pass = "pass2";
		quanizer = 1;

		if (m_isLocal)
			mu = 1400 * 5;
		// 		pass = "pass1";
		gstString = "appsrc name=src  ! ffmpegcolorspace! videorate !   x264enc  name=enc rc-lookahead=1 cabac=true pass=" + pass +
			" quantizer=" + core::StringConverter::toString(quanizer) +
			"   speed-preset=ultrafast sliced-threads=false bitrate=" + core::StringConverter::toString(bitrate) +
			"  ! rtph264pay mtu=1400 ! mysink name=sink sync=false ";

		//profile=baseline,speed-preset=ultrafast,sliced-threads=true,tune=fastdecode
		gstString = "appsrc name=src  ! ffmpegcolorspace! videorate!   x264enc  name=enc "// rc - lookahead = 1 cabac = true pass = " + pass +
			//" quantizer=" + core::StringConverter::toString(quanizer) +
			"   speed-preset=ultrafast  sliced-threads=false bitrate=" + core::StringConverter::toString(bitrate) +
			"  ! rtph264pay mtu=1400 ! tee name=videoTee "
			" dshowaudiosrc ! audio/x-raw-int,endianness=1234,signed=true,width=16,depth=16,rate=44100,channels=2!  audioconvert  ! audioresample ! vorbisenc  !tee name=audioTee "  //oggmux max-delay=50 max-page-delay=50 
			"videoTee. ! queue ! mysink name=sink sync=false "
			"audioTee. ! queue ! mysink name=audiosink sync=false";

		//profile=baseline,speed-preset=ultrafast,sliced-threads=true,tune=fastdecode, rc-lookahead=1 cabac=true
#if GST_VERSION_MAJOR==0
		gstString = "appsrc name=src ! queue  ! ffmpegcolorspace !    "
			"x264enc  name=enc"
			" pass=" + pass +
			"  quantizer=" + core::StringConverter::toString(quanizer) +
			"   speed-preset=ultrafast sliced-threads=false bitrate=" + core::StringConverter::toString(bitrate) +
			" tune=zerolatency"
			//" rc-lookahead=1"
			"  ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
			" ! mysink name=sink sync=false ";


		gstString = "appsrc name=src ! queue  ! ffmpegcolorspace !    "
			"ffenc_mpeg4   name=enc "
			"idct-algo=14 dct-algo=3 quant-type=1 gop-size=24 flags=0x00000010 "//qmin=2 qmax=31 
			" pass=2"
			"  quantizer= " + core::StringConverter::toString(quanizer) +
			//	"   bitrate=" + core::StringConverter::toString(bitrate*10) +
			"  ! rtpmp4vpay send-config=true"// mtu=" + core::StringConverter::toString(mu) + 
			// 			" ! rtpmp4vdepay ! ffdec_mpeg4 ! autovideosink";
			" ! mysink name=sink sync=false ";/**/


		gstString = "appsrc name=src ! queue  ! ffmpegcolorspace ! video/x-raw-yuv!     "
			"x264enc  name=enc"
			" pass=" + pass +
			"  quantizer=" + core::StringConverter::toString(quanizer) +
			"   speed-preset=ultrafast sliced-threads=false bitrate=" + core::StringConverter::toString(bitrate) +
			" tune=zerolatency"
			//" rc-lookahead=1"
			"  ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
			//" !gdppay" 
			" ! mysink name=sink sync=false ";


		if (m_cam0.index == m_cam1.index)
		{
			//ksvideosrc
			gstString = "ksvideosrc name=src device-index=" + core::StringConverter::toString(m_cam0.index) + // device=" + m_cam0.guidPath + "" +//
				" ! video/x-raw-yuv,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! ffmpegcolorspace !  videoflip method=4 !";  // videoflip method=1 !   ";

				/*
				"x264enc  name=enc"
				" pass=" + pass +
				"  quantizer=" + core::StringConverter::toString(quanizer) +
				" bitrate=" + core::StringConverter::toString(bitrate) +
				"   speed-preset=ultrafast sliced-threads=false"
				" tune=zerolatency"
				//" rc-lookahead=1"
				"  ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
				//" !gdppay" 
				" ! mysink name=sink sync=false ";*/

		}
		else
		{

			int halfW = m_resolution.x / 2;
			gstString = "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" + core::StringConverter::toString(halfW) + "   sink_2::ypos=0  sink_2::zorder=1  ";
				/*"x264enc  name=enc"
				" pass=" + pass +
				"  quantizer=" + core::StringConverter::toString(quanizer) +
				"   speed-preset=ultrafast sliced-threads=false bitrate=" + core::StringConverter::toString(bitrate) +
				" tune=zerolatency"
				//" rc-lookahead=1"
				"  ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
				//" !gdppay" 
				" ! mysink name=sink sync=false ";*/

			gstString += "videotestsrc pattern=\"black\" ! video/x-raw-yuv,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + " !  mix.sink_0 ";

			//
			gstString += "ksvideosrc name=src1 device-index=" + core::StringConverter::toString(m_cam0.index) + "  ! video/x-raw-yuv,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! ffmpegcolorspace ! videoflip method=4 ! videoscale !"
				"video/x-raw-yuv,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_resolution.y) + " ! mix.sink_1 ";
			//name=src2 device-index=" + core::StringConverter::toString(m_cam1)
			gstString += "ksvideosrc name=src2 device-index=" + core::StringConverter::toString(m_cam1.index) + "  ! video/x-raw-yuv,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! ffmpegcolorspace ! videoflip method=4 ! videoscale ! "
				"video/x-raw-yuv,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_resolution.y) + "! mix.sink_2 ";

			gstString += " mix. ! ";
		}

		//http://mewiki.project357.com/wiki/X264_Encoding_Suggestions#Encoder_latency
		if (true)
		{
		gstString +=
			" x264enc  name=enc"
			//" pass=" + pass +
			//"// qp-min=1 qp-max=" + core::StringConverter::toString(quanizer*5) +
			//"  quantizer=" + core::StringConverter::toString(quanizer) +
			//" sliced-threads=true "//
			" bitrate=" + core::StringConverter::toString(bitrate) +
			" speed-preset=superfast  "
			" tune=zerolatency "
			//	" rc-lookahead=0"
			" sync-lookahead=0 "
			//" option-string=\"slices = 2\" "
			//" ! queue max-size-bytes=100000000 max-size-time=0 "
			" ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
			" ! mysink name=sink sync=false ";
		}
		else
		{
			//Encode using VP8
			gstString +=
				" vp8enc  name=enc"
				" bitrate=" + core::StringConverter::toString(bitrate) +
				" speed=2"
				" ! rtpvp8pay"// mtu=" + core::StringConverter::toString(mu) + 
				" ! mysink name=sink sync=false ";
		}
	//	gstString += "tp. ! queue ! autovideosink sync=false";
		/*
		gstString += "ksvideosrc name=src1 device-index=0 typefind=true ! typefind ! ffmpegcolorspace !  videoscale ! video/x-raw-yuv,width=640,height=720 ! mix.sink_1 ";
		gstString += "ksvideosrc name=src2 device-index=1 typefind=true ! typefind ! ffmpegcolorspace ! videoscale ! video/x-raw-yuv,width=640,height=720! mix.sink_2 ";
		*/
		if (m_streamAudio)
			gstString += " dshowaudiosrc! audio/x-raw-int,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1   ! flacenc quality=2 ! mysink name=audiosink sync=false "; //oggmux max-delay=50 max-page-delay=50 

#else

	if (m_cam0.index == m_cam1.index)
	{
		//ksvideosrc
		gstString = "ksvideosrc name=src device-index=" + core::StringConverter::toString(m_cam0.index) + // device=" + m_cam0.guidPath + "" +//
			" ! video/x-raw,format=I420,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! videoconvert !  videoflip method=4 !";  // videoflip method=1 !   ";

	}
	else
	{

		int halfW = m_resolution.x / 2;
		gstString = "videomixer name=mix sink_0::xpos=0   sink_0::ypos=0  sink_0::alpha=1  sink_0::zorder=0  sink_1::xpos=0   sink_1::ypos=0  sink_1::zorder=1     sink_2::xpos=" + core::StringConverter::toString(halfW) + "   sink_2::ypos=0  sink_2::zorder=1  ";
	
		gstString += "videotestsrc pattern=\"black\" ! video/x-raw,format=RGB,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + " !  mix.sink_0 ";

		//
		gstString += "ksvideosrc name=src1 device-index=" + core::StringConverter::toString(m_cam0.index) + "  ! video/x-raw,format=RGB,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! videoconvert ! videoflip method=4 ! videoscale !"
			"video/x-raw-yuv,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_resolution.y) + " ! mix.sink_1 ";
		//name=src2 device-index=" + core::StringConverter::toString(m_cam1)
		gstString += "ksvideosrc name=src2 device-index=" + core::StringConverter::toString(m_cam1.index) + "  ! video/x-raw,format=RGB,width=" + core::StringConverter::toString(m_resolution.x) + ",height=" + core::StringConverter::toString(m_resolution.y) + ",framerate=30/1 ! videoconvert ! videoflip method=4 ! videoscale ! "
			"video/x-raw-yuv,width=" + core::StringConverter::toString(halfW) + ",height=" + core::StringConverter::toString(m_resolution.y) + "! mix.sink_2 ";

		gstString += " mix. ! ";
	}

	//http://mewiki.project357.com/wiki/X264_Encoding_Suggestions#Encoder_latency
	if (true)
	{
		gstString +=
			" x264enc  name=enc"
			" bitrate=" + core::StringConverter::toString(bitrate) +
			" speed-preset=superfast  "
			" tune=zerolatency "
			" sync-lookahead=0 "
			" ! rtph264pay"// mtu=" + core::StringConverter::toString(mu) + 
			" ! gdppay"
			" ! mysink name=sink sync=false ";
	}
	else
	{
		//Encode using VP8
		gstString +=
			" vp8enc  name=enc"
			" bitrate=" + core::StringConverter::toString(bitrate) +
			" speed=2"
			" ! rtpvp8pay"// mtu=" + core::StringConverter::toString(mu) + 
			" ! mysink name=sink sync=false ";
	}
	if (m_streamAudio)
		gstString += " directsoundsrc! audio/x-raw,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1   ! flacenc quality=2 ! mysink name=audiosink sync=false "; //oggmux max-delay=50 max-page-delay=50 

#endif
	//	gstString += " dshowaudiosrc! audio/x-raw-int,endianness=1234,signed=true,width=16,depth=16,rate=8000,channels=1   ! flacenc ! mysink name=audiosink sync=false "; //oggmux max-delay=50 max-page-delay=50 
		//audio/x-raw-int, endianness=1234, signed=true, width=16, depth=16, rate=22000, channels=1
		//vorbisenc  !

		printf("Connection String: %s\n", gstString.c_str());
		gstPipeline = gst_parse_launch(gstString.c_str(), &err);// !appsink name = sink sync = false", NULL); sync = false
		if (err)
		{
			printf("Pipeline error: %s", err->message);

		}
		if (!gstPipeline)
		{
			gLogManager.log("pipleine couldn't be created!",ELL_ERROR);
			return false;
		}

		gstSrc = gst_bin_get_by_name(GST_BIN(gstPipeline), "src");
		GstElement* enc = gst_bin_get_by_name(GST_BIN(gstPipeline), "enc");
		GstElement* pay = gst_bin_get_by_name(GST_BIN(gstPipeline), "pay");
		GstElement* sink = gst_bin_get_by_name(GST_BIN(gstPipeline), "sink");
		GstMySink* audioSink = GST_MYSINK(gst_bin_get_by_name(GST_BIN(gstPipeline), "audiosink"));
		gstSink = sink;
		g_signal_connect(gstPipeline, "deep-notify", G_CALLBACK(gst_object_default_deep_notify), NULL);

// 
// 		g_signal_connect(gstSrc, "need-data", G_CALLBACK(start_feed), this);
// 		g_signal_connect(gstSrc, "enough-data", G_CALLBACK(stop_feed), this);

		GstMySink* mySink = GST_MYSINK(sink);
		if (mySink)
		{
			mySink->new_buffer = new_buffer;
			mySink->data = this;
		}
		if (audioSink)
		{
			audioSink->new_buffer = new_audiobuffer;
			audioSink->data = this;
		}
#if 0
		GstMySrc* mySrc = GST_MySRC(gstSrc);
		mySrc->need_buffer = need_buffer;
		mySrc->data = this;
#endif
	//	g_object_set(sink, "emit-signals", false, "sync", false, NULL);
		g_object_set(G_OBJECT(gstSink), "sync", FALSE, "async", FALSE, (void*)NULL);
	//	g_object_set(gstSrc, "emit-signals", false, NULL);

//  		g_signal_connect(sink, "new-buffer", G_CALLBACK(new_buffer), this);
// 		g_signal_connect(sink, "new-preroll", G_CALLBACK(new_preroll), this);

		GstAppSinkCallbacks gstCallbacks;
		gstCallbacks.eos = 0;
		gstCallbacks.new_preroll = 0;// &new_preroll;
		//gstCallbacks.new_buffer = &new_buffer;


		GstAppSrcCallbacks srcCB;
		srcCB.need_data = &start_feed;
		srcCB.enough_data = &stop_feed;
		srcCB.seek_data = 0;


		//gst_app_sink_set_callbacks(GST_APP_SINK(gstSink), &gstCallbacks, this, NULL);
		
#if 0
		gst_base_sink_set_sync(GST_BASE_SINK(sink), false);
	//	gst_app_sink_set_max_buffers(GST_APP_SINK(sink), -1);
		gst_app_sink_set_drop(GST_APP_SINK(sink), true);
		gst_base_sink_set_max_lateness(GST_BASE_SINK(sink), -1);
	//	gst_app_src_set_callbacks(GST_APP_SRC(gstSrc), &srcCB, this, NULL);

#endif
		/* Configure appsink */

	

	//	GstCaps* caps;
#if GST_VERSION_MAJOR==0
	//	caps = gst_video_format_new_caps(GST_VIDEO_FORMAT_BGR, src->GetGrabber()->GetFrameSize().x, src->GetGrabber()->GetFrameSize().y, m_frameRate,  1, 4, 3);
#else

		/*caps = gst_caps_new_simple("video/x-raw",
			"format", G_TYPE_STRING, "RGB",
			//"rate", G_TYPE_INT, 180000,
			//"bpp", G_TYPE_INT, 24,
			//"depth", G_TYPE_INT, 24,
 			"width", G_TYPE_INT, src->GetGrabber()->GetFrameSize().x,
			"height", G_TYPE_INT, src->GetGrabber()->GetFrameSize().y,
			NULL);*/
		GstVideoInfo info;
		gst_video_info_set_format(&info, GST_VIDEO_FORMAT_RGB, src->GetGrabber()->GetFrameSize().x, src->GetGrabber()->GetFrameSize().y);
		//caps = gst_video_info_to_caps(&info);
		
#endif
		//g_object_set(gstSrc, "caps", caps, NULL);
		//gst_app_src_set_caps(GST_APP_SRC(gstSrc), caps);
	//	gst_caps_unref(caps);
//
// 		caps = gst_caps_new_simple("application/x-rtp",
// 			"media", G_TYPE_STRING, "video",
// 			"clock-rate", G_TYPE_INT, 90000,
// 			"payload", G_TYPE_INT, 96,
// 			"encoding-name", G_TYPE_STRING, "H264",
// 			NULL);
// 		g_object_set(sink, "caps", caps, NULL);

//		gst_caps_unref(caps);
		m_counter = 0;
		m_wasted = 0;
		m_lastBufferID = -1;
		m_sentBytes = 0;
		try
		{
			for (int i = 0; i < 2; ++i)
			{
				m_udpClient[i] = network::INetwork::getInstance().createUDPClient();
				m_udpClient[i]->Open();
			}
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
		}
		printf("Setting Pipeline\n");
		setPipelineWithSink(gstPipeline, sink, true);
		if (!startPipeline())
		{
			printf("Failed to start pipeline!\n");
			return false;
		}
		play();

		printf("Pipeline is created and ready to stream!\n");
		return true;
	}
	void Stop()
	{
		if (!bLoaded)
			return;
		close();
		delete m_udpClient[0];
		delete m_udpClient[1];
		m_udpClient[0] = 0;
		m_udpClient[1] = 0;

	}

	bool setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream_){
		startGstMainLoop();

		gstPipeline = pipeline;
		gstSink = sink;

		if (gstSink){
		//	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
		}

		if (gstSink && core::string(gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(gst_element_get_factory(gstSink)))) == "appsink"){
			isAppSink = true;
		}
		else{
			isAppSink = false;
		}

		return true;
	}

	bool startPipeline(){

		bPaused = true;

		if (gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
			gLogManager.log("ofGstUtils - startPipeline(): unable to set pipeline to ready", ELL_WARNING);
			return false;
		}
		
		if (gst_element_get_state(GST_ELEMENT(gstPipeline), NULL, NULL, 10 * GST_SECOND) == GST_STATE_CHANGE_FAILURE){
			gLogManager.log("ofGstUtils - startPipeline(): unable to get pipeline ready status", ELL_WARNING);
			return false;
		}

		// pause the pipeline
		if (gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
			gLogManager.log("ofGstUtils - startPipeline(): unable to pause pipeline", ELL_WARNING);
			return false;
		}

	
		bus = gst_pipeline_get_bus(GST_PIPELINE(gstPipeline));

		if (bus){
			gst_bus_add_watch(bus, (GstBusFunc)busFunction, this);
		}



		return true;
	}

	void close(){
			src->Lock();
		if (bPlaying){
			try
			{
				stopPlay();
			}
			catch (...)
			{
				
			}
		}
		if (bLoaded){
			try
			{
 			gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
 			gst_element_get_state(gstPipeline, NULL, NULL, 2 * GST_SECOND);
			// gst_object_unref(gstSink); this crashes, why??
			}
			catch (...)
			{

			}

			gst_object_unref(gstPipeline);
			gstPipeline = NULL;
			gstSink = NULL;
		}
		src->Unlock();
		
		stopGstMainLoop();
		bLoaded = false;
		bPlaying = false;
		bPaused = true;

	}
	void play()
	{
		setPaused(false);
	}
	void setPaused(bool _bPause)
	{
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
	void stopPlay()
	{
		if (!bPlaying)
			return;
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
	static bool loop_initialized ;

	static void startGstMainLoop(){
		if (!loop_initialized){
			mainLoop = new GstMainLoopThread;

			mainLoopThread = OS::IThreadManager::getInstance().createThread(mainLoop);
			mainLoopThread->start(0);
			//mainLoop->start();
			loop_initialized = true;
		}
	}


	static void stopGstMainLoop(){

		if (!loop_initialized)
			return;
		if (!mainLoopThread)
			return;

		g_main_loop_quit(mainLoop->main_loop);
		OS::IThreadManager::getInstance().killThread(mainLoopThread);
		delete mainLoopThread;
		mainLoopThread = 0;
		loop_initialized = false;
	}

	static GMainLoop * getGstMainLoop(){
		return mainLoop->main_loop;
	}

	bool HandleMessage(GstBus * bus, GstMessage * msg);

	static bool			busFunction(GstBus * bus, GstMessage * message, GstVideoProviderImpl * app)
	{
		return app->HandleMessage(bus, message);
	}
	static gboolean appsink_plugin_init(GstPlugin * plugin)
	{
		gst_element_register(plugin, "appsink", GST_RANK_NONE, GST_TYPE_APP_SINK);

		return TRUE;
	}


	class GstMainLoopThread : public OS::IThreadFunction{
	public:
		GMainLoop *main_loop;
		GstMainLoopThread()
			:main_loop(NULL)
		{

		}

		void setup(){
		}
		void execute(OS::IThread*caller, void*arg){
			main_loop = g_main_loop_new(NULL, FALSE);
			g_main_loop_run(main_loop);
		}
	};
	static GstMainLoopThread * mainLoop;
	static OS::IThread* mainLoopThread;
};

GstVideoProviderImpl::GstMainLoopThread * GstVideoProviderImpl::mainLoop = 0;
OS::IThread * GstVideoProviderImpl::mainLoopThread = 0;

bool GstVideoProviderImpl::loop_initialized = false;


bool GstVideoProviderImpl::HandleMessage(GstBus * bus, GstMessage * msg){

	// 	gLogManager.log("ofGstUtils - gstHandleMessage(): got " << GST_MESSAGE_TYPE_NAME(msg)
	// 		<< " message from " << GST_MESSAGE_SRC_NAME(msg));

	switch (GST_MESSAGE_TYPE(msg)) {

	case GST_MESSAGE_BUFFERING:
		gint pctBuffered;
		gst_message_parse_buffering(msg, &pctBuffered);
		break;

#if GST_VERSION_MAJOR==0
	case GST_MESSAGE_DURATION:{
								  GstFormat format = GST_FORMAT_TIME;
	}break;
#else
	case GST_MESSAGE_DURATION_CHANGED:
	//	gst_element_query_duration(gstPipeline, GST_FORMAT_TIME, &durationNanos);
		break;

#endif

	case GST_MESSAGE_STATE_CHANGED:{
									   GstState oldstate, newstate, pendstate;
									   gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
									   if (newstate == GST_STATE_PAUSED && !bPlaying){
										   bLoaded = true;
										   bPlaying = true;
										   if (!bPaused){
											   //   ofLogVerbose("ofGstUtils") << "gstHandleMessage(): setting stream pipeline to play";
											   play();
										   }
									   }

									   //   ofLogVerbose("ofGstUtils") << "gstHandleMessage(): " << GST_MESSAGE_SRC_NAME(msg) << " state changed from "
									   //   << getName(oldstate) << " to " << getName(newstate) << " (" + getName(pendstate) << ")";
	}break;

	case GST_MESSAGE_ASYNC_DONE:
		//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): async done";
		break;

	case GST_MESSAGE_WARNING:
	{
								GError *err;
								gchar *debug;
								gst_message_parse_warning(msg, &err, &debug);
								printf("ofGstUtils - HandleMessage(): Warning- %s\n", debug);
								gLogManager.log("ofGstUtils - HandleMessage(): Warning- " + core::string(debug), ELL_WARNING);
								// 								ofLogVerbose("ofGstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
								// 									<< gst_element_get_name(GST_MESSAGE_SRC(msg)) << "  reported: " << err->message;

								g_error_free(err);
								g_free(debug);

	}break;
	case GST_MESSAGE_ERROR: {
								GError *err;
								gchar *debug;
								gst_message_parse_error(msg, &err, &debug);
								printf("ofGstUtils - HandleMessage(): Error- %s\n" ,debug);
								gLogManager.log("ofGstUtils - HandleMessage(): Error- " + core::string(debug), ELL_WARNING);
								// 								ofLogVerbose("ofGstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
								// 									<< gst_element_get_name(GST_MESSAGE_SRC(msg)) << "  reported: " << err->message;

								g_error_free(err);
								g_free(debug);

								gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

	}break;

	case GST_MESSAGE_EOS:
		//	ofLogVerbose("ofGstUtils") << "gstHandleMessage(): end of the stream";
		
		break;

	default:
		//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): unhandled message from " << GST_MESSAGE_SRC_NAME(msg);
		break;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////


GstVideoProvider::GstVideoProvider()
{
	m_impl = new GstVideoProviderImpl;
	m_connected = 0;
}

GstVideoProvider::~GstVideoProvider()
{
	delete m_impl;
}


void GstVideoProvider::SetDataSource(GstVideoGrabber* src)
{
	m_impl->SetDataSource(src);
}

void GstVideoProvider::StreamDataTo(const network::NetAddress& target, int videoPort, int audioPort)
{
	m_impl->StreamDataTo(target,videoPort,audioPort);
}

void GstVideoProvider::EnableAudio(bool a)
{
	m_impl->EnableAudio(a);
}
void GstVideoProvider::SetNetworkType(bool isLocal)
{
	m_impl->SetNetworkType(isLocal);
}

void GstVideoProvider::Start(EStreamingQuality quality)
{
	m_connected = true;
	m_impl->Start(quality);
}

void GstVideoProvider::Stop()
{
	m_connected = false;
	m_impl->Stop();
}
bool GstVideoProvider::IsConnected()
{
	return m_connected;
}
void GstVideoProvider::SetCameras(const CameraInfo&  cam0, const CameraInfo&  cam1)
{
	m_impl->SetCameras(cam0, cam1);
}

void GstVideoProvider::SetTargetResolution(const math::vector2di& res)
{
	m_impl->SetTargetResolution(res);
}

bool GstVideoProvider::IsStereoCameras()
{
	return m_impl->m_cam0.index != m_impl->m_cam1.index;
}
}
