
#include "stdafx.h"
#include "AppSrcVideoSrc.h"

#include <gst/app/gstappsrc.h>
#include "CMySrc.h"
#include "CMyUDPSink.h"
#include "ILogManager.h"


namespace mray
{
namespace video
{

	class AppSrcVideoSrcImpl
	{
	public:
		struct VideoSrcData
		{
			VideoSrcData()
			{
				videoSrc = 0;
				index = 0;
				sourceID = 0;
				o = 0;
			}
			GstAppSrcCallbacks srcCB;
			GstAppSrc* videoSrc;
			//GstMySrc * videoSrc;
			int index;
			AppSrcVideoSrcImpl* o;
			int sourceID;


		};
		std::vector<VideoSrcData> m_videoSrc;
		std::vector<IVideoGrabber*> m_grabber;

		std::string m_encoder;
		int m_fps;
		int m_bitRate;
		math::vector2di m_frameSize;
		bool m_freeSize;

		AppSrcVideoSrcImpl()
		{
			m_bitRate = 3000;

			m_fps = 30;
			m_frameSize.set(1280, 720);
			m_freeSize = true;
			m_encoder = "H264";
		}

		void  SetResolution(int width, int height, int fps, bool free)
		{
			m_freeSize = free;
			m_frameSize.set(width, height);
			m_fps = fps;
		}

		core::string GetFormatStr(EPixelFormat fmt)
		{

			core::string format = "RGB";
			switch (fmt)
			{
			case EPixel_R8G8B8:
				format = "RGB";
				break;
			case EPixel_R8G8B8A8:
				format = "RGBA";
				break;
			case EPixel_X8R8G8B8:
				format = "xRGB";
				break;

			case EPixel_B8G8R8:
				format = "BGR";
				break;
			case EPixel_B8G8R8A8:
				format = "BGRA";
				break;
			case EPixel_X8B8G8R8:
				format = "xBGR";
				break;
			case EPixel_YUYV:
				format = "Y41B";
				break;

			case EPixel_LUMINANCE8:
			case EPixel_Alpha8:
				format = "GRAY8";
				break;
			}
			return format;
		}

		GstFlowReturn NeedBuffer(GstMySrc * sink, GstBuffer ** buffer, int index)
		{


			if (!m_grabber[index])
			{
				gLogManager.log("No video grabber is assigned to CustomVideoStreamer", ELL_WARNING);
				return GST_FLOW_ERROR;
			}
			if (!m_grabber[index]->GrabFrame())
			{
				return GST_FLOW_ERROR;
			}
			m_grabber[index]->Lock();

			const video::ImageInfo* ifo = m_grabber[index]->GetLastFrame();
			int len = ifo->imageDataSize;
			GstMapInfo map;
			GstBuffer* outbuf = gst_buffer_new_and_alloc(len);
			gst_buffer_map(outbuf, &map, GST_MAP_WRITE);
			memcpy(map.data, ifo->imageData, len);
			gst_buffer_unmap(outbuf, &map);
			m_grabber[index]->Unlock();
			*buffer = outbuf;

			//OS::IThreadManager::getInstance().sleep(5);
			return GST_FLOW_OK;
		}

		static gboolean read_data(VideoSrcData *d)
		{
			GstFlowReturn ret;

			GstBuffer *buffer;
			if (d->o->NeedBuffer(0, &buffer, d->index) == GST_FLOW_OK)
			{
				ret = gst_app_src_push_buffer(d->videoSrc, buffer);
				if (ret != GST_FLOW_OK){
					gLogManager.log("Failed to push data to AppSrc " + core::StringConverter::toString(d->index), ELL_WARNING);
					ret = gst_app_src_end_of_stream(d->videoSrc);
					return FALSE;
				}
			}
			return TRUE;

		}
		static void start_feed(GstAppSrc *source, guint size, gpointer data)
		{
			VideoSrcData* o = static_cast<VideoSrcData*>(data);
			if (o->sourceID != 0)
			{
				g_source_remove(o->sourceID);
				o->sourceID = 0;
			}
			GST_DEBUG("start feeding");
			o->sourceID = g_idle_add((GSourceFunc)read_data, o);
		}


		static void stop_feed(GstAppSrc *source, gpointer data)
		{
			VideoSrcData* o = static_cast<VideoSrcData*>(data);
			if (o->sourceID != 0) {
				GST_DEBUG("stop feeding");
				g_source_remove(o->sourceID);
				o->sourceID = 0;
			}
		}
		static gboolean seek_data(GstAppSrc *src, guint64 offset, gpointer user_data)
		{
			return TRUE;
		}


