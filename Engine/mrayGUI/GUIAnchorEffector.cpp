

#include "GUIAnchorEffector.h"
#include "GUIElement.h"
#include "GUIDummy.h"
#include "GUIManager.h"
#include "IVideoDevice.h"


namespace mray{
namespace GUI{


GUIAnchorEffector::GUIAnchorEffector(EPosAnchor anch,const math::vector2d& offset):
	m_anchor(anch),
	Anchor(this,&GUIAnchorEffector::setAnchor,&GUIAnchorEffector::getAnchor,mT("EPosAnchor"),mT("Anchor")),
	Offset(this,&GUIAnchorEffector::setOffset,&GUIAnchorEffector::getOffset,EPT_VECTOR2Df,mT("Offset"))
{
	m_offset=offset;
	fillProperties();
}

void GUIAnchorEffector::fillProperties(){
	addPropertie(&Anchor);
	addPropertie(&Offset);
}

void GUIAnchorEffector::effect(GUIElement* elem,float dt)
{
	if(!elem || !isEnabled())return;

	math::rectf rc;
	if(elem->getParent())
	{
		rc=elem->getParent()->getRect();
		rc.BRPoint-=rc.ULPoint;
		rc.ULPoint=0;
	}else if(elem->getDevice()){
		rc=elem->getDevice()->getViewportRect();
	}else
		rc=elem->getRect();
	bool f=0;
	math::vector2d p;
	if(m_anchor==EPAnchor_CENTER){
		p=(math::vector2d(rc.getSize()) - elem->getSize())*0.5f + m_offset;
	}else{
		if(m_anchor==EPAnchor_UR || m_anchor==EPAnchor_BR || m_anchor==EPAnchor_R ){
			p.x=rc.getSize().x - elem->getSize().x +m_offset.x;
		}else if(m_anchor==EPAnchor_UL || m_anchor==EPAnchor_BL || m_anchor==EPAnchor_L){
				p.x=m_offset.x;
			}
		if(m_anchor==EPAnchor_UL || m_anchor==EPAnchor_UR || m_anchor==EPAnchor_U){
			p.y=m_offset.y;
		}else if(m_anchor==EPAnchor_BL || m_anchor==EPAnchor_BR || m_anchor==EPAnchor_B || m_anchor==EPAnchor_MidY){
				p.y=rc.getSize().y - elem->getSize().y - m_offset.y;
			}

		if(m_anchor==EPAnchor_U || m_anchor==EPAnchor_B)
			p.x=(rc.getSize().x - elem->getSize().x)*0.5 + m_offset.x;
		else if(m_anchor==EPAnchor_L || m_anchor==EPAnchor_R)
			p.y=(rc.getSize().y - elem->getSize().y)*0.5 - m_offset.y;

		
		if(m_anchor==EPAnchor_MidX){
			p.x=(rc.getSize().x - elem->getSize().x)*0.5f + m_offset.x;
			p.y=elem->getPos().y;
		}else 
		if(m_anchor==EPAnchor_MidY){
			p.x=elem->getPos().x;
			p.y=(rc.getSize().y - elem->getSize().y)*0.5f - m_offset.y;
		}
	}
	//if(!f)
		p+=rc.ULPoint;
	elem->MainPos=p;
}



}
}
