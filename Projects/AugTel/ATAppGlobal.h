


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
#include "ISoundManager.h"
#include "IGUIManager.h"


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
	class OpenNIHandler;
	class ICameraVideoSource;
	class TBRobotInfo;
	class CRobotConnector;
}
using namespace TBee;
namespace AugTel
{

class Application;
class RobotInfoManager;
class HeadMount;
class DataCommunicator;

enum class EController
{
	XBox,
	Logicool
};
class ATAppGlobal :public AppData
{
protected:

public:
	ATAppGlobal()
	{
		App = 0;
		selectedRobot = 0;
		sqlManager = 0;
		guiManager = 0;
		dataCommunicator = 0;
		cameraProvider = 0;

		TargetVideoPort = 7000;
		TargetAudioPort = 7005;
		TargetHandsVideoPort = 7010;
		TargetCommunicationPort = 6000;

		RtcpStream = false;
	}
	Application* App;
	core::string ServerIP;

	//enable video/audio rtcp connection
	bool RtcpStream;
	uint TargetVideoPort;
	uint TargetAudioPort;
	uint TargetCommunicationPort;
	uint TargetHandsVideoPort;


	core::string DVIPort;
	EController m_controller;

	sound::ISoundManager* soundManager;
	TBRobotInfo* selectedRobot;

	core::string userName;
	core::string userPassword;

	db::ISQLManager* sqlManager;

	GUI::IGUIManager* guiManager;

	HeadMount* headObject;

	TBee::OpenNIHandler* depthProvider;

	TBee::ICameraVideoSource* cameraProvider;
	DataCommunicator* dataCommunicator;
	TBee::CRobotConnector* robotConnector;

	static ATAppGlobal* Instance(){ return (ATAppGlobal*)s_instance; }
};

#define gAppData (*AugTel::ATAppGlobal::Instance())

}
}

#endif