		void LinkWithPipeline(void* pipeline)
		{


			for (int i = 0; i < m_grabber.size(); ++i)
			{
				core::string name = "src" + core::StringConverter::toString(i);
				m_videoSrc[i].videoSrc = GST_APP_SRC(gst_bin_get_by_name(GST_BIN((GstElement*)pipeline), name.c_str()));
				m_videoSrc[i].o = this;
				m_videoSrc[i].index = i;
				if (m_videoSrc[i].videoSrc){
					printf("Linking Video Src:%d\n", i);
					gst_base_src_set_blocksize(GST_BASE_SRC(m_videoSrc[i].videoSrc), 640 * 480 * 3);
					gst_base_src_set_live(GST_BASE_SRC(m_videoSrc[i].videoSrc), true);
					gst_base_src_set_async(GST_BASE_SRC(m_videoSrc[i].videoSrc), false);
					gst_base_src_set_do_timestamp(GST_BASE_SRC(m_videoSrc[i].videoSrc), true);

					g_object_set(G_OBJECT(m_videoSrc[i].videoSrc),
						"stream-type", GST_APP_STREAM_TYPE_STREAM, // GST_APP_STREAM_TYPE_STREAM
						"format", GST_FORMAT_TIME,
						"is-live", TRUE,
						NULL);

					gst_app_src_set_max_bytes(m_videoSrc[i].videoSrc, 640 * 480 * 3);
					gst_app_src_set_emit_signals(m_videoSrc[i].videoSrc, false);

					m_videoSrc[i].srcCB.need_data = &start_feed;
					m_videoSrc[i].srcCB.enough_data = &stop_feed;
					m_videoSrc[i].srcCB.seek_data = &seek_data;
					gst_app_src_set_callbacks(m_videoSrc[i].videoSrc, &m_videoSrc[i].srcCB, &m_videoSrc[i], NULL);

				}
			}
		}

		core::string BuildBaseGStr(int i)
		{

			core::string videoStr;
			if (m_grabber[i])
			{
				core::string format = GetFormatStr(m_grabber[i]->GetImageFormat());
				//ksvideosrc

				videoStr = "appsrc";
				videoStr += " name=src" + core::StringConverter::toString(i) + 
					" do-timestamp=true is-live=true "//"block=true"
					" ! video/x-raw,format=" + format + ",width=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().x) +
					",height=" + core::StringConverter::toString(m_grabber[i]->GetFrameSize().y) + ",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

				videoStr += "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
				videoStr += "! videoconvert  ! video/x-raw,format=I420 ";//",framerate=1/" + core::StringConverter::toString(m_fps);// !videoflip method = 1  ";
				videoStr += "! queue ";
				//	if (m_grabber[i]->GetImageFormat()!=video::EPixel_YUYV)

				if (!m_freeSize)
				{
					videoStr += "! videoscale ! video/x-raw,width=" + core::StringConverter::toString(m_frameSize.x) +
						",height=" + core::StringConverter::toString(m_frameSize.y) + ",framerate=" + core::StringConverter::toString(m_fps) + "/1";

				}
			}
			else{
				videoStr = "mysrc name=src" + core::StringConverter::toString(i) +
					" ! video/x-raw,format=RGB  ";// !videoflip method = 1  ";
			}
			//add time stamp

			return videoStr;
		}

		std::string BuildStringH264(int i)
		{
			std::string videoStr;

			videoStr += BuildBaseGStr(i);


			videoStr += "! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / m_grabber.size()) +
				" speed-preset=superfast pass=cbr tune=zerolatency sync-lookahead=0 rc-lookahead=0 sliced-threads=true key-int-max=5"
				" psy-tune=1 "//interlaced=true sliced-threads=false  "// 
				" ! rtph264pay ";

			return videoStr;
		}

		std::string BuildStringVP8(int i)
		{
			std::string videoStr;

			videoStr += BuildBaseGStr(i);


			videoStr += "! vp8enc end-usage=cbr target-bitrate=" + core::StringConverter::toString((1000 * m_bitRate) / m_grabber.size()) +
				" keyframe-mode=1 keyframe-max-dist=1 threads=4 "// ip-factor=1.8 interlaced=true sliced-threads=false  "// 
				" ! rtpvp8pay  ";
			/*
			//videoStr += " ! vp8enc ! rtpvp8pay ";
			videoStr += " ! theoraenc ! rtptheorapay ";*/

			//videoStr += " ! autovideosink sync=false ";

			return videoStr;

		}
		core::string BuildGStr(int i)
		{
			std::string videoStr ;
			if (m_encoder == "H264")
				videoStr = BuildStringH264(i);
			else if (m_encoder == "VP8")
				videoStr = BuildStringVP8(i);

			return videoStr;
		}

	};

AppSrcVideoSrc::AppSrcVideoSrc()
{
	m_impl = new AppSrcVideoSrcImpl();
}
AppSrcVideoSrc::~AppSrcVideoSrc()
{
	delete m_impl;
}

void  AppSrcVideoSrc::SetResolution(int width, int height, int fps, bool free)
{
	m_impl->SetResolution(width, height, fps, free);
}

std::string AppSrcVideoSrc::GetPipelineStr(int i)
{
	return m_impl->BuildGStr(i);
}

void AppSrcVideoSrc::LinkWithPipeline(void* pipeline)
{
	m_impl->LinkWithPipeline(pipeline);
}

void AppSrcVideoSrc::SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers)
{
	m_impl->m_grabber = grabbers;
	m_impl->m_videoSrc.resize(grabbers.size());
}

int AppSrcVideoSrc::GetVideoSrcCount(){ return m_impl->m_grabber.size(); }

void AppSrcVideoSrc::SetEncoderType(const std::string &type)
{
	m_impl->m_encoder = type;
}

std::string AppSrcVideoSrc::GetDataType()
{
	return m_impl->m_encoder;
}
void AppSrcVideoSrc::SetBitRate(int bitRate)
{
	m_impl->m_bitRate = bitRate;
}
void AppSrcVideoSrc::Start()
{
}
void AppSrcVideoSrc::Pause()
{

}
void AppSrcVideoSrc::Close()
{

}

}
}
