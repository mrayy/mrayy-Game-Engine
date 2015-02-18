


/********************************************************************
	created:	2012/10/29
	created:	29:10:2012   18:37
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\VTAppGlobals.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	VTAppGlobals
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VTAppGlobals___
#define ___VTAppGlobals___


#include "AppData.h"


namespace mray
{
	namespace game
	{
		class OculusCameraComponent;
	}
	namespace video
	{
		class OculusDevice;
	}
namespace VT
{
	class Application;

	enum ECameraSourceType
	{
		EWebCamera,
		EFlyCapture
	};
class VTAppGlobals:public TBee::AppData
{
protected:

	script::CSettingsFile s_values;
public:

	Application* App;
	bool Physics;
	core::string GhostServerIP;
	int Camera0;
	int Camera1;

	video::OculusDevice* oculusDevice;
	game::OculusCameraComponent* oculusComponents[2];

	bool FlipCameras;
	ECameraSourceType CameraType;

	VTAppGlobals()
	{
		App = 0;
		Physics = false;
		Camera0 = Camera1 = 0;
		oculusComponents[0] = oculusComponents[1] = 0;
		oculusDevice = 0;
		FlipCameras = false;
		CameraType = EWebCamera;
	}

	void SetValue(const core::string&catagory, const core::string&name, const core::string& v);
	core::string GetValue(const core::string&catagory, const core::string&name);


	virtual void Load(const core::string& path);
	virtual void Save(const core::string& path);


	static VTAppGlobals* Instance(){ return (VTAppGlobals*)s_instance; }
};

}
}

#endif
