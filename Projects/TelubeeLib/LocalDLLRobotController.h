

#ifndef __LOCALDLLROBOTCONTROLLER__
#define __LOCALDLLROBOTCONTROLLER__

#include "IRobotController.h"
#include "IDynamicLibrary.h"

namespace mray
{
namespace TBee
{
	
class LocalDLLRobotController :public IRobotController
{
protected:

	OS::IDynamicLibraryPtr m_robotLib;
	IRobotController* m_controller;
public:
	LocalDLLRobotController();
	virtual ~LocalDLLRobotController();

	virtual void SetListener(ITelubeeRobotListener* l) ;
	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	virtual void ConnectRobot() ;
	virtual void DisconnectRobot() ;
	//virtual bool IsConnected() ;
	virtual void UpdateRobotStatus(const RobotStatus& st) ;
	virtual std::string ExecCommand(const std::string& cmd, const std::string& args);

	//New Functions 27/7/2015
	virtual ERobotControllerStatus GetRobotStatus() ;
	virtual void ShutdownRobot() ;
	virtual bool GetJointValues(std::vector<float>& values) ;
	virtual void ManualControlRobot() ;


};

}
}

#endif

