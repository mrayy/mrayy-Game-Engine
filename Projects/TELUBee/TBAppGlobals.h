


/********************************************************************
	created:	2012/10/29
	created:	29:10:2012   18:37
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\TBAppGlobals.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	TBAppGlobals
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___TBAppGlobals___
#define ___TBAppGlobals___


#include "AppData.h"

namespace mray
{
	class InputManager;
	namespace db
	{
		class ISQLManager;
	}
	namespace video
	{
		class OculusDevice;
	}
namespace TBee
{

	class Application;
	class MapObject;
	class RobotInfoManager;
	class TBRobotInfo;
	class SeeThroughWindow;


	enum class EController
	{
		XBox,
		Logicool
	};


class TBAppGlobals:public AppData
{
protected:

public:
	TBAppGlobals()
	{
		seeTrough = 0;
		App = 0;
		mapObj = 0;
		selectedRobot = 0;
		sqlManager = 0;

	}

	SeeThroughWindow* seeTrough;
	Application* App;
	core::string ServerIP;

	EController m_controller;

	math::vector2d GetStereoScaleRatio();

	core::string DVIPort;

	MapObject* mapObj;

	TBRobotInfo* selectedRobot;

	core::string userName;
	core::string userPassword;

	db::ISQLManager* sqlManager;

	
	void Load(const core::string& path);
	void Save(const core::string& path);

	static TBAppGlobals* Instance(){ return (TBAppGlobals*)s_instance; }
};

}
}

#endif
