

#ifndef ___PhysXDebugRenderer___
#define ___PhysXDebugRenderer___

#include <GCPtr.h>
#include "compileConfig.h"


namespace mray{
namespace video{
	class IVideoDevice;
}
namespace scene{
	class IMeshBuffer;
}
namespace physics{

class MRAY_PHYSX_DLL PhysXDebugRenderer
{
	GCPtr<scene::IMeshBuffer> m_mesh;
public:
	PhysXDebugRenderer();

	void renderData(const NxDebugRenderable& data) ;

};

}
}




#endif
