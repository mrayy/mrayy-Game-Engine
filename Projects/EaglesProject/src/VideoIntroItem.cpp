
#include "VideoIntroItem.h"
#include "IEaglesManager.h"

namespace mray{
namespace gameMod{

VideoIntroItem::VideoIntroItem(GCPtr<video::IVideoTexture> v){
	m_video=v;
	m_toExit=false;
	m_fadeOut=1;
}
VideoIntroItem::~VideoIntroItem(){
	m_video=0;
}

void VideoIntroItem::setRect(const math::rectf&rc){
	m_viewArea=rc;
}
void VideoIntroItem::setVideo(GCPtr<video::IVideoTexture> v){
	m_video=v;

}
GCPtr<video::IVideoTexture>  VideoIntroItem::getVideo(){
	return m_video;
}

void VideoIntroItem::reset(){
	if(m_video){
		m_video->setFrame(0);
	}
}

void VideoIntroItem::draw(){
	if(!m_video)
		return;
	GCPtr<video::ITexture> tex= m_video->getTexture();

	GCPtr<video::IVideoDevice> device=eagles::IEaglesManager::getInstance().getVideoDevice();
	video::SColor clr=255*m_fadeOut;
	clr.A=255;
	math::rectf r;
	r.ULPoint=m_viewArea.ULPoint*device->getViewportRect().ULPoint;
	r.BRPoint=m_viewArea.BRPoint*device->getViewportRect().BRPoint;
	device->draw2DImage(r,tex,clr);
}
void VideoIntroItem::update(float dt){
	if(m_video)
		m_video->update(dt);
	if(m_toExit){
		m_fadeOut-=dt;
		m_fadeOut=math::Max<float>(m_fadeOut,0);
	}
}

bool VideoIntroItem::isDone(){
	if(!m_video)
		return true;
	if(m_video->isDone())
		return true;

	if(m_toExit && m_fadeOut==0){
		return true;
	}
	return false;
}

void VideoIntroItem::sendSkip(){
	m_toExit=true;
}

}
}