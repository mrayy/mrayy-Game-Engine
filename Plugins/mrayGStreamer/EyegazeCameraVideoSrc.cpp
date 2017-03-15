
#include "stdafx.h"
#include "EyegazeCameraVideoSrc.h"
#include "StringConverter.h"
#include "CameraVideoSrcImpl.h"
#include "CMyListener.h"
#include "rtp.h"
#include "IThreadManager.h"
#include "IMutex.h"

#include <gst/gst.h>

namespace mray
{
namespace video
{

class EyegazeCameraVideoSrcImpl:public IMyListenerCallback
{
public:
	CameraVideoSrcImpl *m_impl;
	bool m_inited;

	bool m_eyePosDirty;
	std::vector<math::vector2df> m_eyepos;
	std::vector<GstElement*> m_videoRects;
	math::vector2di m_cropsize;

	std::list<math::vector4di> m_gazeCashe;


	GstMyListener *m_precodecListener;
	GstMyListener *m_rtpListener;

	OS::IMutex* m_gazeMutex;

	uint32_t m_lastRtpTS;

	bool m_sent;
	math::vector4di m_sendRect;

public:
	EyegazeCameraVideoSrcImpl()
	{
		m_inited = false;
		m_rtpListener = 0;
		m_precodecListener = 0;
		m_eyePosDirty = false;
		m_lastRtpTS = -1;
		m_gazeMutex = OS::IThreadManager::getInstance().createMutex();
		m_sent = true;

		//factorials of 640,480: (2)240, (4)120 , (5)96
		//m_cropsize.set(240, 240);
		m_cropsize.set(120, 120);

	}
	virtual ~EyegazeCameraVideoSrcImpl()
	{
		if (m_rtpListener)
			m_rtpListener->listeners->RemoveListener(this);
		if (m_precodecListener)
			m_precodecListener->listeners->RemoveListener(this);

		delete m_gazeMutex;
	}

	void SetEyegazePos(const std::vector<math::vector2d>& poses)
	{
		bool okay = false;
		for (int i = 0; i < m_eyepos.size() && !okay;++i)
		{
			if (m_eyepos[i] != poses[i])
				okay = true;
		}
		if (!okay)
			return;
		m_eyePosDirty = true;
		m_eyepos= poses;
		_UpdateEyegazePos();
	}

