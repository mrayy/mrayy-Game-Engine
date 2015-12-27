

#include "stdafx.h"
#include "AppData.h"

#include "XMLTree.h"
#include "XMLWriter.h"
#include "CameraConfigurationManager.h"
#include "OptiTrackDataSource.h"

namespace mray
{
namespace TBee
{
	AppData* AppData::s_instance = 0;
AppData::AppData()
{
	s_instance = this;
	MajorVer = 1;
	MinorVer = 0;
//	oculusDevice = 0;
	headController = EHeadControllerType::Oculus;
	robotController = ERobotControllerType::Joystick;
	IsDebugging=0;
	inputMngr=0;
	robotInfoManager = 0;
	stereoMode = ERenderStereoMode::None;
	camConfig = new CameraConfigurationManager();
	optiDataSource = new TBee::OptiTrackDataSource();
	netValueController = new NetworkValueController();
}
AppData::~AppData()
{
	SaveNetValues();
	delete camConfig;
	delete netValueController;
}

void AppData::Init()
{
	camConfig->LoadConfigurations("CameraConfigurations.xml");
}

core::string AppData::GetVersion()
{
	return core::StringConverter::toString(MajorVer) + "." + core::StringConverter::toString(MinorVer);
}
core::string AppData::GetBuild()
{
	return core::string(__DATE__) + " " + core::string(__TIME__);
}
void AppData::SetValue(const core::string&catagory, const core::string&name, const core::string& v)
{
	s_values.setPropertie(catagory, name, v);
}

core::string AppData::GetValue(const core::string&catagory, const core::string&name)
{
	return s_values.getPropertie(catagory, name);
}
void AppData::Load(const core::string& path)
{
	OS::IStreamPtr stream = gFileSystem.openFile(path, OS::BIN_READ);
	if (!stream)
		return;
	s_values.loadSettings(stream);
	stream->close();

}
void AppData::Save(const core::string& path)
{
	OS::IStreamPtr stream = gFileSystem.openFile(path, OS::BIN_WRITE);
	if (!stream)
		return;
	s_values.writeSettings(stream);
	stream->close();
}

void AppData::SaveNetValues()
{
	core::string name = "NetValues.xml";
	xml::XMLElement root("root");
	xml::XMLElement* e = netValueController->GetValues()->exportXMLSettings(&root);
	xml::XMLWriter w;
	w.addElement(e);
	OS::IStreamPtr stream = gFileSystem.openFile(name, OS::TXT_WRITE);
	OS::StreamWriter sw(stream);

	sw.writeString(w.flush());
	stream->close();
}
void AppData::LoadNetValues()
{
	core::string name = "NetValues.xml";
	xml::XMLTree t;
	if (!t.load(name))
		return;

	netValueController->GetValues()->loadXMLSettings((xml::XMLElement*)*t.getElementsBegin());
}


}
}



