
#include "ImageIntroItem.h"
#include "IEaglesManager.h"


namespace mray{
namespace gameMod{

ImageIntroItem::ImageIntroItem(GCPtr<video::ITexture> t,float fadeTime,float freezeTime){
	m_image=t;
	m_mode=0;
	m_time=0;
	m_fadeTime=fadeTime;
	m_appearTime=freezeTime;
}
ImageIntroItem::~ImageIntroItem(){

}

void ImageIntroItem::setRect(const math::rectf&rc){
	m_viewArea=rc;

}

void ImageIntroItem::setImage(GCPtr<video::ITexture> v){
	m_image=v;
}
GCPtr<video::ITexture> ImageIntroItem::getImage(){
	return m_image;
}

void ImageIntroItem::draw(){
	if(!m_image)
		return;

	GCPtr<video::IVideoDevice> device=eagles::IEaglesManager::getInstance().getVideoDevice();
	float f=0;
	switch (m_mode)
	{
	case 0:
		f=m_time/m_fadeTime;
		break;
	case 1:
		f=1;
		break;
	case 2:
		f=1-(m_time/m_fadeTime);
		break;
	}
	video::SColor clr=255*f;
	clr.A=255;
	math::rectf r;
	r.ULPoint=m_viewArea.ULPoint*device->getViewportRect().ULPoint;
	r.BRPoint=m_viewArea.BRPoint*device->getViewportRect().BRPoint;
	device->draw2DImage(r,m_image,clr);

}
void ImageIntroItem::update(float dt){

	m_time+=dt;
	switch (m_mode)
	{
	case 0:
		if(m_time>m_fadeTime){
			m_time-=m_fadeTime;
			m_mode=1;
		}
		break;
	case 1:
		if(m_time>m_appearTime){
			m_time-=m_appearTime;
			m_mode=2;
		}
		break;
	case 2:
		if(m_time>m_fadeTime){
			m_time-=m_fadeTime;
			m_mode=3;
		}
		break;
	}
}

bool ImageIntroItem::isDone(){
	return m_mode==3;
}

void ImageIntroItem::sendSkip(){
	if(m_mode!=2){
		m_mode=2;
		m_time=0;
	}
}

}
}
