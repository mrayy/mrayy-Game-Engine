
#include "GUIPercentSizeEffector.h"
#include "GUIElement.h"
#include "IVideoDevice.h"


namespace mray{
namespace GUI{


GUIPercentSizeEffector::GUIPercentSizeEffector(const math::vector2d& percent,const math::vector2di&constrain):
	SizePercent(this,&GUIPercentSizeEffector::setSizePercent,&GUIPercentSizeEffector::getSizePercent,EPT_VECTOR2Df,mT("SizePercent")),
	Constrain(this,&GUIPercentSizeEffector::setConstrain,&GUIPercentSizeEffector::getConstrain,EPT_VECTOR2Df,mT("Constrain"))
{
	fillProperties();
	m_constrain=constrain;
	m_sizePercent=percent;

}
void GUIPercentSizeEffector::fillProperties(){
	addPropertie(&SizePercent);
	addPropertie(&Constrain);
}

void GUIPercentSizeEffector::effect(GUIElement* elem,float dt){

	if(!elem || !isEnabled())return;
	math::vector2d sz;
	if(elem->getParent())
	{
		sz=elem->getParent()->getSize();
	}else if(elem->getDevice()){
		sz=elem->getDevice()->getViewportRect().getSize();
	}else
		return;
	if(m_constrain.x)
		elem->MainSize=math::vector2d(sz.x*m_sizePercent.x,elem->MainSize().y);
	if(m_constrain.y)
		elem->MainSize=math::vector2d(elem->MainSize().x,sz.y*m_sizePercent.y);
}



}
}
