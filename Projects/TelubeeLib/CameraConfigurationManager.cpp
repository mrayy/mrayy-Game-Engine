

#include "stdafx.h"
#include "CameraConfigurationManager.h"


#include "XMLTree.h"
#include "StringConverter.h"

namespace mray
{
namespace TBee
{
	
TelubeeCameraConfiguration::TelubeeCameraConfiguration() :fov(60), cameraOffset(0), stereoOffset(0.065), OpticalCenter(0.5), FocalCoeff(1), KPCoeff(0)
{
	cameraRotation[0] = cameraRotation[1] = None;
}

void TelubeeCameraConfiguration::LoadFromXML(xml::XMLElement*e)
{
	xml::XMLAttribute* attr;
	name = e->getValueString("Name");
	fov = e->getValueFloat("FOV");
	cameraOffset = e->getValueFloat("CameraOffset");
	stereoOffset = e->getValueFloat("StereoOffset");
	attr = e->getAttribute("OpticalCenter");
	if (attr)
		OpticalCenter = core::StringConverter::toVector2d(attr->value);
	attr = e->getAttribute("FocalCoeff");
	if (attr)
		FocalCoeff = core::StringConverter::toVector2d(attr->value);
	attr = e->getAttribute("KPCoeff");
	if (attr)
		KPCoeff = core::StringConverter::toVector4d(attr->value);

	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
			attr = e->getAttribute("LeftRotation");
		else
			attr = e->getAttribute("RightRotation");

		cameraRotation[i] = None;
		if (attr)
		{
			if (attr->value.equals_ignore_case("CW"))
				cameraRotation[i] = CW;
			else if (attr->value.equals_ignore_case("CCW"))
				cameraRotation[i] = CCW;
			else if (attr->value.equals_ignore_case("Flipped"))
				cameraRotation[i] = Flipped;
		}
	}
}

CameraConfigurationManager::CameraConfigurationManager()
{
}
CameraConfigurationManager::~CameraConfigurationManager()
{
	Clear();
}

void CameraConfigurationManager::Clear()
{
	CamMap::iterator it = m_cameras.begin();
	for (; it != m_cameras.end(); ++it)
	{
		delete it->second;
	}
	m_cameras.clear();
}

TelubeeCameraConfiguration* CameraConfigurationManager::GetCameraConfiguration(const core::string& name)
{
	CamMap::iterator it = m_cameras.find(name);
	if (it == m_cameras.end())
		return 0;
	return it->second;
}

void CameraConfigurationManager::LoadConfigurations(const core::string& path)
{
	xml::XMLTree tree;
	if (!tree.load(path))
		return;
	xml::XMLElement* e = tree.getSubElement("CameraConfigurations");
	if (!e)
		return;
	e = e->getSubElement("CameraConfiguration");
	while (e)
	{
		TelubeeCameraConfiguration* c = new TelubeeCameraConfiguration();
		c->LoadFromXML(e);
		m_cameras[c->name] = c;
		e = e->nextSiblingElement("CameraConfiguration");
	}
}

}
}

