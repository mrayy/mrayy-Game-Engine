

#ifndef __IHEADCONTROLLER__
#define __IHEADCONTROLLER__

#include "point3d.h"
#include "IRobotComponent.h"


namespace mray
{

class IHeadController :public IRobotComponent
{
protected:

public:
	IHeadController(){}
	virtual ~IHeadController(){}

	virtual std::string GetType(){ return "RobotHead"; };

	virtual void SetRotation(const math::vector3d& rotation) = 0;
	virtual math::vector3d GetRotation() = 0;

	virtual std::string ExecCommand(const core::string& cmd, const core::string& args) { return ""; }

};

}


#endif
