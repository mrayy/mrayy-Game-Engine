
#include "stdafx.h"
#include "EyegazeCameraVideoSrc.h"
#include "StringConverter.h"
#include "CameraVideoSrcImpl.h"
#include "CMyListener.h"
#include "rtp.h"
#include "IThreadManager.h"
#include "ILogManager.h"
#include "IMutex.h"

//#include "AveragePer.h"
#include "FPSCalc.h"
#include "ITimer.h"
#include "GStreamerCore.h"

#include <gst/gst.h>

namespace mray
{
namespace video
{

	class EyegazeCameraVideoSrcImpl :public IMyListenerCallback
	{
	public:


		bool m_inited;

		bool m_eyePosDirty;
	//	AveragePer m_sentBytes;

		ICustomVideoSrc* source;

		bool m_separateStreams;

		std::vector<math::vector2df> m_eyepos;
		std::vector<std::vector<GstElement*>> m_videoRects; //each element contains video rects at different levels for each eye
		math::vector2di m_cropsize;

		math::vector2di streamSize;

		struct GazeData
		{
			GstClockTime pts;
			std::vector<math::vector4di> gaze;
		};

		std::list<GazeData> m_gazeCasheTmp;
		std::list<GazeData> m_gazeCashe;

		int m_levels; //number of levels to sample the image at a certain position (eye gaze)


		GstMyListener *m_precodecListener;
		GstMyListener *m_prertpListener;
		GstMyListener *m_rtpListener;

		OS::IMutex* m_gazeMutex;

		core::FPSCalc updateFPS;

		uint32_t m_lastRtpTS;

		bool m_sent;
		std::vector<math::vector4di> m_sendRect;
		int counter;

		FILE* averageBytesFile;

		int foveatedRectsCount;

		math::vector2di _divScaler;

	public:
		EyegazeCameraVideoSrcImpl()
		{
			counter = 0;
			m_inited = false;
			m_rtpListener = 0;
			m_prertpListener = 0;
			m_precodecListener = 0;
			m_eyePosDirty = false;
			m_lastRtpTS = -1;
			m_gazeMutex = OS::IThreadManager::getInstance().createMutex();
			m_sent = true;
			m_levels = 3;
			m_separateStreams = false;
			source = 0;
			foveatedRectsCount = 1;
			_divScaler = 1;
			//m_sentBytes.OnSample += newClassDelegate1("", this, &EyegazeCameraVideoSrcImpl::OnBytesAverage);
			//averageBytesFile = fopen("BytesSent.txt", "w");

			//factorials of 640,480: (2)240, (4)120 , (5)96
			//m_cropsize.set(240, 240);
			m_cropsize.set(120, 120);

			dir = 1;
		}
		virtual ~EyegazeCameraVideoSrcImpl()
		{
			if (m_rtpListener)
				m_rtpListener->listeners->RemoveListener(this);
			if (m_prertpListener)
				m_prertpListener->listeners->RemoveListener(this);
			if (m_precodecListener)
				m_precodecListener->listeners->RemoveListener(this);

			delete m_gazeMutex;

			fclose(averageBytesFile);
		}

		void OnBytesAverage(int bytes){
	//		fprintf(averageBytesFile,"%d\t", bytes);
		}

		void SetEyegazePos(const std::vector<math::vector2d>& poses)
		{
			if (!m_inited)
				return;
			bool okay = false;
			if (m_eyepos.size() != poses.size()){
				m_eyepos.resize(poses.size());
				okay = true;
			}

			math::vector2di framesize = source->GetFrameSize(0)/_divScaler;
			for (int i = 0; i < m_sendRect.size() && !okay; ++i)
			{
				m_eyepos[i] = poses[i];
				math::vector4di r = GetCropRect(0, i, framesize, _divScaler);
				if (r != m_sendRect[i])
					okay = true;
			}
			if (!okay)
				return;
			m_eyePosDirty = true;
			m_eyepos = poses;
			//_UpdateEyegazePos();
		}

