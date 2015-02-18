
#include "stdafx.h"
#include "GUISoundTrack.h"



namespace mray{
namespace GUI{

GUISoundTrack::GUISoundTrack(GUIManager*mngr,float maxCooldown,math::vector2d pos,math::vector2d size)
	:GUIElement(mngr,GUI::GUIE_NONE,0,pos,size,0,0)
{

	m_maxCooldown=maxCooldown;
	m_cooldown=0;
	m_show=0;
	setVisible(0);

	m_BackGnd=new video::TextureUnit();
	m_BackGnd->texture=gTextureResourceManager.loadTexture2D(mT("soundTrackUI.png"));
}

GUISoundTrack::~GUISoundTrack(){
}

void GUISoundTrack::show(const core::string& soundName,const core::string& themeName){
	m_cooldown=m_maxCooldown;
	m_soundName=mT("Track: ");
	m_soundName+=soundName;

	m_themeName=mT("Theme: ");
	m_themeName+=themeName;
	m_lastAlpha=0;
	setVisible(1);


	if(getParent())
		getParent()->bringToFront(this);
	m_show=1;
}



void GUISoundTrack::draw(float dt){
	if(!device || !Visible())return;
	if(m_show){
		if(m_lastAlpha<1){
			m_lastAlpha+=1*dt;
			if(m_lastAlpha>1)
				m_lastAlpha=1;
		}else
			m_cooldown-=dt;
		if(m_cooldown<=0){
			m_show=0;
		}
	}else{
		m_lastAlpha-=1*dt;
		if(m_lastAlpha<=0){
			m_lastAlpha=0;
			setVisible(0);
		}
	}

	math::rectf rc=getRect();

	video::SColor clr=Color();
	clr.A*=m_lastAlpha;

	device->draw2DImage(rc,m_BackGnd,clr);

	clr=255;
	clr.A*=m_lastAlpha;

	mray::GUI::IFont*font=gFontResourceManager.getDefaultFont();
	if(font){
		math::rectf trc;
		trc.ULPoint.x =rc.ULPoint.x + rc.getWidth()*0.44;
		trc.ULPoint.y =rc.ULPoint.y + rc.getHeight()*0.52;
		trc.BRPoint = trc.ULPoint + math::vector2d(rc.getWidth()*0.47,rc.getHeight()*0.14);

		FontAttributes attrs;
		attrs.fontSize=14;
		attrs.fontColor=clr;

		font->print(trc,&attrs,&trc,m_soundName);

		trc.ULPoint.x =rc.ULPoint.x + rc.getWidth()*0.44;
		trc.ULPoint.y =rc.ULPoint.y + rc.getHeight()*0.6667;
		trc.BRPoint = trc.ULPoint + math::vector2d(rc.getWidth()*0.47,rc.getHeight()*0.14);

		font->print(trc,&attrs,&trc,m_themeName);
	}
	GUIElement::draw(dt);

}


}
}




