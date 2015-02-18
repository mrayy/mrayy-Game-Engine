
#include "GUITranslateEffector.h"
#include "GUIElement.h"

namespace mray{
namespace GUI{



GUITranslateEffector::GUITranslateEffector(math::vector2d startP,math::vector2d endP,float speed):
	StartPos(this,&GUITranslateEffector::setStartPos,&GUITranslateEffector::getStartPos,EPT_VECTOR2Df,mT("StartPos")),
	EndPos(this,&GUITranslateEffector::setEndPos,&GUITranslateEffector::getEndPos,EPT_VECTOR2Df,mT("EndPos")),
	Speed(this,&GUITranslateEffector::setSpeed,&GUITranslateEffector::getSpeed,EPT_FLOAT,mT("Speed")),
	AutoReverse(this,&GUITranslateEffector::setAutoReverse,&GUITranslateEffector::getAutoReverse,EPT_BOOLEAN,mT("AutoReverse"))
{
	m_startPos=startP;
	m_endPos=endP;
	m_t=0;
	m_speed=speed;
	m_dirSgn=1;
	m_autoReverse=1;

	fillProperties();
}
void GUITranslateEffector::fillProperties(){
	addPropertie(&StartPos);
	addPropertie(&EndPos);
	addPropertie(&Speed);
	addPropertie(&AutoReverse);
}


void GUITranslateEffector::effect(GUIElement* elem,float dt){
	if(!elem|| !isEnabled())return;
	m_t+=m_dirSgn*m_speed*0.01f*dt;
	if(m_t>=1){
		m_t=1;
		if(m_autoReverse)m_dirSgn*=-1;
	}
	else if(m_t<=0){
		m_t=0;
		if(m_autoReverse)m_dirSgn*=-1;
	}
	elem->MainPos=(math::lerp<math::vector2d>(m_startPos,m_endPos,m_t));
}
bool GUITranslateEffector::isDone(){
	return m_t==0 && m_dirSgn==-1 || m_t==1 && m_dirSgn==1;
}


}
}