

/********************************************************************
	created:	2010/09/17
	created:	17:9:2010   20:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\MathUtil.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	MathUtil
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MathUtil_h__
#define MathUtil_h__

#include "matrix4x4.h"
#include "Plane.h"
#include "quaternion.h"

namespace mray
{
namespace math
{


class MathUtil
{
private:
protected:
public:
	

	static float GaussianDistribution(float x,float offset,float scale);


	//Matrix Util
	static matrix4x4 CreateRefelectionMatrix(const math::Plane&p);

	static matrix4x4 CreateProjectionMatrixPerspectiveFov(float fieldOfView,float aspect,float znear,float zfar);
	
	static matrix4x4 CreateProjectionMatrixOrtho(float left,float right,float bottom,float top,float znear,float zfar);
	static matrix4x4 CreateProjectionMatrixPerspective(float left,float right,float bottom,float top,float znear,float zfar);

	static matrix4x4 CreateLookAtMatrix(const vector3d&pos,const quaternion&ori);
	static matrix4x4 CreateLookAtMatrix(const vector3d& pos,const vector3d& target,const vector3d& up);

	static matrix4x4 CreateSkewSymmetric(const vector3d &v);
};

}
}

#endif // MathUtil_h__
