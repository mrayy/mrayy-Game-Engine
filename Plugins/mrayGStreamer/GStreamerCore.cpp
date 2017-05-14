

#include "stdafx.h"
#include "GStreamerCore.h"

#include "ILogManager.h"
#include <IThreadManager.h>

#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <Windows.h>

#include <glib-object.h>
#include <glib.h>
#include <algorithm>
#include "CMySrc.h"
#include "CMySink.h"
#include "CMyUDPSrc.h"
#include "CMyListener.h"
#include "CMyUDPSink.h"

//#define USE_LIBNICE

#ifdef USE_LIBNICE
#include "nice\gst\gstnicesrc.h"
#include "nice\gst\gstnicesink.h"
#endif

namespace mray
{
namespace video
{

GStreamerCore* GStreamerCore::m_instance=0;
uint GStreamerCore::m_refCount = 0;

#ifdef USE_LIBNICE
static gboolean nicesrc_plugin_init(GstPlugin * plugin){
	return gst_element_register(plugin, "nicesrc", GST_RANK_NONE, GST_TYPE_NICE_SRC);
}
static gboolean nicesink_plugin_init(GstPlugin * plugin){
	return gst_element_register(plugin, "nicesink", GST_RANK_NONE, GST_TYPE_NICE_SINK);
}
#endif

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
		//			startThread();
	}
	void execute(OS::IThread*caller, void*arg){
		main_loop = g_main_loop_new(NULL, FALSE);
		g_main_loop_run(main_loop);
		printf("GST Thread shutdown\n");
	}
};





GStreamerCore::GStreamerCore()
{
	m_mainLoopThread = 0;
	_Init();
}

GStreamerCore::~GStreamerCore()
{
	_StopLoop();
	gst_deinit();
}


void g_logFunction(const gchar   *log_domain,
	GLogLevelFlags log_level,
	const gchar   *message,
	gpointer       user_data)
{
	//printf("GStreamer::%s\n", message);
}

void GStreamerCore::_Init()
{
	GError *err = 0;
	if (!gst_init_check(0,0, &err))
	{
		gLogManager.log("GStreamerCore - Failed to init GStreamer!"+ core::StringConverter::toString(err->message), ELL_ERROR);
	}
	else
	{
 	/*	g_log_set_handler(0, G_LOG_LEVEL_INFO, 0, 0);
 		g_log_set_handler(0, G_LOG_LEVEL_DEBUG, 0, 0);
 		g_log_set_handler(0, G_LOG_LEVEL_MESSAGE, 0, 0);
 		g_log_set_handler(0, G_LOG_LEVEL_CRITICAL, 0, 0);
 		g_log_set_handler(0, G_LOG_FLAG_FATAL , 0, 0);
		g_log_set_default_handler(0, 0);
		g_log_set_handler_full()
		GST_CAT_DEFAULT = 0;
		_gst_debug_min = GST_LEVEL_NONE;

		*/
		_gst_debug_enabled = false;
		g_setenv("GST_DEBUG", "*:0", 1);
		/*
		g_log_set_handler_full("", G_LOG_LEVEL_INFO, g_logFunction, 0, 0);
		g_log_set_handler_full("", G_LOG_LEVEL_DEBUG, g_logFunction, 0, 0);
		g_log_set_handler_full("", G_LOG_LEVEL_MESSAGE, g_logFunction, 0, 0);*/

		fclose(stderr);
		//fclose(stdout);

		//register plugin path
		gLogManager.log("GStreamerCore - Starting Gstreamer", ELL_INFO);
		/*
		core::string gst_path = g_getenv("GSTREAMER_1_0_ROOT_X86");
		if (gst_path != "")
		{
			gst_path = ("GST_PLUGIN_PATH_1_0=" + gst_path + "lib\\gstreamer-1.0" + ";.");
			putenv(gst_path.c_str());
			gLogManager.log("GStreamerCore - GStreamer Plugins Path:" + gst_path, ELL_INFO);
		}
		*/

		gLogManager.log("GStreamerCore - Adding Appsink", ELL_INFO);
		//add our custom src/sink elements
		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			"appsink", (char*)"Element application sink",
			appsink_plugin_init, "0.1", "LGPL", "ofVideoPlayer", "openFrameworks",
			"http://openframeworks.cc/");
// 		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
// 			"mysrc", (char*)"Element application src",
// 			_GstMySrcClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
// 			"");
// 		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
// 			"mysink", (char*)"Element application sink",
// 			_GstMySinkClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
// 			"");
		gLogManager.log("GStreamerCore - Adding myudpsrc", ELL_INFO);
		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			"myudpsrc", (char*)"Element udp src",
			_GstMyUDPSrcClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
			"");
		gLogManager.log("GStreamerCore - Adding myudpsink", ELL_INFO);
		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			"myudpsink", (char*)"Element udp sink",
			_GstMyUDPSinkClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "TELUBee",
			"");

		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
			"mylistener", (char*)"Custom listener element",
			_GstMyListenerClass::plugin_init, "0.1", "LGPL", "GstVideoProvider", "mray",
			"");
#ifdef USE_LIBNICE
		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "nicesrc", strdup("nicesrc"), nicesrc_plugin_init, "1.0.4", "BSD", "libnice", "nice", "http://libnice.org");

		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "nicesink", strdup("nicesink"), nicesink_plugin_init, "1.0.4", "BSD", "libnice", "nice", "http://libnice.org");
#endif
		gLogManager.log("GStreamerCore - GStreamer inited", ELL_INFO);
	}

#if GLIB_MINOR_VERSION<32
	if (!g_thread_supported()){
		g_thread_init(NULL);
	}
#endif
	_StartLoop();
}
void GStreamerCore::_StartLoop()
{
	m_threadFunc = new GstMainLoopThread();
	m_mainLoopThread = OS::IThreadManager::getInstance().createThread(m_threadFunc);
	m_mainLoopThread->start(0);
}

void GStreamerCore::_StopLoop()
{
	if (!m_threadFunc)
		return;
	GstMainLoopThread* mainLoop = (GstMainLoopThread*)m_threadFunc;
	g_main_loop_quit(mainLoop->main_loop);
	bool running = g_main_loop_is_running(mainLoop->main_loop);
	g_main_loop_unref(mainLoop->main_loop);
	delete m_threadFunc;
	OS::IThreadManager::getInstance().killThread(m_mainLoopThread);
	delete m_mainLoopThread;
	m_threadFunc = 0;
	m_mainLoopThread = 0;
}

		
void GStreamerCore::Ref()
{
	m_refCount++;
	if (m_refCount==1)
	{
		m_instance = new GStreamerCore();
	}

}

void GStreamerCore::Unref()
{
	if (m_refCount == 0)
	{
		gLogManager.log("GStreamerCore::Unref() - unreferencing GStreamer with no reference! ", ELL_ERROR);
		return;
	}
	m_refCount--;
	if (m_refCount == 0)
	{
		delete m_instance;
		m_instance = 0;
	}
}

GStreamerCore* GStreamerCore::Instance()
{
	return m_instance;
}

}
}


