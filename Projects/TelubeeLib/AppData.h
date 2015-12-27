/********************************************************************
	created:	2014/01/08
	created:	8:1:2014   20:24
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\AppData.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	AppData
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AppData__
#define __AppData__

#include "StereoRenderer.h"
#include "TBeeCommon.h"
#include "NetworkValueController.h"

#define USE_OPTITRACK 1


namespace mray
{
	class InputManager;
	namespace video
	{
	//	class OculusDevice;
	}
namespace TBee
{
	class RobotInfoManager;
	class CameraConfigurationManager;
	class OptiTrackDataSource;

class AppData
{
protected:
	script::CSettingsFile s_values;
	static AppData* s_instance;
public:
	AppData();
	virtual~AppData();
	void Init();
	bool IsDebugging;

	int MajorVer;
	int MinorVer;
	ERenderStereoMode stereoMode;

	EHeadControllerType headController;
	ERobotControllerType robotController;

	//video::OculusDevice* oculusDevice;
	InputManager* inputMngr;
	RobotInfoManager* robotInfoManager;

	CameraConfigurationManager* camConfig;

	TBee::OptiTrackDataSource* optiDataSource;

	NetworkValueController* netValueController;

	core::string GetVersion();
	core::string GetBuild();

	void SaveNetValues();
	void LoadNetValues();

	void SetValue(const core::string&catagory, const core::string&name, const core::string& v);
	core::string GetValue(const core::string&catagory, const core::string&name);

	virtual void Load(const core::string& path);
	virtual void Save(const core::string& path);

	static AppData* Instance(){ return s_instance; }
};

}
}


#endif