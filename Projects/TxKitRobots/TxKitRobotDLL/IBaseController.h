

#ifndef IBaseController_h__
#define IBaseController_h__

#include "point2d.h"
#include "IRobotComponent.h"

namespace mray
{
	
class IBaseController :public IRobotComponent
{
protected:
public:
	IBaseController(){}
	virtual ~IBaseController(){}

	virtual std::string GetType(){ return "RobotBase"; };

	virtual void Drive(const math::vector2di& speed, int rotationSpeed)=0;
	virtual void DriveStop()=0;

	virtual void UpdateSensors(){}


	virtual int GetSensorCount() = 0;
	virtual float GetSensorValue(int s) = 0;
	virtual int GetBatteryLevel() = 0;
};

}

#endif // IBaseController_h__

