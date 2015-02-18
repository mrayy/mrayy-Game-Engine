

#ifndef ___IMeshLoader___
#define ___IMeshLoader___

#include "GCPtr.h"
#include "mstring.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace scene{
	class SMesh;
}
namespace loaders{

class IMeshLoader
{
protected:
public:
	IMeshLoader(){};
	virtual~IMeshLoader(){};
	virtual scene::SMesh* load(const  core::string& name)=0;
	virtual scene::SMesh* load(OS::IStream* stream)=0;
	virtual bool canLoad(const  core::string&name)=0;
	virtual const core::string&getExtension()=0;
};

MakeSharedPtrType(IMeshLoader);


};//loaders
};//mray

#endif






