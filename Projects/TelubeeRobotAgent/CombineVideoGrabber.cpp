

#include "stdafx.h"
#include "CombineVideoGrabber.h"



#include "stdafx.h"
#include "CombineVideoGrabber.h"

#include "Engine.h"


namespace mray
{


	CombineVideoGrabber::CombineVideoGrabber(video::IVideoGrabber* g1, video::IVideoGrabber* g2)
	{
		m_g1 = 0;
		m_g2 = 0;
		m_bufferID=0;
		SetGrabbers(g1, g2);
	}

	void CombineVideoGrabber::SetGrabbers(video::IVideoGrabber* g1, video::IVideoGrabber* g2)
	{
		m_g1 = g1;
		m_g2 = g2;
		m_targetSize = m_g1->GetFrameSize();
		m_targetSize.y *= 2;
		m_lastImage.createData(math::vector3di(m_targetSize.x, m_targetSize.y, 1), GetImageFormat());
	}

	void CombineVideoGrabber::SetFrameSize(int w, int h)
	{
	}
	const math::vector2di& CombineVideoGrabber::GetFrameSize()
	{
		return m_targetSize;
	}

	void CombineVideoGrabber::SetImageFormat(video::EPixelFormat fmt)
	{
	}
	video::EPixelFormat CombineVideoGrabber::GetImageFormat()
	{
		return video::EPixel_R8G8B8;
	}
	/*
	void CombineVideoGrabber::_RotateImage(const video::ImageInfo* src, video::ImageInfo* dst, const math::recti &srcRect, bool cw)
	{
		dst->createData(math::vector3d(srcRect.getHeight(), srcRect.getWidth(), 1), src->format);

		struct pixel
		{
			uchar r, g, b;
		};
		pixel* psrc = (pixel*)src->imageData;
		pixel* pdst= (pixel*)dst->imageData;
				int indexSrc = 0;
				int indexDst = 0;
		for (int i = srcRect.ULPoint.x; i < srcRect.BRPoint.x; ++i)
		{
			for (int j = srcRect.ULPoint.y; j < srcRect.BRPoint.y; ++j)
			{

				indexSrc = (src->Size.y - j - 1)*src->Size.x + (i);
				if (cw)
					indexDst = (i - srcRect.ULPoint.x)*srcRect.getHeight() + j - srcRect.ULPoint.y;
				else
					indexDst = (srcRect.getWidth() - (i - srcRect.ULPoint.x) - 1)*srcRect.getHeight() + (srcRect.getHeight() - (j - srcRect.ULPoint.y) - 1);
				pdst[indexDst] = psrc[indexSrc];
			}
		}
	}*/

	bool CombineVideoGrabber::GrabFrame()
	{
		m_newFrame = false;
		bool ret = false;
		bool a = false, b = false;

		if (m_g1 && m_g1->GrabFrame())
			a = true;
		if (m_g2 && m_g2->GrabFrame())
			b = true;
		ret = a | b;
		if (!ret)
			return false;
		return true;
		m_newFrame = true;

		m_bufferID++;
		a = b = false;


		if (m_g1->GetLastFrame()->Size.x > 0)a = true;
		if (m_g2->GetLastFrame()->Size.x > 0)b = true;

		memcpy(m_lastImage.imageData, m_g1->GetLastFrame()->imageData, m_g1->GetLastFrame()->imageDataSize);
		memcpy(m_lastImage.imageData + m_g1->GetLastFrame()->imageDataSize, m_g2->GetLastFrame()->imageData, m_g2->GetLastFrame()->imageDataSize);


		return true;
	}
	bool CombineVideoGrabber::HasNewFrame()
	{
		return m_newFrame;
	}


	const video::ImageInfo* CombineVideoGrabber::GetLastFrame()
	{
		return &m_lastImage;
	}

}


