
/********************************************************************
	created:	2009/02/06
	created:	6:2:2009   13:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalDebugger2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalDebugger2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalDebugger2D___
#define ___IPhysicalDebugger2D___

#include "IVideoDevice.h"

namespace mray{
namespace physics{

enum EPhysics2DDebugFlag{
	E2DDF_Shape,		//draw shapes
	E2DDF_Joint,		//draw joint connections
	E2DDF_CoreShape,	//draw core (TOI) shapes
	E2DDF_AABB,			//draw axis aligned bounding boxes
	E2DDF_OBB,			//draw draw oriented bounding boxes
	E2DDF_Pair,			//draw draw broad-phase pairs
	E2DDF_CenterOfMass,		//draw axis draw center of mass frame

	E2DDF_FlagsCount
};

class IPhysicalDebugger2D
{
public:
	IPhysicalDebugger2D(){}
	virtual~IPhysicalDebugger2D(){}

	virtual void setDevice(video::IVideoDevice*device)=0;
	virtual video::IVideoDevice*getDevice()=0;

	virtual void setFlag(EPhysics2DDebugFlag flag,bool state)=0;
	virtual bool getFlag(EPhysics2DDebugFlag flag)=0;
};

}
}


#endif //___IPhysicalDebugger2D___
