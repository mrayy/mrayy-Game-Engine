
#include "GUIEffectorFactory.h"


namespace mray{
namespace GUI{


GUIEffectorFactory::GUIEffectorFactory(){
}
GUIEffectorFactory::~GUIEffectorFactory(){
}

void GUIEffectorFactory::addEffectorCreator(GCPtr<IGUIEffectorCreator> c){
	core::string s=c->getType();
	s.make_lower();
	m_creators[s]=c;
}
void GUIEffectorFactory::removeEffectorCreator(const core::string&type){
	m_creators.erase(type);
}

GCPtr<GUI::GUIEffector> GUIEffectorFactory::createEffector(const core::string&type){
	core::string s=type;
	s.make_lower();
	EffectorCreators::iterator it= m_creators.find(s);
	if(it==m_creators.end())
		return 0;
	return it->second->createEffector();
}


}
}