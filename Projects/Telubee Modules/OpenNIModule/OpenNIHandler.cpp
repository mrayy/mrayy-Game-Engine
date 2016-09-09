

#include "stdafx.h"

#include "OpenNIHandler.h"
#include "OpenNIManager.h"
#include "OpenNIUtils.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "IThreadManager.h"

namespace mray
{
	namespace TBee
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

	m_threadFunc = 0;
	m_size.set(320,240);

	m_center = 0.5;
	m_scale = 1;


}
OpenNIHandler::~OpenNIHandler()
{
	if (m_threadFunc)
	{
		m_threadFunc->destroy = true;
		OS::IThreadManager::getInstance().killThread(m_thread);
		delete m_threadFunc;
	}
}

void OpenNIHandler::Init()
{

	m_openNiCamera = OpenNIManager::getInstance().CreateDeviceAny();


	m_threadFunc = new OpenNIHandlerThread(this);
	m_mutix = OS::IThreadManager::getInstance().createMutex();
}
void OpenNIHandler::CreateDepthFrame(int w, int h)
{
	m_size.x = w;
	m_size.y = h;
	m_normalCalc.CreateDepthFrame(w, h);
}
void OpenNIHandler::Start(int w, int h)
{
	m_size.x = w;
	m_size.y = h;
	if (m_openNiCamera->Open(m_size.x, m_size.y, true, false) != openni::STATUS_OK)
		return;
	openni::VideoStream& depthStream = m_openNiCamera->GetDepthStream();

	float hfov = math::toDeg(depthStream.getHorizontalFieldOfView());
	float vfov = math::toDeg(depthStream.getVerticalFieldOfView());
	float minV = depthStream.getMinPixelValue();
	float maxV = depthStream.getMaxPixelValue();
	m_started = true;
	m_thread = OS::IThreadManager::getInstance().createThread(m_threadFunc);
	m_thread->start(0);


	printf("Depth stream started - Horizontal=%f , Vertical=%f , min=%f, max=%f\n", hfov, vfov, minV, maxV);
}

void OpenNIHandler::Close()
{
	m_started = false;
	if (m_threadFunc)
		m_threadFunc->destroy = true;
	OS::IThreadManager::getInstance().killThread(m_thread);
	if (m_openNiCamera)
		m_openNiCamera->Close();
//	delete m_thread;
	m_thread = 0;
	
}


void OpenNIHandler::Update(float dt)
{
	
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

		{
			float v = 100 ;
			m_normalCalc.SetAmplitude(v);
			m_normalCalc.SetDepthFrame(depthFrame);

		}

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
