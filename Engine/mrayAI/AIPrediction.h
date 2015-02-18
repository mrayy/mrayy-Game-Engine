


/********************************************************************
	created:	2010/05/28
	created:	28:5:2010   16:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIPrediction.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIPrediction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIPrediction_h__
#define AIPrediction_h__

#include "CompileConfig.h"
#include "point3d.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL AIPrediction
{
private:
protected:
public:

	//predict the next position after "t" time
	static math::vector3d PredictPosition(const math::vector3d& currPos,const math::vector3d& vel,const math::vector3d& acc,float t);
	
	static float PredictCollisionTime(const math::vector3d& aPos,const math::vector3d& aVel,const math::vector3d& aAcc,float aRad,
		const math::vector3d& bPos,const math::vector3d& bVel,const math::vector3d& bAcc,float bRad);

};

}
}

#endif // AIPrediction_h__
