
#ifndef ___CRotateConstraint___
#define ___CRotateConstraint___


#include "IAnimators.h"



namespace mray{
namespace scene{

class CRotateConstraint:public scene::IAnimators
{
	math::Point3d<bool> m_eMin;
	math::Point3d<bool> m_eMax;
	math::vector3d m_Min;
	math::vector3d m_Max;
public:
	CRotateConstraint(math::vector3d min,math::vector3d max,math::Point3d<bool> emin,math::Point3d<bool> emax);
	virtual void animate(scene::IMovable *node,float dt);
};


}
}

#endif


