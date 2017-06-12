

#ifndef IBaseController_h__
#define IBaseController_h__

#include "point2d.h"

namespace mray
{
	
class IBaseController
{
protected:
public:
	IBaseController(){}
	virtual ~IBaseController(){}


	virtual bool Connect(const core::string& port) = 0;
	virtual bool IsConnected() = 0;
	virtual void Disconnect()=0;

	virtual void Start() {}
	virtual void Stop() {}
	virtual bool IsStarted(){ return true; }

	virtual void Drive(const math::vector2di& speed, int rotationSpeed)=0;
	virtual void DriveStop()=0;

	virtual void UpdateSensors(){}

	virtual std::string ExecCommand(const core::string& cmd, const core::string& args) { return ""; }

	virtual int GetSensorCount() = 0;
	virtual float GetSensorValue(int s) = 0;
	virtual int GetBatteryLevel() = 0;
};

}

#endif // IBaseController_h__

