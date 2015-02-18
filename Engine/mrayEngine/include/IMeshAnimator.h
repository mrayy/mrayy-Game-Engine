
/********************************************************************
	created:	2009/03/15
	created:	15:3:2009   23:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IMeshAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IMeshAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IMeshAnimator___
#define ___IMeshAnimator___

#include "GCPtr.h"

namespace mray{
namespace scene{

class IMeshBuffer;

MakeSharedPtrType(IMeshAnimator);

class IMeshAnimator
{
private:
protected:
public:
	IMeshAnimator(){}
	virtual~IMeshAnimator(){}

	virtual void update(float dt)=0;
	virtual IMeshBuffer* getMeshBuffer()=0;
	virtual void setMeshBuffer(IMeshBuffer*b)=0;

	virtual IMeshAnimator* duplicate()=0;
};


}
}


#endif //___IMeshAnimator___
