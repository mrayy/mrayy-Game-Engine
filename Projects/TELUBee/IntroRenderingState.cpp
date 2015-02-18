

#include "stdafx.h"
#include "IntroRenderingState.h"

#include "Application.h"
#include "IVideoDevice.h"
#include "TBAppGlobals.h"
#include "TextureResourceManager.h"
#include "VideoResourceManager.h"
#include "GStreamVideoProvider.h"

namespace mray
{
namespace TBee
{

	void IntroItem::LoadXML(xml::XMLElement* e)
	{
		xml::XMLAttribute* attr;
		attr= e->getAttribute("CanSkip");
		if(attr)
			m_canSkip=core::StringConverter::toBool(attr->value);
	}

	void IntroItem::Update(float dt)
	{
		m_timer+=dt;
		if(!m_skipped)
		{
			if(m_timer<2)
			{
				m_fadeAmount=(m_timer)/2.0f;
			}else if(m_timer>GetLength()-2)
			{
				m_fadeAmount=1-(m_timer-(GetLength()-2))/2.0f;
			}else
				m_fadeAmount=1;
		}else
		{
			m_fadeAmount-=dt;
			if(m_fadeAmount<0)
				m_fadeAmount=0;
		}
	}
	void IntroItem::Start()
	{
		m_timer=0;
	}

	class ImageIntroItem:public IntroItem
	{
	protected:
		video::TextureUnit m_image;
		float m_length;
	public:
		ImageIntroItem()
		{
			m_length=0;
		}
		virtual void Start()
		{
			IntroItem::Start();
		}
		virtual bool IsDone()
		{
			return m_timer>=m_length || IntroItem::IsDone();
		}
		virtual void Render(const math::rectf& rc)
		{
			Engine::getInstance().getDevice()->useTexture(0,&m_image);
			Engine::getInstance().getDevice()->draw2DImage(rc, video::SColor(1, 1, 1, m_fadeAmount));
		}
		virtual void Update(float dt)
		{
			IntroItem::Update(dt);
		}
		float GetLength()
		{
			return m_length;
		}

		virtual void LoadXML(xml::XMLElement* e)
		{
			IntroItem::LoadXML(e);
			xml::XMLAttribute* attr;
			attr=e->getAttribute("Image");
			if(attr)
			{
				m_image.SetTexture(gTextureResourceManager.loadTexture2D(attr->value));
			}
			attr=e->getAttribute("Length");
			if(attr)
				m_length=core::StringConverter::toFloat(attr->value);
		}
	};

	class VideoIntroItem:public IntroItem
	{
	protected:
		core::string m_fileName;
		video::VideoGrabberTexture* m_video;
		video::ITexturePtr m_texture;
	public:
		VideoIntroItem()
		{
			m_video=new video::VideoGrabberTexture();
			m_texture = Engine::getInstance().getDevice()->createEmptyTexture2D(true);
		}
		~VideoIntroItem()
		{
			delete m_video;
		}
		virtual void Start()
		{
			IntroItem::Start();
			video::IVideoSource* v=(video::IVideoSource*)m_video->GetGrabber().pointer();

			if(v)
			{
				v->seek(0);
				v->play();
			}

		}
		virtual bool IsDone()
		{
			if(IntroItem::IsDone())
				return true;
			video::IVideoSource* v=(video::IVideoSource*)m_video->GetGrabber().pointer();

			if(v)
			{
				return v->isDone();
			}
			return true;
		}
		virtual void Render(const math::rectf& rc)
		{
			m_video->Blit();
			video::TextureUnit tex;
			tex.SetTexture(m_video->GetTexture());
			Engine::getInstance().getDevice()->useTexture(0, &tex);
			Engine::getInstance().getDevice()->draw2DImage(math::rectf(math::vector2d(0), rc.getSize()), video::SColor(1, 1, 1, m_fadeAmount));

		}
		virtual void Update(float dt)
		{
			IntroItem::Update(dt);
		}


