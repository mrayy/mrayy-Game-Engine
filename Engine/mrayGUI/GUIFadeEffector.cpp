

#include "GUIFadeEffector.h"
#include "GUIElement.h"


namespace mray{
namespace GUI{


GUIFadeEffector::GUIFadeEffector(bool fadeOut,bool fadeIn,float speed,float idleTime,int minAlpha,int maxAlpha):
	Speed(this,&GUIFadeEffector::setSpeed,&GUIFadeEffector::getSpeed,EPT_FLOAT,mT("Speed")),
	IdleTime(this,&GUIFadeEffector::setIdleTime,&GUIFadeEffector::getIdleTime,EPT_FLOAT,mT("IdleTime")),
	FadeIn(this,&GUIFadeEffector::setFadeIn,&GUIFadeEffector::getFadeIn,EPT_BOOLEAN,mT("FadeIn")),
	FadeOut(this,&GUIFadeEffector::setFadeOut,&GUIFadeEffector::getFadeOut,EPT_BOOLEAN,mT("FadeOut")),
	MinAlpha(this,&GUIFadeEffector::setMinAlpha,&GUIFadeEffector::getMinAlpha,EPT_INT,mT("MinAlpha")),
	MaxAlpha(this,&GUIFadeEffector::setMaxAlpha,&GUIFadeEffector::getMaxAlpha,EPT_INT,mT("MaxAlpha"))

{
	m_fadeIn=fadeIn;
	m_fadeOut=fadeOut;
	m_speed=speed;
	m_idleTime=idleTime;
	m_minAlpha=minAlpha;
	m_maxAlpha=maxAlpha;
	m_startTime=0;
	m_stage=0;
	m_clrA=minAlpha;
	fillProperties();
	
}
void GUIFadeEffector::fillProperties(){
	addPropertie(&Speed);
	addPropertie(&IdleTime);
	addPropertie(&FadeIn);
	addPropertie(&FadeOut);
	addPropertie(&MinAlpha);
	addPropertie(&MaxAlpha);
}


bool GUIFadeEffector::isDone(){
	return (m_stage==3);
}
void GUIFadeEffector::reset(){
	m_stage=0;
	m_clrA=m_minAlpha;
	m_startTime=0;
}


void GUIFadeEffector::setParameters(bool fadeOut,bool fadeIn,float speed,float idleTime,int minAlpha,int maxAlpha){
	m_fadeIn=fadeIn;
	m_fadeOut=fadeOut;
	m_speed=speed;
	m_idleTime=idleTime;

}
void GUIFadeEffector::setFadeTime(const float& time){
	m_speed = (float)(m_maxAlpha-m_minAlpha)/time;
}


void GUIFadeEffector::setMinMaxAlpha(int minAlpha,int maxAlpha){
	m_minAlpha=minAlpha;
	m_maxAlpha=maxAlpha;
}


void GUIFadeEffector::setStage(int s){
	m_stage=s;
	m_stage=math::clamp<int>(s,0,3);
	if(s==0)
		m_clrA=m_minAlpha;
	else
		m_clrA=m_maxAlpha;
}
void GUIFadeEffector::effect(GUIElement* elem,float dt){
	if(!elem || !isEnabled())return;
	video::SColor clr=elem->Color;
	switch(m_stage){
	case 0:	
	if(m_fadeOut){
		if(m_clrA<m_maxAlpha){
			m_clrA += m_speed*dt;
			if(m_clrA>m_maxAlpha){
				m_clrA=m_maxAlpha;
				m_stage=1;
				break;
			}
			clr.A=m_clrA;
			break;
		}else
			m_stage =1;
	}else{
		clr.A=m_maxAlpha;
		m_stage=1;
		m_startTime=0;
	}
	case 1:
		if(m_startTime<m_idleTime)
			m_startTime+=dt;
		else
			m_stage=2;

		break;
	case 2:
		if(m_fadeIn){
			if(m_clrA>m_minAlpha){
				m_clrA -= m_speed*dt;
				if(m_clrA<m_minAlpha){
					m_clrA=m_minAlpha;
					m_stage=3;
					break;
				}
				clr.A=m_clrA;
				break;
			}else
				m_stage =3;
		}else
			m_stage=3;

		break;
	}
	elem->Color=clr;
	if(elem->getFontAttributes()){
		clr=elem->getFontAttributes()->fontColor;
		clr.A=m_clrA;
		elem->getFontAttributes()->fontColor=clr;
	}
}


}
}
