

#include "stdafx.h"
#include "VideoAppSinkHandler.h"
#include "PixelUtil.h"
#include "ILogManager.h"
#include "Engine.h"
#include "ITimer.h"
#include "IThreadManager.h"

#include <gst/video/video.h>

namespace mray
{
namespace video
{


	VideoAppSinkHandler::VideoAppSinkHandler()
	{
		m_IsFrameNew = false;
		m_HavePixelsChanged = false;
		m_BackPixelsChanged = false;
		m_IsAllocated = false;
		m_frameID = 0;
		m_surfaceCount = 1;
		m_captureFPS = 0;
		m_frameCount = 0;

		m_mutex = OS::IThreadManager::getInstance().createMutex();
	}
	VideoAppSinkHandler::~VideoAppSinkHandler()
	{
	}
	void VideoAppSinkHandler::Close()
	{
		m_frameID = 0;
		m_IsAllocated = false;
		for (int i = 0; i < m_surfaceCount; ++i)
		{
			m_pixels[i].data.clear();
			m_backPixels[i].data.clear();
		}
		//	m_eventPixels.clear();
	}


	static GstVideoInfo getVideoInfo(GstSample * sample){
		GstCaps *caps = gst_sample_get_caps(sample);
		GstVideoInfo vinfo;
		gst_video_info_init(&vinfo);
		if (caps){
			gst_video_info_from_caps(&vinfo, caps);
		}
		return vinfo;
	}

	video::EPixelFormat getVideoFormat(GstVideoFormat format){
		switch (format){
		case GST_VIDEO_FORMAT_GRAY8:
			return EPixel_LUMINANCE8;

		case GST_VIDEO_FORMAT_RGB:
			return EPixel_R8G8B8;

		case GST_VIDEO_FORMAT_BGR:
			return EPixel_B8G8R8;

		case GST_VIDEO_FORMAT_RGBA:
			return EPixel_R8G8B8A8;

		case GST_VIDEO_FORMAT_BGRA:
			return EPixel_B8G8R8A8;

		case GST_VIDEO_FORMAT_RGB16:
			return EPixel_R5G6B5;

		case GST_VIDEO_FORMAT_I420:
			return EPixel_YUYV;

		default:
			return EPixel_Unkown;
		}
	}
	GstFlowReturn VideoAppSinkHandler::process_sample(std::shared_ptr<GstSample> sample){
		GstBuffer * _buffer = gst_sample_get_buffer(sample.get());



		GstVideoInfo vinfo = getVideoInfo(sample.get());
		video::EPixelFormat fmt = getVideoFormat(vinfo.finfo->format);
		m_pixelFormat = fmt;
		if (fmt == video::EPixel_Unkown)
		{
			return GST_FLOW_ERROR;
		}
		bool isI420 = false;;

		int height = vinfo.height;
		if (fmt == video::EPixel_YUYV)
		{
			isI420 = true;
			//fmt = video::EPixel_LUMINANCE8;
			height *= 1.5;
		}
		if (m_pixels[0].data.imageData && (m_pixels[0].data.Size.x != vinfo.width || m_pixels[0].data.Size.y != height || m_pixels[0].data.format != fmt))
		{
			m_IsAllocated = false;
			m_pixels[0].data.clear();
			m_backPixels[0].data.clear();
		}

		gst_buffer_map(_buffer, &mapinfo, GST_MAP_READ);
		guint size = mapinfo.size;
		float pxSize = video::PixelUtil::getPixelDescription(fmt).elemSizeB;

		int stride = 0;
		int dataSize = m_pixels[0].data.Size.x*m_pixels[0].data.Size.y;
		if (isI420)
		{
		}
		else
			dataSize *= pxSize;

		if (m_pixels[0].data.imageData && dataSize != (int)size){
			GstVideoInfo vinfo = getVideoInfo(sample.get());
			stride = vinfo.stride[0];

			if (stride != (m_pixels[0].data.Size.x * pxSize)) {
				gst_buffer_unmap(_buffer, &mapinfo);
				gLogManager.StartLog(ELL_WARNING) << "VideoAppSinkHandler::process_sample(): error on new buffer, buffer size: " << size << "!= init size: " << dataSize;
				gLogManager.flush();
				return GST_FLOW_ERROR;
			}
		}
		m_mutex->lock();
		buffer = sample;

		if (m_pixels[0].data.imageData){
			++m_frameID;
			//if (stride > 0) {
			m_backPixels[0].data.setData(mapinfo.data, m_pixels[0].data.Size, m_pixels[0].data.format);
			m_backPixels[0].PTS = _buffer->pts;
			m_backPixels[0].DTS = _buffer->dts;
			// 		}
			// 		else {
			// 			m_backPixels[0].setData(mapinfo.data, m_pixels[0].Size, m_pixels[0].format);
			// 			m_eventPixels.setData(mapinfo.data, m_pixels[0].Size, m_pixels[0].format);
			// 		}

			m_BackPixelsChanged = true;
			m_mutex->unlock();
			if (stride == 0) {
				//	ofNotifyEvent(prerollEvent, eventPixels);
			}
		}
		else{

			if (isI420)
				_Allocate(vinfo.width, vinfo.height*1.5, fmt);
			else
				_Allocate(vinfo.width, vinfo.height, fmt);

			m_mutex->unlock();
			FIRE_LISTENR_METHOD(OnStreamPrepared, (this));

		}
		gst_buffer_unmap(_buffer, &mapinfo);
		return GST_FLOW_OK;
	}
	bool VideoAppSinkHandler::_Allocate(int width, int height, video::EPixelFormat fmt)
	{

		if (m_IsAllocated) return true;

		m_frameSize.x = width;
		m_frameSize.y = height;

		m_pixels[0].data.createData(math::vector3di(width, height, 1), fmt);
		m_backPixels[0].data.createData(math::vector3di(width, height, 1), fmt);

		m_HavePixelsChanged = false;
		m_BackPixelsChanged = true;
		m_IsAllocated = true;

		m_frameCount = 0;
		m_timeAcc = 0;
		m_lastT = 0;
		m_captureFPS = 0;

		return m_IsAllocated;
	}




	bool VideoAppSinkHandler::GrabFrame(){
		m_mutex->lock();
		m_HavePixelsChanged = m_BackPixelsChanged;
		if (m_HavePixelsChanged){
			m_BackPixelsChanged = false;
			math::Swap(m_pixels[0].data.imageData, m_backPixels[0].data.imageData);
			math::Swap(m_pixels[0].DTS, m_backPixels[0].DTS);
			math::Swap(m_pixels[0].PTS, m_backPixels[0].PTS);

			prevBuffer = buffer;


			float t = gEngine.getTimer()->getSeconds();
			m_timeAcc += (t - m_lastT)*0.001f;

			++m_frameCount;
			if (m_timeAcc > 1)
			{
				m_captureFPS = m_frameCount;
				m_timeAcc = m_timeAcc - (int)m_timeAcc;
				m_frameCount = 0;
				//	printf("Capture FPS: %d\n", m_captureFPS);
			}

			m_lastT = t;
		}

		m_mutex->unlock();
		m_IsFrameNew = m_HavePixelsChanged;
		m_HavePixelsChanged = false;
		return m_IsFrameNew;
	}



	float VideoAppSinkHandler::GetCaptureFrameRate()
	{
		return m_captureFPS;
	}



}
}