

/********************************************************************
	created:	2010/04/18
	created:	18:4:2010   7:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IDynamicObstacleSolver.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IDynamicObstacleSolver
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Solve the movements in dynamic environment
*********************************************************************/

#ifndef IDynamicObstacleSolver_h__
#define IDynamicObstacleSolver_h__

#include "AgentObstacle.h"
#include "IAIComponent.h"

namespace mray
{
namespace AI
{

	class IRenderDevice;

class IDynamicObstacleSolver:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
public:
	IDynamicObstacleSolver()
	{}
	virtual ~IDynamicObstacleSolver(){}

	virtual void CalculateAvoidanceForce()=0;
	virtual const math::quaternion& GetResultOrientation()=0;
	virtual float GetResultSpeed()=0;
	
	virtual void Visualize(IRenderDevice*dev){}
};

}
}


#endif // IDynamicObstacleSolver_h__
