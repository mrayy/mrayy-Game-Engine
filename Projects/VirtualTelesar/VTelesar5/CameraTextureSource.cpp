
#include "stdafx.h"
#include "CameraTextureSource.h"

#include "VTAppGlobals.h"


#include "DirectShowVideoGrabber.h"

//#define FLYCAMERA_GRABBER
#ifdef FLYCAMERA_GRABBER
#include "FlyCameraVideoGrabber.h"
#endif

#include "DirectShowVideoGrabber.h"


namespace mray
{
namespace video
{

	IMPLEMENT_RTTI(CameraTextureSource,ITextureSource);

	const core::string CameraTextureSource::FactorySourceType=mT("CameraTexture");


	class CameraPoolManager
	{
	protected:
		struct CameraRef
		{
			CameraRef():index(0),m_camera(0),refCount(0)
			{
			}

			void Request()
			{
				++refCount;
				if(refCount==1)
				{
					m_camera->Start();
				}
			}
			void Release()
			{
				--refCount;
				if(refCount==0)
				{
					m_camera->Stop();
				}else if(refCount<0)
					refCount=0;
			}

			int index;
			GCPtr<ICameraVideoGrabber> m_camera;
			int refCount;
		};

		std::map<int,CameraRef> m_cameraPool;
	public:

		static CameraPoolManager Instance;

		CameraPoolManager()
		{
		}
		virtual~CameraPoolManager()
		{
		}

		video::ICameraVideoGrabber* RequestCamera(int i)
		{
			std::map<int,CameraRef>::iterator it=m_cameraPool.find(i);
			if(it!=m_cameraPool.end())
			{
				return it->second.m_camera;
			}
			CameraRef c;
			
			if (VT::VTAppGlobals::Instance()->CameraType == VT::EWebCamera)
				c.m_camera=new video::DirectShowVideoGrabber();
			else
#ifdef FLYCAMERA_GRABBER
			 c.m_camera=new video::FlyCameraVideoGrabber();
#else
			gLogManager.log("VideoGrabber is not recognized!",ELL_WARNING);
#endif

			//c.m_camera->InitDevice(i,640,480,30);
			m_cameraPool[i]=c;
			return c.m_camera;
		}
		void UseCamera(int i)
		{
			std::map<int,CameraRef>::iterator it=m_cameraPool.find(i);
			if(it!=m_cameraPool.end())
			{
				it->second.Request();
			}
		}
		void UnseCamera(int i)
		{
			std::map<int,CameraRef>::iterator it=m_cameraPool.find(i);
			if(it!=m_cameraPool.end())
			{
				it->second.Release();
			}
		}
	};

	CameraPoolManager CameraPoolManager::Instance;

CameraTextureSource::CameraTextureSource()
{
	m_texGrabber=new video::VideoGrabberTexture();
}
CameraTextureSource::~CameraTextureSource()
{
	Unload();
	delete m_texGrabber;
}


bool CameraTextureSource::_Load()
{
	if(m_texGrabber->GetGrabber().isNull())
	{
		video::ICameraVideoGrabber* vdo=CameraPoolManager::Instance.RequestCamera(m_cameraSource.id);
		
		video::ITexturePtr tex=Engine::getInstance().getDevice()->createEmptyTexture2D(true);

		m_texGrabber->Set(vdo,tex);

	}
	video::ICameraVideoGrabber* cam=(video::ICameraVideoGrabber*)m_texGrabber->GetGrabber().pointer();

	cam->InitDevice(m_cameraSource.id,m_cameraSource.width,m_cameraSource.height,m_cameraSource.fps);
	CameraPoolManager::Instance.UseCamera(m_cameraSource.id);

	return true;
}
bool CameraTextureSource::_Unload()
{
	if(m_texGrabber->GetGrabber())
	{
		CameraPoolManager::Instance.UnseCamera(m_cameraSource.id);
	}
	return true;
}

void CameraTextureSource::SetCameraInfo(int id,int w,int h,int fps)
{
	m_cameraSource.id=id;
	m_cameraSource.width=w;
	m_cameraSource.height=h;
	m_cameraSource.fps=fps;
}


void CameraTextureSource::Update(float dt)
{
	m_texGrabber->Blit();
}

ITextureCRef CameraTextureSource::GetTexture()
{
	return m_texGrabber->GetTexture();
}
xml::XMLElement* CameraTextureSource::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;

	attr=e->getAttribute("ID");
	if(attr)
		m_cameraSource.id=core::StringConverter::toInt(attr->value);

	attr=e->getAttribute("Width");
	if(attr)
	{
		m_cameraSource.width=core::StringConverter::toInt(attr->value);
	}
	attr=e->getAttribute("Height");
	if(attr)
	{
		m_cameraSource.height=core::StringConverter::toInt(attr->value);
	}
	attr=e->getAttribute("FPS");
	if(attr)
	{
		m_cameraSource.fps=core::StringConverter::toInt(attr->value);
	}
	return e;
}


}
}