		math::vector2d GetRectSize(int level)
		{

			//use formula:  Size=Level*(ImageSize - EyegazeCropSize)/LevelsCount +EyegazeCropSize 
			//return level*(framesize - m_cropsize) / m_levels + m_cropsize; // uniform size selection between the foveation and full scene fov
			return (level + 1)* m_cropsize; //linear increment 
		}

		//level==0 --> gaze rect
		//level==1 --> sample rect at level 1
		//level==m_levels-1 --> Entire image rect

		math::vector2di gazePos;
		int dir;
		math::vector4di GetCropRect(int i, int level,const math::vector2di& framesize, math::vector2di divScaler)
		{
			if (i >= m_eyepos.size())
				return math::vector4di(0, 0, m_cropsize.x, m_cropsize.y);

			math::vector2d targetSize = GetRectSize(level)/2;
			//targetSize /= divScaler;

			//math::vector2di framesize = source->GetFrameSize(i);

			math::vector2di gazePos(framesize.x*m_eyepos[i].x, framesize.y*m_eyepos[i].y);

			/*gazePos.x += dir*10;
			gazePos.y = framesize.y / 2;
			if (gazePos.x > 800)
			{
				dir = -dir;
				gazePos.x = 800;
			}if (gazePos.x <300)
			{
				dir = -dir;
				gazePos.x = 300;
			}*/

			/*
			gazePos.x = math::Max(math::Min(gazePos.x, framesize.x - m_cropsize.x / 2), m_cropsize.x / 2);
			gazePos.y = math::Max(math::Min(gazePos.y, framesize.y - m_cropsize.y / 2), m_cropsize.y / 2);

			math::vector4di cropRect(gazePos.x - m_cropsize.x / 2, framesize.x - (gazePos.x + m_cropsize.x / 2),
			gazePos.y - m_cropsize.y / 2, framesize.y - (gazePos.y + m_cropsize.y / 2));//left,right,top,bottom
			*/
			gazePos.x = math::Max<int>(math::Min<int>(gazePos.x, framesize.x - targetSize.x ), targetSize.x );
			gazePos.y = math::Max<int>(math::Min<int>(gazePos.y, framesize.y - targetSize.y ), targetSize.y );

			math::vector4di cropRect(gazePos.x - targetSize.x , framesize.x - (gazePos.x + targetSize.x ),
				gazePos.y - targetSize.y , framesize.y - (gazePos.y + targetSize.y ));//left,right,top,bottom
			return cropRect;

		}
		void _UpdateEyegazePos()
		{
			if (!m_inited || !m_eyePosDirty /*|| !m_sent*/)
				return;
			m_eyePosDirty = false;
			//m_sent = false;

			math::vector2di framesize = source->GetFrameSize(0)/_divScaler;
			updateFPS.regFrame(gGStreamerCore->GetTimer()->getSeconds());

			float fps = updateFPS.getFPS();
			printf("Eyegaze Update Rate: %f\n", fps);
			//update video boxes
			for (int level = 0; level < m_levels; ++level)
			{
				m_sendRect[level] = GetCropRect(0, level, framesize, _divScaler);
				math::vector4di &cropRect = m_sendRect[level];
				for (int i = 0; i < m_videoRects.size(); ++i)
				{
					if (!m_videoRects[i][level])
						continue;

					g_object_set(m_videoRects[i][level], "left", cropRect.x, "right", cropRect.y,
						"top", cropRect.z, "bottom", cropRect.w, 0);
				}
			}
		}
		void SetEyegazeCrop(int w, int h)
		{
			if (h % 2 == 1)
				h++;
			m_cropsize.set(w, h);
		}
		void LinkWithPipeline(void* p)
		{
			GstElement* pipeline = (GstElement*)p;
			m_inited = true;
			source->LinkWithPipeline(p);
			m_videoRects.clear();
			m_sendRect.resize(m_levels);
			for (int i = 0; i < foveatedRectsCount; ++i)
			{
				m_videoRects.push_back(std::vector<GstElement*>());
				for (int level = 0; level < m_levels; ++level)
				{
					core::string name = "box_" + core::StringConverter::toString(i) + "_" + core::StringConverter::toString(level);
					GstElement* e = gst_bin_get_by_name(GST_BIN(pipeline), name.c_str());
					m_videoRects[i].push_back(e);
				}
			}
			m_rtpListener = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline), "rtplistener"));
			if (m_rtpListener)
				m_rtpListener->listeners->AddListener(this);
			m_prertpListener = GST_MyListener(gst_bin_get_by_name(GST_BIN(pipeline), "preRtplistener"));
			if (m_prertpListener)
				m_prertpListener->listeners->AddListener(this);
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
		GazeData _ds;
		virtual void ListenerOnDataChained(_GstMyListener* src, GstBuffer * buffer)
		{

			if (src == m_precodecListener) //before encoder is applied, save the rect
			{
				if (m_sendRect.size() > 0)
				{

					if (m_sent)
						_UpdateEyegazePos();
					m_sent = false; 
					GazeData ds;
					ds.pts = buffer->pts;
					ds.gaze = m_sendRect;
				//	m_gazeMutex->lock();
					/*
					for (int i = 0; i < m_levels; ++i)
					{
					g_object_get(m_videoRects[0][i], "left", &m_sendRect[i].x, "right", &m_sendRect[i].y,
					"top", &m_sendRect[i].z, "bottom", &m_sendRect[i].w, 0);
					}
					m_gazeCashe.push_back(ds);*/
					_ds = ds;
					m_gazeMutex->lock();
					m_gazeCasheTmp.push_back(_ds);
					m_gazeMutex->unlock();
				//	m_gazeMutex->unlock();


					// 			GstMapInfo map;
					// 			gst_buffer_map(buffer, &map, GST_MAP_READ);
					// 
					// 			gst_buffer_unmap(buffer, &map);
				}
			}
			else if (src == m_prertpListener)//before rtp
			{
				m_gazeMutex->lock();
				if (m_gazeCasheTmp.size() > 0)
				{
					m_gazeCashe.push_back(m_gazeCasheTmp.front());
					m_gazeCasheTmp.pop_front();
				}
				else m_gazeCashe.push_back(_ds);
				m_gazeMutex->unlock();
				m_sent = true;
			}
			else if (src == m_rtpListener)//after rtp payloader
			{

				GstMapInfo map;
				gst_buffer_map(buffer, &map, GST_MAP_READ);

				{
					RTPPacketData packet;
					packet.timestamp = rtp_timestamp(map.data);
					//packet.length = rtp_padding_payload((unsigned char*)map.data, map.size, packet.data);
					if (packet.timestamp != m_lastRtpTS)//inject eyegaze data to the first frame of the rtp stream
					{
						m_lastRtpTS = packet.timestamp;
						GazeData ds;
						m_gazeMutex->lock();
						/*while (m_gazeCashe.size() > 0)
						{
							// 					int count = 0;
							// 					do {
							//						count++;
							ds = m_gazeCashe.front();
							m_gazeCashe.pop_front();
						}*/
					//	ds = _ds;
						ds = m_gazeCashe.front();
						m_gazeCashe.pop_front();

						m_gazeMutex->unlock();
						// 					} while (ds.pts != buffer->pts);
						// 					if (count > 1)
						// 						printf("exceeding!\n");
						std::vector<math::vector4di> &gaze = ds.gaze;
						math::vector2di framesize = source->GetFrameSize(0) / _divScaler;

						{
							//static unsigned char buffer[2000];
							static unsigned char data[128];
							unsigned char* ptr = data;

							const int header = 0x1010;
							int levels = gaze.size();
							memcpy(ptr, &header, sizeof(header)); ptr += sizeof(header);
							memcpy(ptr, &levels, sizeof(levels)); ptr += sizeof(levels);
							for (int i = 0; i < gaze.size(); ++i)
							{
								math::Swap(gaze[i].y, gaze[i].z);

								gaze[i].z = framesize.x - gaze[i].x - gaze[i].z;
								gaze[i].w = framesize.y - gaze[i].y - gaze[i].w;
								memcpy(ptr, &gaze[i], sizeof(math::vector4di));
								ptr += sizeof(math::vector4di);
							}
							int dataLen = sizeof(math::vector4di)*gaze.size() + sizeof(int)*2;

// 							memcpy(ptr, &counter, sizeof(counter));
// 							counter++;

							int len = dataLen + map.size;


							//add rtp padding
							//unsigned short len = rtp_add_padding(map.data, map.size, data, 2 * sizeof(float), buffer);

							GstMemory *mem;
							mem = gst_allocator_alloc(NULL, len, NULL);

							GstMapInfo info_out;
							gst_memory_map(mem, &info_out, GST_MAP_WRITE);
							guint8 *out = info_out.data;

							rtp_add_padding(map.data, map.size, data, dataLen, out);

							gst_memory_unmap(mem, &info_out);

							gst_buffer_replace_all_memory(buffer, mem);
						}
					}
				}
				/*
				else{

					m_gazeMutex->lock();
					GazeData ds = m_gazeCashe.front();
					m_gazeCashe.pop_front();
					m_gazeMutex->unlock();

					memcpy(map.data + map.size - sizeof(ds.gaze), &ds.gaze, sizeof(ds.gaze));
					}*/
//				m_sentBytes.Add(map.size);

				gst_buffer_unmap(buffer, &map);


			}
		}

	};

	EyegazeCameraVideoSrc::EyegazeCameraVideoSrc()
	{
		m_data = new EyegazeCameraVideoSrcImpl();
	}

	EyegazeCameraVideoSrc::~EyegazeCameraVideoSrc()
	{
		delete m_data;
	}
	void EyegazeCameraVideoSrc::Start()
	{
		m_data->source->Start();
	}
	void EyegazeCameraVideoSrc::Pause()
	{
		m_data->source->Pause();
	}
	void EyegazeCameraVideoSrc::Close()
	{
		m_data->source->Close();
	}
	void EyegazeCameraVideoSrc::LoadParameters(xml::XMLElement* e)
	{
		ICustomVideoSrc::LoadParameters(e);
		m_data->source->LoadParameters(e);

	}

	void EyegazeCameraVideoSrc::SetBitRate(int bitrate){
		m_data->source->SetBitRate(bitrate);
		ICustomVideoSrc::SetBitRate(bitrate);
	}
	std::string EyegazeCameraVideoSrc::BuildStringH264()
	{
		if (false)
		{
			std::string videoStr = "";


			// 			videoStr += " ! videoconvert ! autovideosink sync=false ";
			// 			return videoStr;

			videoStr += /*"! mylistener name=precodec" */"  ! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / GetStreamsCount()) + " ";

			/*
			" speed-preset=superfast  tune=zerolatency pass=cbr sliced-threads=true"//" key-int-max=5"
			" sync-lookahead=0 rc-lookahead=0"
			" psy-tune=none "//interlaced=true sliced-threads=false  "//
			" quantizer=15 ";*/


			//fill in parameters
			std::map<std::string, std::string>::iterator it = m_encoderParams.begin();
			for (; it != m_encoderParams.end(); ++it)
				videoStr += it->first + "=" + it->second + " ";

			videoStr += "  ! rtph264pay mtu=" + core::StringConverter::toString(m_mtuSize) + " ! mylistener name=rtplistener ";

			return videoStr;
		}
		else
		{
			if (m_data->source == 0)
				return "";
			return "! mylistener name=preRtplistener "+m_data->source->BuildStringH264() + " ! mylistener name=rtplistener ";
		}
	}

	std::string EyegazeCameraVideoSrc::_generateString(int i)
	{
		std::string videoStr;
		math::vector2di framesize = m_data->source->GetFrameSize(i);
		if (i < m_data->source->GetVideoSrcCount())
		{
			/*	videoStr = "ksvideosrc";
				videoStr += " name=src" + core::StringConverter::toString(i);
				videoStr += " device-index=" + core::StringConverter::toString(m_impl->m_cams[i]);

				//videoStr += "videotestsrc ";
				//" do-timestamp=true is-live=true "//"block=true"
				videoStr += " ! video/x-raw,width=" + core::StringConverter::toString(framesize.x) +
				",height=" + core::StringConverter::toString(framesize.y) + " ! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ! videoconvert ";// +",framerate=" + core::StringConverter::toString(m_fps) + "/1 ";
				*/

			videoStr = m_data->source->GetCameraStr(i);
		}
		else
			videoStr = "videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(framesize.x) +
			",height=" + core::StringConverter::toString(framesize.y);

		return videoStr;

	}

	std::string EyegazeCameraVideoSrc::_generateFoveatedString(int i, math::vector2di framesize,math::vector2di divScaler)
	{
		//math::vector2di framesize = m_data->source->GetFrameSize(0);	
		math::vector2di cropSize = m_data->m_cropsize;

		m_data->_divScaler = divScaler;

		//cropSize /= divScaler;

		if (cropSize.x % 2 == 1)
			cropSize.x += 1;
		if (cropSize.y % 2 == 1)
			cropSize.y += 1;


		int sceneWidth = (framesize.x* cropSize.y) / framesize.y;
		int sceneHeight = (framesize.y* cropSize.x) / framesize.x;

		if (sceneWidth % 2 == 1)
			sceneWidth += 1;

		if (sceneHeight % 2 == 1)
			sceneHeight += 1;

		int lostWidth = framesize.x - sceneWidth*framesize.y / cropSize.y;
		int lostHeight = framesize.y - sceneHeight*framesize.x / cropSize.x;


		std::string videoStr = "";
		if (m_data->m_eyepos.size() <= i)
			m_data->m_eyepos.push_back(math::vector2df(0.5f, 0.5f));
		math::vector2di gazePos(framesize.x*m_data->m_eyepos[i].x, framesize.y*m_data->m_eyepos[i].y);
		math::vector4di cropRect(gazePos.x - cropSize.x / 2, framesize.x - (gazePos.x + cropSize.x / 2),
			gazePos.y - cropSize.y / 2, framesize.y - (gazePos.y + cropSize.y / 2));//left,right,top,bottom

		std::string mName = "mix_" + core::StringConverter::toString(i);
		std::string tName = "t_" + core::StringConverter::toString(i);
		std::string mixerStr = "";
		if (m_data->m_levels > 0)
		{
			mixerStr = "videomixer name=" + mName;
			//mixerStr += "  sink_0::xpos=0 sink_0::ypos=0  sink_0::zorder=0 sink_1::alpha=1  ";
			char buffer[256];
			for (int level = 0; level < m_data->m_levels; ++level){
				int xpos = cropSize.x*level;
				sprintf(buffer, "  sink_%d::xpos=%d sink_%d::ypos=0  sink_%d::zorder=0 sink_%d::alpha=1  ", level, xpos, level, level, level, level);
				mixerStr += buffer;
				//mixerStr += "  sink_2::xpos=0 sink_2::ypos=" + core::StringConverter::toString(cropSize.y) + "  sink_2::zorder=0 sink_2::alpha=1  ";
			}
			sprintf(buffer, "  sink_%d::xpos=%d sink_%d::ypos=0  sink_%d::zorder=0 sink_%d::alpha=1  ", m_data->m_levels, cropSize.x*m_data->m_levels, m_data->m_levels, m_data->m_levels, m_data->m_levels, m_data->m_levels);
			//mixerStr += "  sink_2::xpos=" + core::StringConverter::toString(cropSize.x) + " sink_2::ypos=0 sink_2::zorder=0 sink_2::alpha=1  ";
			mixerStr += buffer;

			videoStr += mixerStr;
		}

		for (int level = 0; level < m_data->m_levels; ++level){
			videoStr += tName + ". ! queue "; //! queue

			cropRect = m_data->GetCropRect(i, level, framesize, m_data->_divScaler);

			m_data->streamSize.x += cropSize.x;


			//eyegaze string
			videoStr += " ! videobox name=box_" + core::StringConverter::toString(i) + "_" + core::StringConverter::toString(level) +
				" left=" + core::StringConverter::toString(cropRect.x) +
				" right=" + core::StringConverter::toString(cropRect.y) +
				" top=" + core::StringConverter::toString(cropRect.z) +
				" bottom=" + core::StringConverter::toString(cropRect.w);

			videoStr += " ! videoscale add-borders=false method=6 sharpen=1 envelope=4 ! video/x-raw,width=" + core::StringConverter::toString(cropSize.x) + ",height=" + core::StringConverter::toString(cropSize.y) +
				" ! " + mName + ".sink_" + core::StringConverter::toString(level) + " ";
		}

		
		if (m_data->m_levels > 0){
			videoStr += tName + ". ! queue "; //
			videoStr += " !  videoscale add-borders=false method=6 sharpen=0 envelope=1 ! video/x-raw,width=" + core::StringConverter::toString(sceneWidth) + ",height=" + core::StringConverter::toString(cropSize.y)+" ";// +" ! videoconvert ";
			m_data->streamSize.x += sceneWidth;
		}
		else{
			m_data->streamSize.x += framesize.x;

		}
		if (m_data->m_levels > 0){
			videoStr += " ! " + mName + ".sink_" + core::StringConverter::toString(m_data->m_levels) + " ";//output to the mixer


			videoStr += mName + ". ";
		}
		return videoStr;

	}
	std::string EyegazeCameraVideoSrc::_generateFullString()
	{

		std::string videoStr;
		bool mixer = false;
		math::vector2di framesize = m_data->source->GetFrameSize(0);

		int camsCount = 0;
		float totalWidth = 0;
		float totalHeight = 0;

		for (int i = 0; i < m_data->source->GetVideoSrcCount(); ++i)
		{
			totalWidth += framesize.x;
			totalHeight += framesize.y;
			camsCount++;
		}

		m_data->streamSize.x = 0;
		m_data->streamSize.y = 0;
		if (camsCount == 0)
		{
			videoStr = "videotestsrc ! video/x-raw,width=" + core::StringConverter::toString(framesize.x) +
				",height=" + core::StringConverter::toString(framesize.y);
		}
		else
		{

			if ( camsCount > 1)//!m_data->m_separateStreams  &&
			{
				gLogManager.log("starting with multiple streams",ELL_INFO);
				mixer = true;
				videoStr = " videomixer name=mix_eyes ";
				int xpos = 0;
				int ypos = 0;
				for (int i = 0; i < camsCount; ++i)
				{
					std::string name = "sink_" + core::StringConverter::toString(i);
					//videoStr += "  " + name + "::xpos=" + core::StringConverter::toString(xpos) + " " + name + "::ypos=0  " + name + "::zorder=0 " + name + "::zorder=1  ";
					videoStr += "  " + name + "::xpos=0 " + name + "::ypos=" + core::StringConverter::toString(ypos) + " " + name + "::zorder=0 " + name + "::zorder=1  ";
					xpos += m_data->m_cropsize.x;
					ypos += m_data->m_cropsize.y;
					m_data->streamSize.y += m_data->m_cropsize.y;
				}
			}
			else{
				m_data->streamSize.y = m_data->m_cropsize.y;

			}

			int counter = 0;
			core::string tName;

			bool precodecSet = false;
			for (int i = 0; i < m_data->source->GetVideoSrcCount(); ++i)
			{
				videoStr += GetCameraStr(i);
				if (!precodecSet)
				{
					videoStr += "! mylistener name=precodec ";
					precodecSet = true;
				}
				tName = "t_" + core::StringConverter::toString(i);
				if (m_data->m_levels > 0){
					videoStr += "! tee name=" + tName + " ";
				}

				//			if (m_data->source->IsAvailable(i))
				{

					videoStr += _generateFoveatedString(i,framesize);

					if (mixer)
					{
						videoStr += " ! mix_eyes.sink_" + core::StringConverter::toString(counter) + " ";
					}
					++counter;
				}

			}

			if (mixer)
			{
				//	videoStr += " mix. ! videoflip method=5 ";// "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
				videoStr += " mix_eyes.  ";// "! videorate max-rate=" + core::StringConverter::toString(m_fps) + " ";
			}
			else
			{
				//videoStr += " ! mylistener name=precodec ";
			}

			videoStr += " ! videoconvert ";
		}

		//fprintf(m_data->averageBytesFile, "%dx%d\n", m_data->streamSize.x, m_data->streamSize.y);
		return videoStr;
	}


	std::string EyegazeCameraVideoSrc::GetEncodingStr()
	{
		std::string videoStr;

		if (m_encoder == "H264")
		{

			videoStr += BuildStringH264();
			//interlaced=true sliced-threads=false  "// 
			//videoStr += " ! rtph264pay ";
		}
		if (m_encoder == "JPEG")
		{
			videoStr = "! jpegenc  ";
			videoStr += " ! rtpjpegpay ";
		}

		return videoStr;
	}
	std::string EyegazeCameraVideoSrc::GetCameraStr(int i)
	{
		if (m_data->source->GetVideoSrcCount() <= i)
			return "";
		return m_data->source->GetCameraStr(i);
	}
	void  EyegazeCameraVideoSrc::SetResolution(int width, int height, int fps, bool free)
	{
		m_data->source->SetResolution(width, height, fps, free);
	}
	void EyegazeCameraVideoSrc::_setFoveatedRectsCount(int count)
	{
		m_data->foveatedRectsCount = count;
	}

	std::string EyegazeCameraVideoSrc::GetPipelineStr(int i)
	{
		std::string videoStr;
		videoStr = _generateFullString();
		videoStr += GetEncodingStr();
		return videoStr;
	}

	void EyegazeCameraVideoSrc::SetCameraSource(ICustomVideoSrc* source)
	{
		m_data->source = source;
		if (source != 0){
			m_data->m_separateStreams = source->IsSeparateStreams();
			_setFoveatedRectsCount(source->GetVideoSrcCount());
		}
	}
	math::vector2di EyegazeCameraVideoSrc::GetFrameSize(int i)
	{
		return m_data->source->GetFrameSize(i);
	}

	void EyegazeCameraVideoSrc::LinkWithPipeline(void* pipeline)
	{
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
	void EyegazeCameraVideoSrc::SetEyegazeLevels(int levels)
	{
		m_data->m_levels = levels;
	}
	int EyegazeCameraVideoSrc::GetEyegazeLevels()
	{
		return m_data->m_levels;
	}
	math::vector2di EyegazeCameraVideoSrc::GetEyeCropSize()
	{
		return m_data->m_cropsize;
	}
	void EyegazeCameraVideoSrc::SetSeparateStreams(bool separate)
	{
		m_data->m_separateStreams = separate;
	}
	int EyegazeCameraVideoSrc::GetCurrentFPS(int i)
	{
		return m_data->source->GetCurrentFPS(i);
	}
	int EyegazeCameraVideoSrc::GetVideoSrcCount()
	{

		return m_data->source->GetVideoSrcCount();
	}
}
}

