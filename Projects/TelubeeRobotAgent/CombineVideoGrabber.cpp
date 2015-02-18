

#include "stdafx.h"
#include "CombineVideoGrabber.h"



#include "stdafx.h"
#include "CombineVideoGrabber.h"

#include "Engine.h"


namespace mray
{


	CombineVideoGrabber::CombineVideoGrabber()
	{
		m_g1 = 0;
		m_g2 = 0;
		m_bufferID=0;
		m_rotate90 = false;
#if USE_GPU
		m_rtTex = Engine::getInstance().getDevice()->createEmptyTexture2D(false);
		m_tex1 = Engine::getInstance().getDevice()->createEmptyTexture2D(false);
		m_tex2 = Engine::getInstance().getDevice()->createEmptyTexture2D(false);
		m_rt = Engine::getInstance().getDevice()->createRenderTarget("", m_rtTex, 0, 0, 0);
#endif
	}

	void CombineVideoGrabber::SetGrabbers(video::IVideoGrabber* g1, video::IVideoGrabber* g2)
	{
		m_g1 = g1;
		m_g2 = g2;
	}

	void CombineVideoGrabber::SetFrameSize(int w, int h)
	{
		m_targetSize.set(w, h);
		m_lastImage.createData(math::vector3di(w, h, 1), GetImageFormat());
#if USE_GPU
		m_rtTex->createTexture(math::vector3di(w, h, 1), GetImageFormat());
#endif
	}
	const math::vector2di& CombineVideoGrabber::GetFrameSize()
	{
		return m_g1->GetFrameSize();
	}

	void CombineVideoGrabber::SetImageFormat(video::EPixelFormat fmt)
	{
	}
	video::EPixelFormat CombineVideoGrabber::GetImageFormat()
	{
		return video::EPixel_R8G8B8;
	}

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
	}

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


#if !USE_GPU
		math::vector2di targetSize(900, 720);
		int widthDiff = abs(targetSize.x - m_g1->GetFrameSize().x);
		math::recti srcRect = math::recti(widthDiff / 2, 0, m_g1->GetFrameSize().x - widthDiff / 2, m_g1->GetFrameSize().y);
		srcRect.Repair();

		const video::ImageInfo* f1, *f2;
		f1 = &m_frame1;
		f2 = &m_frame2;
		if (m_rotate90 )
		{
			if (a)
			{
				_RotateImage(m_g1->GetLastFrame(), &m_frame1, srcRect, false);
			}
			if (b)
			{
				_RotateImage(m_g2->GetLastFrame(), &m_frame2, srcRect, true);
			}
		}
		else
		{
			if (a)
			{
				m_frame1.setData(m_g1->GetLastFrame()->imageData, m_g1->GetLastFrame()->Size, m_g1->GetLastFrame()->format);
			}
			if (b)
			{
				m_frame2.setData(m_g2->GetLastFrame()->imageData, m_g2->GetLastFrame()->Size, m_g2->GetLastFrame()->format);
			}
		}
		if (a)
			video::ColorConverter::resizeImage( &m_frame1, math::Point2di(m_targetSize.x / 2, m_targetSize.y));
		if (b)
			video::ColorConverter::resizeImage( &m_frame2, math::Point2di(m_targetSize.x / 2, m_targetSize.y));



		//if (a && b)
		{
			for (int i = 0; i < m_targetSize.x / 2; ++i)
			{
				for (int j = 0; j < m_targetSize.y; ++j)
				{
					int index1 = (j*m_targetSize.x + i) * 3;//for frame1
					int index2 = (j*m_targetSize.x + i + m_targetSize.x / 2) * 3;//for frame1
					int index = (j*m_targetSize.x / 2 + i) * 3;//for frame
					if (a)
					{
						m_lastImage.imageData[index1 + 0] = f1->imageData[index + 0];
						m_lastImage.imageData[index1 + 1] = f1->imageData[index + 1];
						m_lastImage.imageData[index1 + 2] = f1->imageData[index + 2];
					}
					else
					{
						m_lastImage.imageData[index1 + 0] = m_lastImage.imageData[index1 + 1] = m_lastImage.imageData[index1 + 2] = 0;
					}

					if (b)
					{
						m_lastImage.imageData[index2 + 0] = f2->imageData[index + 0];
						m_lastImage.imageData[index2 + 1] = f2->imageData[index + 1];
						m_lastImage.imageData[index2 + 2] = f2->imageData[index + 2];
					}
					else
					{
						m_lastImage.imageData[index2 + 0] = m_lastImage.imageData[index2 + 1] = m_lastImage.imageData[index2 + 2] = 0;
					}
				}
			}
		}
		/*
		else if (a)
		{
			video::ColorConverter::resizeImage(&m_frame1, math::Point2di(m_targetSize.x, m_targetSize.y));

			m_lastImage.setData(m_frame1.imageData, m_frame1.Size, m_frame1.format);
		}
		else if (b)
		{
			video::ColorConverter::resizeImage(&m_frame2, math::Point2di(m_targetSize.x, m_targetSize.y));
			m_lastImage.setData(m_frame2.imageData, m_frame2.Size, m_frame2.format);
		}*/
