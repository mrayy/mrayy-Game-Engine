

#ifndef __RobotHandler__
#define __RobotHandler__

#include "IRobotController.h"

namespace mray
{
namespace TBee
{
	class RobotHandler;

	class IRobotHandlerListener
	{
	public:
		virtual void OnCalibrationDone(RobotHandler* sender){};

		virtual void OnCollisionData(RobotHandler* sender, float left, float right){}

		virtual void OnReportMessage(RobotHandler* sender, int code, const core::string& msg){}
		virtual void OnRobotStatus(RobotHandler* sender, const RobotStatus& status){};
	};
	
class RobotHandler: public IRobotStatusProvider
{
protected:

	RobotStatus m_robotStatus;

	OS::IDynamicLibraryPtr m_robotLib;
	IRobotController* m_robotController;
	bool m_localControl;
	IRobotHandlerListener* m_listener;

	void _RobotStatus(const RobotStatus& st);

public:
	RobotHandler();
	virtual ~RobotHandler();
	
	void Initialize();

	const RobotStatus& GetRobotStatus()const{
		return m_robotStatus;
	}

	virtual void GetRobotStatus(RobotStatus& st)const;

	IRobotController* GetRobotController(){ return m_robotController; }

	void SetLocalControl(bool c){ m_localControl = c; }
	bool IsLocalControl(){ return m_localControl; }

	void SetRobotData(const RobotStatus &st);

	virtual void OnCollisionData(float left, float right);
	void OnReportMessage(int code, const std::string& msg);

	void SetListener(IRobotHandlerListener* l){ m_listener = l; }
};

}
}


#endif
