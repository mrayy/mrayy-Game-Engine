
#include "GUIEffector.h"


namespace mray{
namespace GUI{



GUIEffector::GUIEffector():
	m_enabled(1),
	Enabled(this,&GUIEffector::setEnable,&GUIEffector::isEnabled,mT("Enabled"))
{
	fillProperties();
}
GUIEffector::~GUIEffector(){
}

void GUIEffector::fillProperties(){
	addPropertie(&Enabled);
}

void  GUIEffector::loadXMLSettings(xml::XMLElement* elem){
	IObject::loadXMLSettings(elem);
}
xml::XMLElement*  GUIEffector::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement *e=IObject::exportXMLSettings(elem);
	return e;
}

}
}

