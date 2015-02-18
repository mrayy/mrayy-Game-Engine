#include "stdafx.h"

#include "IGUITheme.h"

#include "GUIProfile.h"
#include "StringUtil.h"

namespace mray
{
namespace GUI
{

IGUITheme::IGUITheme(const core::string&name):m_name(name),m_material(0)
{

}
IGUITheme::~IGUITheme()
{
	ProfileList::iterator it=m_profiles.begin();
	for(;it!=m_profiles.end();++it)
		delete it->second;
	m_profiles.clear();
}

void IGUITheme::setMaterial(video::RenderMaterial* m){
	m_material=m;
}
video::RenderMaterial*IGUITheme::getMaterial(){
	return m_material;
}

FontAttributes* IGUITheme::getHintFontAttr(){
	return &m_hintFontAttr;
}


void IGUITheme::addProfile(GUIProfile*  p){
	core::string name=core::StringUtil::ToLower(p->getName());
	m_profiles.insert(ProfileList::value_type(name,p));
}
GUIProfile*  IGUITheme::getProfile(const core::string&name){
	core::string pName=core::StringUtil::ToLower(name);
	ProfileList::iterator it=m_profiles.find(pName);
	if(it==m_profiles.end())
		return 0;
	return it->second;
}

}
}

