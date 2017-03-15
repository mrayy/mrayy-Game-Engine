

#include "stdafx.h"
#include "GstPipelineHandler.h"
#include "ILogManager.h"
#include "GStreamerCore.h"

#define USE_GST_CLOCK 

#ifdef USE_GST_CLOCK
#include <gst/net/gstnet.h>
#endif

namespace mray
{
namespace video
{
	class GstPipelineHandlerImpl
	{
	public:
		GstPipelineHandlerImpl()
		{
			gstPipeline = 0;
			closing = false;
			paused = true;
			Loaded = false;
			playing = false;

#ifdef USE_GST_CLOCK
			baseTime = 0;
			isMasterClock = false;
			clockProvider = 0;

			clock = 0;
			clockPort = 0;
#endif
		}

		guint busWatchID;
		GstElement* gstPipeline;
#ifdef USE_GST_CLOCK
		GstNetTimeProvider* clockProvider;
		GstClock* clock;
		uint clockPort;
		core::string clockIP;
		ulong baseTime;
		bool isMasterClock;
#endif

		bool  paused;
		bool  Loaded;
		bool  playing;
		bool  closing;

	};

	GstPipelineHandler::GstPipelineHandler()
	{
		GStreamerCore::Instance()->Ref();

		m_data = new GstPipelineHandlerImpl();

	}
	GstPipelineHandler::~GstPipelineHandler()
	{
		Close();
		GStreamerCore::Instance()->Unref();
		delete m_data;
	}