#else

		video::IVideoDevice* dev = Engine::getInstance().getDevice();
		dev->setRenderTarget(m_rt);
		video::LockedPixelBox lpb;
		if (m_g1->GetLastFrame()->Size != m_tex1->getSize())
			m_tex1->createTexture(m_g1->GetLastFrame()->Size, m_g1->GetLastFrame()->format);

		if (m_g2->GetLastFrame()->Size != m_tex2->getSize())
			m_tex2->createTexture(m_g2->GetLastFrame()->Size, m_g2->GetLastFrame()->format);

		if (a)
		{
			lpb.box = math::box3d(0, m_g1->GetLastFrame()->Size);
			lpb.format = m_g1->GetLastFrame()->format;
			lpb.data = m_g1->GetLastFrame()->imageData;
			m_tex1->getSurface(0)->blitFromMemory(lpb);
		}
		if (b)
		{
			lpb.box = math::box3d(0, m_g2->GetLastFrame()->Size);
			lpb.format = m_g2->GetLastFrame()->format;
			lpb.data = m_g2->GetLastFrame()->imageData;
			m_tex2->getSurface(0)->blitFromMemory(lpb);
		}
		video::TextureUnit tu;

		if (a && b)
		{
			math::rectf rc;
			tu.SetTexture(m_tex1);
			dev->useTexture(0, &tu);
			rc = math::rectf(0, math::vector2d(m_targetSize.x / 2, m_targetSize.y));
			dev->draw2DImage(rc, 1);

			tu.SetTexture(m_tex2);
			dev->useTexture(0, &tu);
			rc.ULPoint.x += m_targetSize.x / 2;
			rc.BRPoint.x += m_targetSize.x / 2;
			dev->draw2DImage(rc, 1);
		}
		else if (a)
		{
			math::rectf rc;
			tu.SetTexture(m_tex1);
			dev->useTexture(0, &tu);
			rc = math::rectf(0, math::vector2d(m_targetSize.x, m_targetSize.y));
			dev->draw2DImage(rc, 1);
		}
		else if (b)
		{
			math::rectf rc;
			tu.SetTexture(m_tex2);
			dev->useTexture(0, &tu);
			rc = math::rectf(0, math::vector2d(m_targetSize.x, m_targetSize.y));
			dev->draw2DImage(rc, 1);
		}
		Engine::getInstance().getDevice()->setRenderTarget(0);

		lpb = m_rtTex->getSurface(0)->lock(math::box3d(0, math::Point3di(m_targetSize.x, m_targetSize.y, 1)), video::IHardwarePixelBuffer::ELO_ReadOnly);
		m_lastImage.setData(lpb.data, math::Point3di(m_targetSize.x, m_targetSize.y, 1), GetImageFormat());
		m_rtTex->getSurface(0)->unlock();

#endif

		return true;
	}
	bool CombineVideoGrabber::HasNewFrame()
	{
		return m_g1->HasNewFrame();
	}


	const video::ImageInfo* CombineVideoGrabber::GetLastFrame()
	{
		return m_g1->GetLastFrame();
	}

}


