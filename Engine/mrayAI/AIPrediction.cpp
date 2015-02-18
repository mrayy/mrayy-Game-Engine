#include "stdafx.h"


#include "AIPrediction.h"


namespace mray
{
namespace AI
{

math::vector3d AIPrediction::PredictPosition(const math::vector3d& currPos,const math::vector3d& vel,const math::vector3d& acc,float t)
{
	return currPos+(vel+acc*t)*t;
}

float AIPrediction::PredictCollisionTime(const math::vector3d& aPos,const math::vector3d& aVel,const math::vector3d& aAcc,float aRad,
	const math::vector3d& bPos,const math::vector3d& bVel,const math::vector3d& bAcc,float bRad)
{
	math::vector3d a=(bAcc-aAcc);
	math::vector3d v=(bVel-aVel);
	math::vector3d p=(bPos-aPos);

	math::vector3d delta=v*v-a*p*4;
	if(delta.x<0 || delta.y<0 || delta.z<0)
		return -1;

	

	return 0;
}


}
}
