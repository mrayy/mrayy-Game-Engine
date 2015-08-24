
#include "stdafx.h"
#include "CameraProfile.h"
#include "XMLTree.h"





namespace mray
{


bool CameraProfile::GetValue(const core::string& param, core::string &v)
{
	std::map<core::string, ParamPair>::iterator it= m_params.find(param);
	if (it == m_params.end())
		return false;
	v= it->second.value;
	return true;
}


void CameraProfile::LoadFromXML(xml::XMLElement* elem)
{
	m_name= elem->getValueString("Name");
	xml::XMLElement* e = elem->getSubElement("Param");
	while (e)
	{
		ParamPair p;
		p.name = e->getValueString("Name");
		p.value = e->getValueString("Value");
		m_params[p.name] = p;
		e = e->nextSiblingElement("Param");
	}
}



CameraProfileManager::CameraProfileManager()
{

}
CameraProfileManager::~CameraProfileManager()
{
	std::map<core::string, CameraProfile*>::iterator it= m_cameras.begin();
	for (; it != m_cameras.end();++it)
	{
		delete it->second;
	}
	m_cameras.clear();

}

CameraProfile* CameraProfileManager::GetProfile(const core::string& name)
{
	std::map<core::string, CameraProfile*>::iterator it = m_cameras.find(name);
	if (it == m_cameras.end())
		return 0;
	return it->second;

}

void CameraProfileManager::LoadFromXML(const core::string& path)
{
	xml::XMLTree tree;
	if (!tree.load(path))
		return;

	xml::XMLElement* e = tree.getSubElement("Cameras");
	if (!e)
		return;
	e = e->getSubElement("Camera");
	while (e)
	{
		CameraProfile* p = new CameraProfile();
		p->LoadFromXML(e);
		m_cameras[p->GetName()] = p;
		e = e->nextSiblingElement("Camera");
	}
}

}
