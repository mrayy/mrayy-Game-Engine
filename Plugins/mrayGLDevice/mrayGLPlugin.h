

#ifndef ___mrayGLPlugin___
#define ___mrayGLPlugin___

#include <IPlugin.h>
#include <GCPtr.h>
#include "compileConfig.h"

namespace mray{
namespace plugins{

class MRAY_GL_DLL mrayGLPlugin:public IPlugin
{
public:
	mrayGLPlugin();
	virtual~mrayGLPlugin();

	virtual core::string getName();

	virtual void install();
	virtual void uninstall();
};

}
}




#endif


