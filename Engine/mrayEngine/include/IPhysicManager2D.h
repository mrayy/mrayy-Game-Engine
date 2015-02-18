

/********************************************************************
	created:	2009/02/02
	created:	2:2:2009   23:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicManager2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicManager2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicManager2D___
#define ___IPhysicManager2D___


#include "IPhysicalNode2D.h"
#include "IPhysicalJoint2D.h"
#include "IPhysics2DDef.h"
#include "IPhysicalDebugger2D.h"
#include "IPhysicContactListener2D.h"

namespace mray{
namespace physics{

class IPhysicManager2D
{
public:
	virtual void update(float dt)=0;

	virtual IPhysicalDebugger2D* getDebugger()=0;

	virtual bool isShapeSupported(ESupported2DShapes s)=0;
	virtual bool isJointSupported(ESupported2DJoints s)=0;

	virtual GCPtr<IPhysicalNode2D> createNode(IPhysicalNode2DDesc*desc,IPhysicalShape2DDef*shapeDef)=0;
	virtual GCPtr<IPhysicalJoint2D> createJoint(IPhysicalJoint2DDesc*desc)=0;

	virtual void setContactListner(IPhysicContactListener2D*listner)=0;

	virtual void registerBenchmark()=0;
	virtual void unregisterBenchmark()=0;
};

}
}


#endif //___IPhysicManager2D___
