

#ifndef ___IAnimLoader___
#define ___IAnimLoader___

#include "IStream.h"


namespace mray{
namespace animation{
	class TransformationAnimationMixer;
}
namespace OS{
	class IStream;
}

namespace loaders{

class IAnimLoader
{
protected:
public:
	IAnimLoader(){};
	virtual~IAnimLoader(){};
	virtual animation::TransformationAnimationMixer* load(const  core::string& name)=0;
	virtual animation::TransformationAnimationMixer* load(OS::IStream* file)=0;
	virtual bool canLoad(const core::string&name)=0;
};

MakeSharedPtrType(IAnimLoader)


};//loaders
};//mray


#endif















