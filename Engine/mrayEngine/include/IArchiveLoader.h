


#ifndef ___IArchiveLoader___
#define ___IArchiveLoader___


#include "GCPtr.h"
#include "mTypes.h"
#include "mstring.h"
#include "IArchive.h"

namespace mray{
namespace OS{

class IArchiveLoader
{
public:
	virtual ~IArchiveLoader(){};
	virtual IArchivePtr loadArchive(const  core::string&name)=0;
	virtual core::string getArchiveType()=0;
	virtual bool canLoadArchive(const  core::string&name)=0;
};

}
}


#endif

