
/********************************************************************
	created:	2012/02/19
	created:	19:2:2012   12:08
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IAnimWriter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IAnimWriter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IAnimWriter__
#define __IAnimWriter__

#include "mstring.h"
#include "GCptr.h"

namespace mray
{
	namespace OS{
		class IStream;
	}
namespace animation
{
	class TransformationAnimationMixer;
}

namespace loaders
{


class IAnimWriter
{
private:
protected:
public:
	IAnimWriter(){}
	virtual~IAnimWriter(){}
	virtual bool write(animation::TransformationAnimationMixer* mixer,const  core::string& name)=0;
	virtual bool write(animation::TransformationAnimationMixer* mixer,OS::IStream* file)=0;
	virtual bool canWrite(const core::string&name)=0;
};

MakeSharedPtrType(IAnimWriter)


};//loaders
}

#endif