	math::vector4di GetCropRect(int i)
	{
		if (i >= m_eyepos.size())
			return math::vector4di(0, 0, m_cropsize.x, m_cropsize.y);
		math::vector2di gazePos(m_impl->m_frameSize.x*m_eyepos[i].x, m_impl->m_frameSize.y*m_eyepos[i].y);
		gazePos.x = math::Max(math::Min(gazePos.x, m_impl->m_frameSize.x - m_cropsize.x / 2), m_cropsize.x / 2);
		gazePos.y = math::Max(math::Min(gazePos.y, m_impl->m_frameSize.y - m_cropsize.y / 2), m_cropsize.y / 2);

		math::vector4di cropRect(gazePos.x - m_cropsize.x / 2, m_impl->m_frameSize.x - (gazePos.x + m_cropsize.x / 2),
			gazePos.y - m_cropsize.y / 2, m_impl->m_frameSize.y - (gazePos.y + m_cropsize.y / 2));//left,right,top,bottom

		return cropRect;

	}
	void _UpdateEyegazePos()
	{
		if (!m_eyePosDirty || !m_sent)
			return;
		m_eyePosDirty = false;
		m_sent = false;
		m_sendRect = GetCropRect(0);
		//update video boxes
		for (int i = 0; i < m_videoRects.size(); ++i)
		{
			if (!m_videoRects[i])
				continue;

			math::vector4di cropRect = m_sendRect;// GetCropRect(i);
			g_object_set(m_videoRects[i],"left",cropRect.x,"right",cropRect.y,
				"top", cropRect.z, "bottom", cropRect.w ,0);
		}
	}
	void SetEyegazeCrop(int w, int h)
	{
		m_cropsize.set(w, h);
	}
	void LinkWithPipeline(void* p)
	{
		GstElement* pipeline = (GstElement*)p;
		m_inited = true;
		for (int i = 0; i < m_impl->m_cams.size(); ++i)
		{
			core::string name = "box_" + core::StringConverter::toString(i);
			GstElement* e = gst_bin_get_by_name(GST_BIN(pipeline), name.c_str());
			m_videoRects.push_back(e);
		}
		m_rtpListener = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline), "rtplistener"));
		if (m_rtpListener)
			m_rtpListener->listeners->AddListener(this);
		m_precodecListener = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline), "precodec"));
		if (m_precodecListener)
			m_precodecListener->listeners->AddListener(this);
	}

	struct RTPPacketData
	{
		uint32_t timestamp;
		uint64_t presentationTime;
		unsigned short length;
		unsigned char data[255];
	};
	virtual void ListenerOnDataChained(_GstMyListener* src, GstBuffer * buffer)
	{
		if (src == m_precodecListener)
		{
			//_UpdateEyegazePos();
			m_gazeMutex->lock();
			m_gazeCashe.push_back(m_sendRect);
			m_gazeMutex->unlock();
		}
		else if (src == m_rtpListener)
		{

			GstMapInfo map;
			gst_buffer_map(buffer, &map, GST_MAP_READ);

			if (true)
			{
				RTPPacketData packet;
				packet.timestamp = rtp_timestamp(map.data);
				//packet.length = rtp_padding_payload((unsigned char*)map.data, map.size, packet.data);
				if (packet.timestamp != m_lastRtpTS)
				{
					m_lastRtpTS = packet.timestamp;
					m_gazeMutex->lock();
					math::vector4di gaze = m_gazeCashe.front();
					m_gazeCashe.pop_front();
					m_sent = true;
					m_gazeMutex->unlock();

					{
						//static unsigned char buffer[2000];
						static unsigned char data[20];

						math::Swap(gaze.y, gaze.z);

						gaze.z = m_impl->m_frameSize.x - gaze.x - gaze.z;
						gaze.w = m_impl->m_frameSize.y - gaze.y - gaze.w;

						memcpy(data, &gaze, sizeof(math::vector4di));

						int len = sizeof(math::vector4di) + map.size;


						//add rtp padding
						//unsigned short len = rtp_add_padding(map.data, map.size, data, 2 * sizeof(float), buffer);

						GstMemory *mem;
						mem = gst_allocator_alloc(NULL, len, NULL);

						GstMapInfo info_out;
						gst_memory_map(mem, &info_out, GST_MAP_WRITE);
						guint8 *out = info_out.data;

						rtp_add_padding(map.data, map.size, data, sizeof(math::vector4di), out);

						gst_memory_unmap(mem, &info_out);

						gst_buffer_replace_all_memory(buffer, mem);
					}
				}
			}
			else{

				m_gazeMutex->lock();
				math::vector4di gaze = m_gazeCashe.front();
				m_gazeCashe.pop_front();
				m_gazeMutex->unlock();

				memcpy(map.data + map.size - sizeof(gaze), &gaze, sizeof(gaze));
			}

			gst_buffer_unmap(buffer, &map);
		}
	}

};

EyegazeCameraVideoSrc::EyegazeCameraVideoSrc()
{
	m_data = new EyegazeCameraVideoSrcImpl();
	m_data->m_impl = m_impl;
}

EyegazeCameraVideoSrc::~EyegazeCameraVideoSrc()
{
	delete m_data;
}
std::string EyegazeCameraVideoSrc::BuildStringH264()
{
	if (false)
	{
		std::string videoStr = "";


		// 			videoStr += " ! videoconvert ! autovideosink sync=false ";
		// 			return videoStr;

		videoStr += "! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / GetStreamsCount()) + " ";

		/*
		" speed-preset=superfast  tune=zerolatency pass=cbr sliced-threads=true"//" key-int-max=5"
		" sync-lookahead=0 rc-lookahead=0"
		" psy-tune=none "//interlaced=true sliced-threads=false  "//
		" quantizer=15 ";*/

		//fill in parameters
		std::map<std::string, std::string>::iterator it = m_encoderParams.begin();
		for (; it != m_encoderParams.end(); ++it)
			videoStr += it->first + "=" + it->second + " ";

		videoStr += "! mylistener name=rtplistener  ! rtph264pay mtu=" + core::StringConverter::toString(m_mtuSize) + " ";

		//videoStr += "! autovideosink";

		return videoStr;
	}else
		return CameraVideoSrc::BuildStringH264() + " ! mylistener name=rtplistener ";
}

