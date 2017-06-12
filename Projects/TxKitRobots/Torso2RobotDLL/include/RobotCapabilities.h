


#ifndef __ROBOTCAPABILITIES__
#define __ROBOTCAPABILITIES__

// #include <point3d.h>
//#include <point2d.h>
#include <vector>

namespace mray
{
namespace TBee
{
	
class RobotCapabilities
{
public:
	bool hasBattery;
	bool hasDistanceSensors;
	bool hasBumpSensors;
	bool canMove;
	bool canRotate;
	bool hasParallaxMotion;
	bool hasHands;

	int distanceSensorCount;
	int bumpSensorCount;

	int bodyJointsCount;
	int armsJointsCount;
	int handsJointsCount;

// 	math::Point3d<bool> enabledMotion;
// 	math::Point3d<bool> enabledRotation;
// 	math::vector3d headLimits[2];


public:

	RobotCapabilities()
	{
		hasBattery = false;
		hasDistanceSensors = 0;
		hasBumpSensors = 0;
		canMove = 0;
		canRotate = 0;
		hasParallaxMotion = 0;
		hasHands = 0;
		distanceSensorCount = 0;
		bumpSensorCount = 0;
		bodyJointsCount = 0;
		armsJointsCount = 0;
		handsJointsCount = 0;
// 		headLimits[0].set(-180, -180, -180);
// 		headLimits[1].set(180, 180, 180);

	}
	virtual ~RobotCapabilities()
	{
	}

};

}
}


#endif
