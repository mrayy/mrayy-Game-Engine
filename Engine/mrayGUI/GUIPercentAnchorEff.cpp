
#include "GUIPercentAnchorEff.h"
#include "rect.h"
#include "GUIElement.h"
#include "IVideoDevice.h"

namespace mray{
namespace GUI{


GUIPercentAnchorEff::GUIPercentAnchorEff(EPosAnchor anch,const math::vector2d &posPercent)
	:GUIAnchorEffector(anch,0),m_posPercent(posPercent),
	PosPercent(this,&GUIPercentAnchorEff::setPosPercent,&GUIPercentAnchorEff::getPosPercent,EPT_VECTOR2Df,mT("PosPercent"))
{
	fillProperties();
}
GUIPercentAnchorEff::~GUIPercentAnchorEff(){
}

void GUIPercentAnchorEff::fillProperties(){
	addPropertie(&PosPercent);
}

void GUIPercentAnchorEff::effect(GUIElement* elem,float dt){
	if(!elem)
		return;

	math::rectf rc;
	if(elem->getParent())
	{
		rc=elem->getParent()->getRect();
	}else if(elem->getDevice()){
		rc=elem->getDevice()->getViewportRect();
	}else
		rc=elem->getRect();

	m_offset=rc.getSize()*m_posPercent;
	GUIAnchorEffector::effect(elem,dt);
}

}
}