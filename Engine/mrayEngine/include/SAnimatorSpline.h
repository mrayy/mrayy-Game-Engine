

#ifndef ___SAnimatorSpline___
#define ___SAnimatorSpline___

#include "IAnimators.h"




namespace mray{
namespace scene{
class ISceneNode;

class MRAY_DLL SAnimatorSpline:public IAnimators
{
	std::vector<math::vector3d> points;
	float Speed;
	float Tightness;
	float currTim;
	
public:
	SAnimatorSpline(std::vector<math::vector3d> &p,float speed,float tightness);

	virtual void animate(IMovable*  node,float dt);
};


}
}



#endif







