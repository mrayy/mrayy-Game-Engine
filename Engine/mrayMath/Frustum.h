
#ifndef ___Frustum___
#define ___Frustum___

#include "Plane.h"
#include "box3d.h"
#include "matrix4x4.h"
#include "compileConfig.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL Frustum
{
protected:
public:

	enum SF_PLANES
	{
		FP_LEFT_PLANE=0,
		FP_RIGHT_PLANE,
		FP_TOP_PLANE,
		FP_BOTTOM_PLANE,
		FP_NEAR_PLANE,
		FP_FAR_PLANE,
		FP_PLANE_COUNT
	};

	math::vector3d viewPos;
	math::Plane planes[FP_PLANE_COUNT];
	math::box3d boundingBox;

	Frustum();
	Frustum(const math::matrix4x4&projMat);

	math::vector3d getFarLUp()const;
	math::vector3d getFarRUp()const;
	math::vector3d getFarLDown()const;
	math::vector3d getFarRDown()const;

	void set(const math::matrix4x4&projMat);
	inline void transform(const math::matrix4x4 &mat)
	{
		for (int i=0; i<FP_PLANE_COUNT; ++i)
			mat.transformPlane(planes[i]);

		viewPos=mat*viewPos;
		recalcBoundingBox();
	}

	bool pointInFrustrum(const math::vector3d& p)const;
	bool boxInFrustrum(const math::vector3d &p1,const math::vector3d& p2)const;
	bool boxInFrustrum(const math::box3d& b)const;
	bool sphereInFrustrum(const math::vector3d& p,float radius)const;

	void recalcBoundingBox();

};

}
}

#endif
