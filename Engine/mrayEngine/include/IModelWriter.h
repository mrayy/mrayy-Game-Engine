


/********************************************************************
	created:	2012/02/11
	created:	11:2:2012   16:38
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IModelWriter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IModelWriter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IModelWriter__
#define __IModelWriter__

#include "GCPtr.h"
#include "mstring.h"
#include "IFileWriter.h"

namespace mray
{
namespace OS{
	class IStream;
}
namespace scene{
	class SMesh;
}
namespace loaders
{

class IModelWriter:public IFileWriter
{
private:
protected:
public:
	IModelWriter(){}
	virtual~IModelWriter(){}

	virtual bool write(scene::SMesh*m,OS::IStream* stream)=0;
};

}
}

#endif