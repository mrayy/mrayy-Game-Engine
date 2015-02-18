#ifndef ___ITerrain___
#define ___ITerrain___

#include "IRenderable.h"

namespace mray{
namespace scene{

class ITerrain:public IRenderable
{
public:
	ITerrain(){};
	
	virtual float getHeight(float x,float z,math::vector3d *outNormal=0)=0;
	
	virtual void setPosition(const math::vector3d& v)=0;
	//virtual void setRotation(const math::vector3d&v)=0;
	virtual void setScale(const math::vector3d&v)=0;

	//	0----1
	//  | \  |
	//  |  \ |	Dir = 0,else Dir=1
	//  3----2
	virtual void QueryQuads2D(const math::rectf&rc,std::vector<math::rectf>& outRects,bool &splitDir)=0;
};


}
}


#endif

