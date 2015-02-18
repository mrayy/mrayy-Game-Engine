
#include "stdafx.h"
#include "GUIElementFactory.h"


namespace mray{
namespace GUI{


GUIElementFactory::GUIElementFactory(){
}
GUIElementFactory::~GUIElementFactory(){
}

void GUIElementFactory::RegisterFactory(const IGUIElementFactory* c)
{
	core::string s=c->GetElementType();
	s.make_lower();
	m_creators[s]=c;
}
void GUIElementFactory::RemoveElementCreator(const core::string&type){
	m_creators.erase(type);
}

IGUIElement* GUIElementFactory::CreateElement(const core::string&type,IGUIManager* creator){
	core::string s=type;
	s.make_lower();
	ElementCreators::iterator it= m_creators.find(s);
	if(it==m_creators.end())
		return 0;
	return it->second->CreateElement(creator);
}


}
}

