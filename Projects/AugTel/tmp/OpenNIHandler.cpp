

#include "stdafx.h"
#include "OpenNIHandler.h"
#include "OpenNIManager.h"
#include "OpenNIUtils.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "IThreadManager.h"

namespace mray
{
namespace AugTel
{


	class OpenNIHandlerThread :public OS::IThreadFunction
	{
		OpenNIHandler* m_owner;
	public:
		bool destroy;
		OpenNIHandlerThread(OpenNIHandler* owner)
		{
			m_owner = owner;
			destroy = false;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive() && !destroy)
			{
				if (m_owner->IsStarted())
				{
					m_owner->_Update();
				}
				else
				{
					Sleep(100);
				}
		//		Sleep(10);
			}
		}
	};

OpenNIHandler::OpenNIHandler()
{
	m_minDepth = 1;

	m_threadFunc = 0;
	m_newBuffer = false;
	m_viewDepth = false;
	m_size.set(320, 240);

	m_center = 0.5;
	m_scale = 1;

}
OpenNIHandler::~OpenNIHandler()
{
	if (m_threadFunc)
	{
		m_threadFunc->destroy = true;
		OS::IThreadManager::getInstance().killThread(m_thread);
	}
}

void OpenNIHandler::Init()
{

	m_openNiCamera = OpenNIManager::getInstance().CreateDeviceAny();

	m_depthTex = Engine::getInstance().getDevice()->createEmptyTexture2D(false);
	m_depthTex->setMipmapsFilter(false);
	m_viewDepth = false;

	m_threadFunc = new OpenNIHandlerThread(this);
	m_thread = OS::IThreadManager::getInstance().createThread(m_threadFunc);
	m_mutix = OS::IThreadManager::getInstance().createMutex();
	m_thread->start(0);
	m_currentData = 0;
}

void OpenNIHandler::Start()
{
	if (m_openNiCamera->Open(m_size.x, m_size.y, true, false) != openni::STATUS_OK)
		return;
	openni::VideoStream& depthStream = m_openNiCamera->GetDepthStream();

	float hfov = math::toDeg(depthStream.getHorizontalFieldOfView());
	float vfov = math::toDeg(depthStream.getVerticalFieldOfView());
	float minV = depthStream.getMinPixelValue();
	float maxV = depthStream.getMaxPixelValue();
	m_started = true;


	printf("Depth stream started - Horizontal=%f , Vertical=%f , min=%f, max=%f\n", hfov, vfov, minV, maxV);
}

void OpenNIHandler::Close()
{
	m_started = false;
	m_openNiCamera->Close();
}

void OpenNIHandler::Render(const math::rectf& rc, float alpha)
{
	video::TextureUnit tex;
	video::IVideoDevice* device = Engine::getInstance().getDevice();

	tex.SetTexture(m_depthTex);
	device->useTexture(0, &tex);

	math::rectf drc(m_center - m_scale*0.5f, m_center + m_scale*0.5f);
	drc.ULPoint *= rc.getSize();
	drc.BRPoint *= rc.getSize();
	device->draw2DImage(drc, video::SColor(1, 1, 1, alpha));

	if (0)
	{
		float maxHisto = 0;
		for (int i = 0; i < m_depthHisto.size(); ++i)
		{
			if (m_depthHisto[i]>maxHisto)
			{
				maxHisto = m_depthHisto[i];
			}
		}

		math::Point2df pts[1000];
		for (int i = 0; i < m_depthHisto.size() / 10; i++)
		{
			int idx = i * 10;
			pts[i].x = (rc.getWidth()*idx) / m_depthHisto.size();
			pts[i].y = 300 - 100 * m_depthHisto[idx] / maxHisto;
		}
		device->draw2DLine(pts, m_depthHisto.size() / 10, video::SColor(1, 1, 0, 1));
	}

}


void OpenNIHandler::Update(float dt)
{
	if (m_newBuffer)
	{
		m_mutix->lock();
		if (m_depthTex->getSize().x != m_currentData->Size.x || m_depthTex->getSize().y != m_currentData->Size.y)
		{
			m_depthTex->createTexture(math::vector3di(m_currentData->Size.x, m_currentData->Size.y, 1), video::EPixel_R8G8B8);
		}
		video::IHardwarePixelBuffer* buff = m_depthTex->getSurface(0);
	//	video::LockedPixelBox box = buff->lock(math::box3d(0, m_depthTex->getSize()), video::IHardwareBuffer::ELO_Discard);

		video::LockedPixelBox bb;
		bb.box.MinP = 0;
		bb.box.MaxP = m_currentData->Size;
		bb.format = m_currentData->format;
		bb.data = m_currentData->imageData;
		bb.rowPitch = m_currentData->Size.x;
		bb.slicePitch = m_currentData->Size.x*m_currentData->Size.y;

		buff->blitFromMemory(bb);
		m_newBuffer = false;
		m_mutix->unlock();
	}
}
void OpenNIHandler::_Update()
{

	openni::VideoStream& depthStream = m_openNiCamera->GetDepthStream();

	static openni::VideoFrameRef depthFrame;
	static bool readed = false;
	if (depthStream.isValid())
	{
		if (!readed)
		{
			depthStream.readFrame(&depthFrame);
			//	readed = true;
		}
		video::ImageInfo* currentData;
		if (m_currentData == &m_depthData[0])
			currentData = &m_depthData[1];
		else
			currentData = &m_depthData[0];

		if (m_depthData[0].Size.x != depthFrame.getWidth() || m_depthData[0].Size.y != depthFrame.getHeight())
		{
			m_mutix->lock();
			m_depthData[0].createData(math::vector3d(depthFrame.getWidth(), depthFrame.getHeight(), 1), video::EPixel_R8G8B8);
			m_depthData[1].createData(math::vector3d(depthFrame.getWidth(), depthFrame.getHeight(), 1), video::EPixel_R8G8B8);
			m_mutix->unlock();
		}

#if 0

		{
			if (m_depthHisto.size() != depthStream.getMaxPixelValue())
				m_depthHisto.resize(depthStream.getMaxPixelValue());
			OpenNIUtils::CalculateHistogram(depthFrame, m_depthHisto);

			OpenNIUtils::ConvertToTexture(depthFrame, box, &m_depthHisto);

			float v = 255;// *m_minDepth->GetValue() / m_minDepth->GetMaxValue();
			int pixelsCount = box.box.getWidth()*box.box.getHeight();
			for (int i = 0; i < pixelsCount; ++i)
			{
				int idx = i * 4;
				if (((uchar*)box.data)[idx + 0] < v)
				{
					((uchar*)box.data)[idx + 0] = ((uchar*)box.data)[idx + 1] = ((uchar*)box.data)[idx + 2] = 0;
				}
			}
			video::ImageInfo img;
			img.imageData = (uchar*)box.data;
			img.Size = box.box.MaxP - box.box.MinP;
			img.format = box.format;
			img.autoDel = false;
			//video::ColorConverter::createNormalMap(&img, v);
			box.data = img.imageData;
			box.format = img.format;
		}
#else
		{
			float v = 255 * m_minDepth;
			m_normalCalc.SetAmplitude(v);
			m_normalCalc.SetDepthFrame(depthStream);
			math::vector3d* normals = m_normalCalc.GetNormalData();
			float *depth = m_normalCalc.GetDepthData();
			uchar* ptr = (uchar*)currentData->imageData;

			static float angle = 0;
			math::vector3d lightPos;
			lightPos.x = 10 * cos(angle);
			lightPos.z = 10 * sin(angle);
			angle += gFPS.dt();
			lightPos.Normalize();

			for (int y = 0; y < m_normalCalc.Height(); ++y)
			{
				for (int x = 0; x < m_normalCalc.Width(); ++x)
				{
					if (false)
					{
						math::vector3d clr = math::vector3d(1, 1, 1)*(*normals).dotProduct(lightPos);
						(*ptr++) = (uchar)(clr.x * 255);
						(*ptr++) = (uchar)(clr.y * 255);
						(*ptr++) = (uchar)(clr.z * 255);
						++normals;
					}
					else if (!m_viewDepth)
					{
						(*ptr++) = (uchar)(math::Max<float>(0,(*normals).x*0.5f+0.5f)* 255);
						(*ptr++) = (uchar)(math::Max<float>(0, (*normals).y*0.5f + 0.5f) * 255);
						(*ptr++) = (uchar)(math::Max<float>(0, (*normals).z*0.5f + 0.5f) * 255);
						++normals;
					}
					else
					{
						(*ptr++) = (uchar)((*depth) * 255);
						(*ptr++) = (uchar)((*depth) * 255);
						(*ptr++) = (uchar)((*depth) * 255);

						++depth;
					}
				}
			}
		}

#endif
		m_mutix->lock();
		m_currentData = currentData;
		m_newBuffer = true;
		m_mutix->unlock();
	}

}


bool OpenNIHandler::ConvertToImageSpace(const math::vector2d& pos, math::vector2di& ret)
{

	math::rectf rc(m_center - m_scale*0.5f, m_center + m_scale*0.5f);
	if (!rc.IsPointInside(pos))
		return false;

	math::vector2d p=pos- rc.ULPoint;
	p /= m_scale;
	ret=p*math::vector2d(m_size);
	return true;


}

}
}

