


#ifndef ___IANIMATORS___
#define ___IANIMATORS___


#include "CompileConfig.h"
#include "GCPtr.h"

namespace mray{
namespace scene{

	class IMovable;


enum ANIMATORS_TYPES
{
	AT_CIRCLE
};

class IAnimators  
{
public:
	virtual ~IAnimators(){}
	virtual void animate(IMovable*node,float dt)=0;
};

MakeSharedPtrType(IAnimators)


};//scene
};//mray


#endif