		float GetLength()
		{
			video::IVideoSource* v=(video::IVideoSource*)m_video->GetGrabber().pointer();
			return v->getDuration();
		}

		void Skip()
		{
			IntroItem::Skip();
			/*
			video::IVideoSource* v=(video::IVideoSource*)m_video->GetGrabber().pointer();

			if(v)
			{
				v->stop();
			}*/
		}

		virtual void LoadXML(xml::XMLElement* e)
		{
			IntroItem::LoadXML(e);
			xml::XMLAttribute* attr;
			attr=e->getAttribute("Video");
			if(attr)
			{
				m_fileName=attr->value;
				//GCPtr<video::GStreamVideoProvider> vdo;
				video::IVideoSourcePtr vdo=gVideoResourceManager.LoadVideo(m_fileName);
				m_video->Set(vdo,m_texture);
			}
		}
	};


IntroRenderingState::IntroRenderingState()
:IRenderingState("Intro")
{
	m_currentItem=0;
}
IntroRenderingState::~IntroRenderingState()
{
	for (int i=0;i<m_introItems.size();++i)
	{
		delete m_introItems[i];
	}
	m_introItems.clear();
}

void IntroRenderingState::InitState()
{

	IRenderingState::InitState();

}

void IntroRenderingState::OnEvent(Event* e)
{
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* evt=(KeyboardEvent*)e;
		if(evt->press && evt->key==KEY_ESCAPE)
		{
			if(m_currentItem!=-1 && m_currentItem<=m_introItems.size())
				m_introItems[m_currentItem]->Skip();
		}
	}
}
void IntroRenderingState::OnEnter(IRenderingState*prev)
{
	m_currentItem=-1;

}
void IntroRenderingState::OnExit()
{
	IRenderingState::OnExit();
}
video::IRenderTarget* IntroRenderingState::Render(const math::rectf& rc,ETargetEye eye)
{
	IRenderingState::Render(rc,eye);
	video::IVideoDevice* dev=Engine::getInstance().getDevice();
	dev->setRenderTarget(m_renderTarget[GetEyeIndex(eye)]);
	dev->set2DMode();

	if(m_currentItem<m_introItems.size())
	{
		m_introItems[m_currentItem]->Render(rc);
	}
	dev->setRenderTarget(0);

	return m_renderTarget[GetEyeIndex(eye)].pointer();
}

void IntroRenderingState::Update(float dt)
{
	if(m_currentItem!=-1 && m_currentItem>=m_introItems.size())
	{
		m_exitCode=STATE_EXIT_CODE;
		return;
	}
	if(m_currentItem==-1 || m_introItems[m_currentItem]->IsDone())
	{
		++m_currentItem;

		if(m_currentItem<m_introItems.size())
		{
			m_introItems[m_currentItem]->Start();
		}
	}
	if(m_currentItem<m_introItems.size())
		m_introItems[m_currentItem]->Update(dt);
}
video::IRenderTarget* IntroRenderingState::GetLastFrame(ETargetEye eye)
{
	return m_renderTarget[GetEyeIndex(eye)].pointer();
}

void IntroRenderingState::LoadFromXML(xml::XMLElement* e)
{
	IRenderingState::LoadFromXML(e);

	xml::xmlSubElementsMapIT it=e->getElementsBegin();
	xml::xmlSubElementsMapIT end=e->getElementsEnd();
	for (;it!=end;++it)
	{
		xml::XMLElement* elem=dynamic_cast<xml::XMLElement*>(*it);
		if(!elem)
			continue;
		IntroItem *item=0;
		if(elem->getName().equals_ignore_case("Image"))
		{
			item=new ImageIntroItem();
		}else if(elem->getName().equals_ignore_case("Video"))
		{
			item=new VideoIntroItem();
		}
		if (item)
		{
			item->LoadXML(elem);
			m_introItems.push_back(item);
		}
	}
}


}
}

