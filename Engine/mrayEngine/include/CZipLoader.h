

#ifndef ___CZipLoader___
#define ___CZipLoader___


#include "IArchiveLoader.h"

namespace mray{
namespace OS{

class MRAY_DLL CZipLoader:public IArchiveLoader
{
public:
	virtual ~CZipLoader();
	virtual IArchivePtr loadArchive(const  core::string&name);
	virtual core::string getArchiveType();
	virtual bool canLoadArchive(const  core::string&name);
};

}
}



#endif



