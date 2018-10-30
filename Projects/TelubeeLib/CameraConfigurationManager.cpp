

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
	cameraType = ECameraType::POVCamera;
	captureType = ECameraCaptureType::CaptureRaw;
	streamType = EStreamCodec::StreamCoded;
	FlipX = FlipY = false;
	separateStreams = false;
	CameraStreams = 1;
}

void TelubeeCameraConfiguration::LoadFromXML(xml::XMLElement*e)
{
	xml::XMLAttribute* attr;
	name = e->getValueString("Name");
	fov = e->getValueFloat("FOV");
	cameraOffset = e->getValueFloat("CameraOffset");
	stereoOffset = e->getValueFloat("StereoOffset");
	if (e->getValueBool("FlipX"))
		FlipX = e->getValueBool("FlipX");
	if (e->getValueBool("FlipY"))
		FlipY = e->getValueBool("FlipY");
	attr = e->getAttribute("OpticalCenter");
	if (attr)
		OpticalCenter = core::StringConverter::toVector2d(attr->value);
	attr = e->getAttribute("FocalCoeff");
	if (attr)
		FocalCoeff = core::StringConverter::toVector2d(attr->value);
	attr = e->getAttribute("KPCoeff");
	if (attr)
		KPCoeff = core::StringConverter::toVector4d(attr->value);

	attr = e->getAttribute("PixelShift");
	if (attr)
		PixelShift = core::StringConverter::toVector4d(attr->value);
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

	cameraType = ECameraType::POVCamera;
	captureType = ECameraCaptureType::CaptureRaw;

	attr = e->getAttribute("Type");
	if (attr)
	{
		if (attr->value == "POV")
			cameraType = ECameraType::POVCamera;
		else if (attr->value == "OMNI")
			cameraType = ECameraType::OmniCamera;
	}
	attr = e->getAttribute("CaptureMedia");
	if (attr)
	{
		if (attr->value == "RAW")
			captureType = ECameraCaptureType::CaptureRaw;
		if (attr->value == "CAM")
			captureType = ECameraCaptureType::CaptureCam;
		else if (attr->value == "H264")
			captureType = ECameraCaptureType::CaptureH264;
		else if (attr->value == "JPEG")
			captureType = ECameraCaptureType::CaptureJpeg;
	}

	xml::XMLElement* elem = e->getSubElement("rect");
	while (elem)
	{
		attr=elem->getAttribute("value");
		if (attr)
		{
			math::vector4d v = core::StringConverter::toVector4d(attr->value);
			customRects.push_back(v);
		}
		elem = elem->nextSiblingElement("rect");
	}
}


xml::XMLElement* TelubeeCameraConfiguration::ExportToXML(xml::XMLElement*elem)
{
	xml::XMLElement* e=new xml::XMLElement("CameraConfiguration");
	elem->addSubElement(e);

	e->addAttribute("Name", name);
	e->addAttribute("EncoderType", encoderType);
	e->addAttribute("FOV", core::StringConverter::toString(fov));
	e->addAttribute("CameraOffset", core::StringConverter::toString(cameraOffset));
	e->addAttribute("StereoOffset", core::StringConverter::toString(stereoOffset));
	e->addAttribute("OpticalCenter", core::StringConverter::toString(OpticalCenter));
	e->addAttribute("FocalCoeff", core::StringConverter::toString(FocalCoeff));
	e->addAttribute("KPCoeff", core::StringConverter::toString(KPCoeff));
	e->addAttribute("PixelShift", core::StringConverter::toString(PixelShift));
	e->addAttribute("FlipX", FlipX ? "true" : "false");
	e->addAttribute("FlipY", FlipY ? "true" : "false");
	e->addAttribute("SeparateStreams", separateStreams ? "true" : "false");
	e->addAttribute("CameraStreams", core::StringConverter::toString(CameraStreams));


	for (int i = 0; i < 2; ++i)
	{

		core::string v = "None";
		if (cameraRotation[i]==CW)
			v = "CW";
		if (cameraRotation[i] == CCW)
			v = "CCW";
		if (cameraRotation[i] == Flipped)
			v = "Flipped";
		if (i == 0)
			e->addAttribute("LeftRotation", v);
		else
			e->addAttribute("RightRotation", v);
	}

	for (int i = 0; i < customRects.size(); ++i)
	{
		xml::XMLElement* r = new xml::XMLElement("rect");
		r->addAttribute("value", core::StringConverter::toString(customRects[i]));
		e->addSubElement(r);
	}

	switch (cameraType)
	{
	case mray::TBee::TelubeeCameraConfiguration::POVCamera:
		e->addAttribute("Type", "POV");
		break;
	case mray::TBee::TelubeeCameraConfiguration::OmniCamera:
		e->addAttribute("Type", "OMNI");
		break;
	default:
		break;
	}
	switch (streamType)
	{
	case mray::TBee::TelubeeCameraConfiguration::StreamRaw:
		e->addAttribute("StreamCodec", "Raw");
		break;
	case mray::TBee::TelubeeCameraConfiguration::StreamCoded:
		e->addAttribute("StreamCodec", "Coded");
		break;
	case mray::TBee::TelubeeCameraConfiguration::StreamOvrvision:
		e->addAttribute("StreamCodec", "Ovrvision");
		break;
	case mray::TBee::TelubeeCameraConfiguration::StreamFoveatedOvrvision:
		e->addAttribute("StreamCodec", "FoveatedOVR");
		break;
	case mray::TBee::TelubeeCameraConfiguration::StreamEyegazeRaw:
		e->addAttribute("StreamCodec", "EyegazeRaw");
		break;
	default:
		break;
	}
	return e;
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