	bool GstPipelineHandler::CreatePipeline()
	{
		if (!m_data->gstPipeline)
			return false;


		//enable logging to stdout
		g_signal_connect(m_data->gstPipeline, "deep-notify", G_CALLBACK(gst_object_default_deep_notify), NULL);


#ifdef USE_GST_CLOCK
		m_data->isMasterClock = (m_data->clockIP == "127.0.0.1" ? false : true);
#endif
		m_data->paused = true;
		m_data->Loaded = false;
		m_data->playing = false;

		GstBus * bus = gst_pipeline_get_bus(GST_PIPELINE(m_data->gstPipeline));

		if (bus){
			m_data->busWatchID = gst_bus_add_watch(bus, (GstBusFunc)busFunction, this);
		}

		gst_object_unref(bus);


		if (gst_element_set_state(GST_ELEMENT(m_data->gstPipeline), GST_STATE_READY) == GST_STATE_CHANGE_FAILURE) {
			(gLogManager.StartLog(ELL_WARNING) << "GStreamerNetworkPlayer::Play(): unable to set pipeline to ready").flush();
			return false;
		}
		if (gst_element_get_state(GST_ELEMENT(m_data->gstPipeline), NULL, NULL, 10 * GST_SECOND) == GST_STATE_CHANGE_FAILURE){
			(gLogManager.StartLog(ELL_WARNING) << "GStreamerNetworkPlayer::Play(): unable to get pipeline ready status").flush();
			return false;
		}

		// pause the pipeline
		if (gst_element_set_state(GST_ELEMENT(m_data->gstPipeline), GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
			(gLogManager.StartLog(ELL_WARNING) << "GStreamerNetworkPlayer::Play(): unable to pause pipeline").flush();
			return false;
		}
#ifdef USE_GST_CLOCK
		if (false)
		{
			if (m_data->isMasterClock)
			{
				ulong basetime;
				m_data->clock = gst_pipeline_get_clock(GST_PIPELINE(m_data->gstPipeline));
				
				basetime = gst_clock_get_time(m_data->clock);
				gst_pipeline_use_clock(GST_PIPELINE(m_data->gstPipeline), m_data->clock);

				m_data->clockProvider = gst_net_time_provider_new(m_data->clock, "127.0.0.1", m_data->clockPort);
				gint port;
				g_object_get(m_data->clockProvider, "port", &port, 0);
				m_data->clockPort = port;

				gst_element_set_start_time(m_data->gstPipeline, GST_CLOCK_TIME_NONE);
				SetClockBaseTime(basetime);
			}
			else
			{
				m_data->clock = gst_net_client_clock_new("", m_data->clockIP.c_str(), m_data->clockPort, m_data->baseTime);
				gst_element_set_start_time(m_data->gstPipeline, GST_CLOCK_TIME_NONE);
				SetClockBaseTime(m_data->baseTime);
				gst_pipeline_use_clock(GST_PIPELINE(m_data->gstPipeline), m_data->clock);
			}
		}
#endif
	//	Stop();
		return true;
	}

	void GstPipelineHandler::SetPaused(bool p)
	{
		m_data->paused = p;
		if (m_data->Loaded)
		{
			if (m_data->playing )
			{
				if (m_data->paused)
				{
					gst_element_set_state(m_data->gstPipeline, GST_STATE_PAUSED);
				}
				else
					gst_element_set_state(m_data->gstPipeline, GST_STATE_PLAYING);
			}
			else
			{
				GstState state = GST_STATE_PAUSED;
				gst_element_set_state(m_data->gstPipeline, state);
				gst_element_get_state(m_data->gstPipeline, &state, NULL, 2 * GST_SECOND);
				if (!m_data->paused)
					gst_element_set_state(m_data->gstPipeline, GST_STATE_PLAYING);
				m_data->playing = true;
			}
		}
	}
	void GstPipelineHandler::Stop()
	{
//   		SetPaused(true);
//   		return;
		if (!m_data->Loaded)return;
		GstState state;
		if (!m_data->paused){
			state = GST_STATE_PAUSED;
			gst_element_set_state(m_data->gstPipeline, state);
			gst_element_get_state(m_data->gstPipeline, &state, NULL, 2 * GST_SECOND);
		}

		state = GST_STATE_READY;
		gst_element_set_state(m_data->gstPipeline, state);
		gst_element_get_state(m_data->gstPipeline, &state, NULL, 2 * GST_SECOND);
		m_data->playing = false;
		m_data->paused = true;
	}
	bool GstPipelineHandler::IsLoaded()
	{
		return m_data->Loaded;
	}
	bool GstPipelineHandler::IsPaused()
	{
		return m_data->paused;
	}
	bool GstPipelineHandler::IsPlaying()
	{
		return m_data->playing;
	}
	bool GstPipelineHandler::QueryLatency(bool &isLive, ulong& minLatency, ulong& maxLatency)
	{
		bool ok = false;
		GstQuery * q = gst_query_new_latency();
		if (gst_element_query(m_data->gstPipeline, q)) {
			GstClockTime minlat = 0, maxlat = 0;
			gboolean live;
			gst_query_parse_latency(q, &live, &minlat, &maxlat);
			isLive = live;
			minLatency = minlat;
			maxLatency = maxlat;
			ok=true;
		}
		gst_query_unref(q);
		return ok;
	}
	void GstPipelineHandler::Close()
	{

		Stop();

		if (m_data->Loaded){
			gst_element_set_state(GST_ELEMENT(m_data->gstPipeline), GST_STATE_NULL);
			gst_element_get_state(m_data->gstPipeline, NULL, NULL, 2 * GST_SECOND);

			if (m_data->busWatchID != 0) g_source_remove(m_data->busWatchID);

			gst_object_unref(m_data->gstPipeline);
			m_data->gstPipeline = NULL;
		}

#ifdef USE_GST_CLOCK

#endif
		m_data->Loaded = false;
	}
	bool GstPipelineHandler::HandleMessage(GstBus * bus, GstMessage * msg)
	{
		switch (GST_MESSAGE_TYPE(msg)) {

		case GST_MESSAGE_BUFFERING:
			gint pctBuffered;
			gst_message_parse_buffering(msg, &pctBuffered);
			break;

#if GST_VERSION_MAJOR==0
		case GST_MESSAGE_DURATION:{
			GstFormat format = GST_FORMAT_TIME;
			gst_element_query_duration(m_data->gstPipeline, &format, &durationNanos);
		}break;
#else
		case GST_MESSAGE_DURATION_CHANGED:
			//gst_element_query_duration(m_data->gstPipeline, GST_FORMAT_TIME, &durationNanos);
			break;

#endif

		case GST_MESSAGE_STATE_CHANGED:{
			GstState oldstate, newstate, pendstate;
			gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
			if (newstate == GST_STATE_PAUSED && !m_data->playing){
				m_data->Loaded = true;
				m_data->playing = true;
				if (!m_data->paused){
					SetPaused(false);
				}
			}
			else if (newstate == GST_STATE_READY)
			{
				FIRE_LISTENR_METHOD(OnPipelineReady, (this));
			}
			else if (newstate == GST_STATE_PLAYING)
			{
				FIRE_LISTENR_METHOD(OnPipelinePlaying, (this));
			}
			else if (newstate == GST_STATE_PAUSED)
			{
				FIRE_LISTENR_METHOD(OnPipelineStopped, (this));
			}

		}break;

		case GST_MESSAGE_ASYNC_DONE:
			break;

		case GST_MESSAGE_WARNING:
		{
			GError *err;
			gchar *debug;
			gst_message_parse_warning(msg, &err, &debug);
			gchar * name = gst_element_get_name(GST_MESSAGE_SRC(msg));

			gLogManager.StartLog(ELL_WARNING) << "GStreamerNetworkPlayer::HandleMessage(): warning in module "
				<< name << "  reported: " << err->message;
			gLogManager.flush();

			g_free(name);
			g_error_free(err);
			g_free(debug);

		//	gst_element_set_state(GST_ELEMENT(m_data->gstPipeline), GST_STATE_NULL);

		}break;
		case GST_MESSAGE_ERROR:
		{
			GError *err;
			gchar *debug;
			gst_message_parse_error(msg, &err, &debug);
			gchar * name = gst_element_get_name(GST_MESSAGE_SRC(msg));

			gLogManager.StartLog(ELL_WARNING) << "GStreamerNetworkPlayer::HandleMessage(): error in module "
				<< name << "  reported: " << err->message<<" - "<<debug;
			gLogManager.flush();

			g_free(name);
			g_error_free(err);
			g_free(debug);

			FIRE_LISTENR_METHOD(OnPipelineError, (this));

			gst_element_set_state(GST_ELEMENT(m_data->gstPipeline), GST_STATE_NULL);

		}break;


		default:
			break;
		}

		return true;
	}

	void GstPipelineHandler::SetClockAddr(const core::string& host, int port)
	{
#ifdef USE_GST_CLOCK
		m_data->clockPort = port;
		m_data->clockIP = host;
#endif

	}
	void GstPipelineHandler::SetClockBaseTime(ulong baseTime)
	{
#ifdef USE_GST_CLOCK
		m_data->baseTime = baseTime;
		if (m_data->gstPipeline)
		{
			GstClock* clock = gst_pipeline_get_clock(GST_PIPELINE(m_data->gstPipeline));

			gst_element_set_base_time(m_data->gstPipeline, baseTime);// -gst_clock_get_time(clock));
		}
#endif
	}

	int GstPipelineHandler::GetClockPort()
	{
#ifdef USE_GST_CLOCK
		return m_data->clockPort;
#else 
		return 0;
#endif
	}
	ulong GstPipelineHandler::GetClockBaseTime()
	{
#ifdef USE_GST_CLOCK
		if (m_data->clock)
			return gst_clock_get_time(m_data->clock);
		return m_data->baseTime;
#else
		return 0;
#endif
	}
	bool GstPipelineHandler::busFunction(GstBus * bus, GstMessage * message, GstPipelineHandler * player)
	{
		return player->HandleMessage(bus, message);

	}
	void GstPipelineHandler::SetPipeline(GstElement* p)
	{
		m_data->gstPipeline = p;
	}
	GstElement* GstPipelineHandler::GetPipeline()
	{
		return m_data->gstPipeline;
	}




}
}

