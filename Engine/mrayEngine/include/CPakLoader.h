

#ifndef ___CPakLoader___
#define ___CPakLoader___


#include "IArchiveLoader.h"

namespace mray{
namespace OS{

class MRAY_DLL CPakLoader:public IArchiveLoader
{
public:
	virtual ~CPakLoader();
	virtual IArchivePtr loadArchive(const  core::string&name);
	virtual core::string getArchiveType();
	virtual bool canLoadArchive(const  core::string&name);
};

}
}



#endif



