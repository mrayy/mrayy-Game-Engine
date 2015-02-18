

#include "stdafx.h"
#include "VideoTextureSource.h"
#include "VideoResourceManager.h"


namespace mray
{
namespace video
{
	const core::string VideoTextureSource::FactorySourceType=mT("VideoTexture");

VideoTextureSource::VideoTextureSource()
{
	m_autoStart=false;
	m_loop=false;
}

VideoTextureSource::~VideoTextureSource()
{
}


bool VideoTextureSource::_Load()
{
	if(m_video.isNull())
	{
		m_video=VideoResourceManager::getInstance().LoadVideo(m_videoPath);
	}
	if(m_video.isNull())
		return false;
	bool ret= m_video->load();

	return ret;
}

bool VideoTextureSource::_Unload()
{
	if(m_video.isNull())
		return false;
	m_video->unload();
	return true;
}


void VideoTextureSource::Update(float dt)
{
	m_videoGrabber.Blit();
}


ITextureCRef VideoTextureSource::GetTexture()
{
	if(m_video.isNull())
		return ITexturePtr::Null;
	return m_videoGrabber.GetTexture();
}

xml::XMLElement* VideoTextureSource::LoadFromXML(xml::XMLElement* e)
{
	m_videoPath=e->getValueString("Path");
	m_loop=e->getValueBool("Loop");
	m_autoStart=e->getValueBool("AutoStart");

	xml::XMLAttribute* attr;
	attr=e->getAttribute("TargetResolution");

	if(!attr)
		m_targetResoulution=-1;
	else
	{
		m_targetResoulution=core::StringConverter::toVector2d(attr->value);
	}
	return e;
}

IMPLEMENT_TEXTURE_FACTORY(VideoTextureSource);

}
}

