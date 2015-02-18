
#include "GUIElementFactory.h"


namespace mray{
namespace GUI{


GUIElementFactory::GUIElementFactory(){
}
GUIElementFactory::~GUIElementFactory(){
}

void GUIElementFactory::addElementCreator(GCPtr<IGUIElementCreator> c){
	core::string s=c->getType();
	s.make_lower();
	m_creators[s]=c;
}
void GUIElementFactory::removeElementCreator(const core::string&type){
	m_creators.erase(type);
}

GCPtr<GUI::GUIElement> GUIElementFactory::createElement(const core::string&type){
	core::string s=type;
	s.make_lower();
	ElementCreators::iterator it= m_creators.find(s);
	if(it==m_creators.end())
		return 0;
	return it->second->createElement();
}


}
}

