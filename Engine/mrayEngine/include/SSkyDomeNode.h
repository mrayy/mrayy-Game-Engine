

#ifndef ___SSKEYDOOM_NODE___
#define ___SSKEYDOOM_NODE___


#include "SMesh.h"
#include "ISkyShape.h"

namespace mray{
namespace scene{

class SceneManager;
class MRAY_DLL SSkyDomeNode:public ISkyShape
{
public:
	SSkyDomeNode(const video::ITexturePtr& texture,int uSegments,int vSegments,
		double texPercentage,double spherePercentage,float radius);
	virtual ~SSkyDomeNode();

	

};

}
}


#endif










