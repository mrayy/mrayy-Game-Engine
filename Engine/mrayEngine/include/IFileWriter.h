/********************************************************************
	created:	2009/02/12
	created:	12:2:2009   18:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IFileWriter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IFileWriter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IFileWriter___
#define ___IFileWriter___

#include "mstring.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace loaders{

class IFileWriter
{
public:

	virtual~IFileWriter(){};
	virtual bool canWrite(const core::string& fileName)=0;
	virtual const core::string& getExt()=0;

	virtual const core::string& getDescription()=0;
};

}
}


#endif //___IFileWriter___