
/********************************************************************
	created:	2009/02/12
	created:	12:2:2009   18:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IFileLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IFileLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IFileLoader___
#define ___IFileLoader___

#include "mstring.h"
#include "GCPtr.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace loaders{

class IFileLoader
{
public:
	virtual ~IFileLoader(){}

	virtual bool canLoad(const core::string& fileName)=0;
	virtual bool canLoad(OS::IStream* file)=0;

	virtual const core::string& getDescription()=0;
};

}
}


#endif //___IFileLoader___
