/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   11:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IInverseKinmatics.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IInverseKinmatics
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IInverseKinmatics___
#define ___IInverseKinmatics___

#include "IJointQuery.h"

namespace mray{
namespace animation{

class MRAY_DLL IInverseKinmatics
{
private:
protected:
public:
	IInverseKinmatics(){}
	virtual~IInverseKinmatics(){}

	virtual void calcOrintation(bool e)=0;
	virtual bool isCalcOrintation()=0;

	virtual void setJointsChain(IJointQuery* q)=0;
	virtual void setTarget(const math::vector3d&p,const math::quaternion&r)=0;

	virtual void update()=0;
};

}
}

#endif //___IInverseKinmatics___