std::string EyegazeCameraVideoSrc::_generateString(int i)
{
	std::string videoStr;
	if (m_impl->m_cams[i] != -1)
	{
		videoStr = "ksvideosrc";
		videoStr += " name=src" + core::StringConverter::toString(i);
		videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

		//videoStr += "videotestsrc ";
		//" do-timestamp=true is-live=true "//"block=true"
		videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
			",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
		
	}
	else
		videoStr="videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
		",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);

	return videoStr;

}
std::string EyegazeCameraVideoSrc::_generateFullString()
{

	std::string videoStr;
	bool mixer = false;

	int camsCount = 0;
	float totalWidth = 0;
	float totalHeight = 0;

	if (m_data->m_cropsize.x % 2 == 1)
		m_data->m_cropsize.x += 1;
	if (m_data->m_cropsize.y % 2 == 1)
		m_data->m_cropsize.y += 1;

	int sceneWidth = (m_data->m_impl->m_frameSize.x* m_data->m_cropsize.y) / m_impl->m_frameSize.y;
	int sceneHeight = (m_data->m_impl->m_frameSize.y* m_data->m_cropsize.x) / m_impl->m_frameSize.x;

	if (sceneWidth % 2 == 1)
		sceneWidth += 1;

	if (sceneHeight % 2 == 1)
		sceneHeight += 1;

	int lostWidth = m_data->m_impl->m_frameSize.x-sceneWidth*m_impl->m_frameSize.y / m_data->m_cropsize.y;
	int lostHeight = m_data->m_impl->m_frameSize.y- sceneHeight*m_impl->m_frameSize.x / m_data->m_cropsize.x;

	for (int i = 0; i < m_impl->m_cams.size(); ++i)
	{
		if (m_impl->m_cams[i] != -1)
		{
			camsCount++;
			totalWidth += m_impl->m_frameSize.x;
			totalHeight += m_impl->m_frameSize.y;
		}
	}
	if (camsCount == 0)
	{
		videoStr = "videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
			",height=" + core::StringConverter::toString(m_impl->m_frameSize.y);
	}
	else
	{
		if (!m_impl->m_separateStreams  && camsCount > 1)
		{
			mixer = true;
			videoStr = " videomixer name=mix "
				"  sink_0::xpos=0 sink_0::ypos=0  sink_0::zorder=0 sink_0::alpha=1  ";
			int xpos = 0;
			int ypos = 0;
			for (int i = 0; i < camsCount; ++i)
			{
				std::string name = "sink_" + core::StringConverter::toString(i + 1);
				//videoStr += "  " + name + "::xpos=" + core::StringConverter::toString(xpos) + " " + name + "::ypos=0  " + name + "::zorder=0 " + name + "::zorder=1  ";
				videoStr += "  " + name + "::xpos=0 " + name + "::ypos=" + core::StringConverter::toString(ypos) + " " + name + "::zorder=0 " + name + "::zorder=1  ";
				xpos += m_data->m_cropsize.x;
				ypos += m_data->m_cropsize.y;
			}
		}

		int counter = 1;
		core::string mixerStr;
		core::string mName;
		core::string tName;

		bool precodecSet = false;
		for (int i = 0; i < m_impl->m_cams.size(); ++i)
		{
			if (m_impl->m_cams[i] != -1)
			{
				if (m_data->m_eyepos.size() <= i)
					m_data->m_eyepos.push_back(math::vector2df(0.5f, 0.5f));
				math::vector2di gazePos(m_impl->m_frameSize.x*m_data->m_eyepos[i].x, m_impl->m_frameSize.y*m_data->m_eyepos[i].y);
				math::vector4di cropRect(gazePos.x - m_data->m_cropsize.x / 2, m_impl->m_frameSize.x - (gazePos.x + m_data->m_cropsize.x / 2),
					gazePos.y - m_data->m_cropsize.y / 2, m_impl->m_frameSize.y - (gazePos.y + m_data->m_cropsize.y / 2));//left,right,top,bottom

				mName = "mix_" + core::StringConverter::toString(i);
				tName = "t_" + core::StringConverter::toString(i);
				mixerStr = "videomixer name="+mName;
				//mixerStr += "  sink_0::xpos=0 sink_0::ypos=0  sink_0::zorder=0 sink_1::alpha=1  ";
				mixerStr += "  sink_1::xpos=0 sink_1::ypos=0  sink_1::zorder=0 sink_1::alpha=1  ";
				//mixerStr += "  sink_2::xpos=0 sink_2::ypos=" + core::StringConverter::toString(m_data->m_cropsize.y) + "  sink_2::zorder=0 sink_2::alpha=1  ";
				mixerStr += "  sink_2::xpos=" + core::StringConverter::toString(m_data->m_cropsize.x) + " sink_2::ypos=0 sink_2::zorder=0 sink_2::alpha=1  ";

				videoStr += mixerStr;

				//videoStr += "videotestsrc pattern=1 ! video/x-raw,width=" + core::StringConverter::toString(sceneWidth) +
				//	",height=" + core::StringConverter::toString(m_data->m_cropsize.y) + ",framerate=" + core::StringConverter::toString(m_fps)+"/1 ! "+mName+".sink_0 ";

				videoStr += "ksvideosrc";
				videoStr += " name=src" + core::StringConverter::toString(i);
				videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

				//videoStr += "videotestsrc ";
				//" do-timestamp=true is-live=true "//"block=true"
				videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(m_impl->m_frameSize.x) +
					",height=" + core::StringConverter::toString(m_impl->m_frameSize.y) + " ! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ! videoconvert ! tee name=" + tName + " ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";

				if (!precodecSet)
				{
					videoStr += tName + ". ! mylistener name=precodec ! queue ! ";
					precodecSet = true;
				}
				else
					videoStr += tName + ". ! queue ! ";

				//eyegaze string
				videoStr +=  "  videobox name=box_" + core::StringConverter::toString(i) + 
					" left="   + core::StringConverter::toString(cropRect.x)+
					" right="  + core::StringConverter::toString(cropRect.y)+
					" top="    + core::StringConverter::toString(cropRect.z)+
					" bottom=" + core::StringConverter::toString(cropRect.w)+
					" ! videoconvert !" + mName + ".sink_1 ";


				//scene string
				//videoStr += tName + ". ! queue ! videoscale ! video/x-raw,width=" + core::StringConverter::toString(m_data->m_cropsize.x) + ",height=" + core::StringConverter::toString(sceneHeight) + " ! videoconvert ! " + mName + ".sink_2 ";
				videoStr += tName + ". ! queue ";
				if (lostWidth > 0)
				{
					//videoStr += " ! videobox left=0 top=0 bottom=0 right=" + core::StringConverter::toString(lostWidth) ;
				}
				videoStr+=" !  videoscale add-borders=false method=6 sharpen=1 envelope=4 ! video/x-raw,width=" + core::StringConverter::toString(sceneWidth) + ",height=" + core::StringConverter::toString(m_data->m_cropsize.y) + " ! videoconvert ! " + mName + ".sink_2 ";

				videoStr += mName+". ";

				if (mixer)
				{
					videoStr +=" ! mix.sink_" + core::StringConverter::toString(counter) + " ";
				}
				++counter;
			}

		}

		if (mixer)
		{
		//	videoStr += " mix. ! videoflip method=5 ";// "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
			videoStr += " mix.  ";// "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
		}
		else 
		{
			//videoStr += " ! mylistener name=precodec ";
		}
	}
	return videoStr;
}
void EyegazeCameraVideoSrc::LinkWithPipeline(void* pipeline)
{
	CameraVideoSrc::LinkWithPipeline(pipeline);
	m_data->LinkWithPipeline(pipeline);
}
void EyegazeCameraVideoSrc::SetEyegazePos(const std::vector<math::vector2df>& poses)
{
	m_data->SetEyegazePos(poses);
}
void EyegazeCameraVideoSrc::SetEyegazeCrop(int w, int h)
{
	m_data->SetEyegazeCrop(w, h);
}

}
}

