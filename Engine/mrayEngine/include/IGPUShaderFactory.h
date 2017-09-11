


#ifndef ___IGPUShaderFactory___
#define ___IGPUShaderFactory___

#include "compileconfig.h"
#include "GCPtr.h"
#include "mTypes.h"
#include "IGPUShaderProgram.h"

namespace mray{
	namespace OS{
		class IStream;
	}
namespace video{
	class IVideoDevice;

class MRAY_DLL IGPUShaderFactory
{
public:
	IGPUShaderFactory(){}
	virtual ~IGPUShaderFactory(){}

	virtual const core::string&getType()=0;

	virtual bool canCreateType(const core::string& type)=0;

	virtual IGPUShaderProgram* createShader(EShaderProgramType type)=0;

};

}
}


#endif


