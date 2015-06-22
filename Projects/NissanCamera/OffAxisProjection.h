


#ifndef __OFFAXISPROJECTION__
#define __OFFAXISPROJECTION__

//http://csc.lsu.edu/~kooima/pdfs/gen-perspective.pdf

namespace mray
{
namespace video
{
	
class OffAxisProjection
{
protected:

	bool m_dirty;

	math::vector3d m_pos;
	math::vector3d m_pa,m_pb,m_pc;

	float m_zfar, m_znear;

	float m_fov;

	math::matrix4x4 m_projection;
	math::matrix4x4 m_view;

	math::quaternion m_rotation;


	void _UpdateMatrix();

public:
	OffAxisProjection();
	virtual ~OffAxisProjection();

	void SetEyePosition(const math::vector3d& pos);
	void SetZNearFar(float near, float far);

	//pa: Lower Left Corner of the screen 
	//pb: Top Left Corner of the screen 
	//pc: Lower Right Corner of the screen 
	// pc	----------------
	// |                |
	// |                |
	// |                |
	// pb---------------pa
	void SetScreenCorners(const math::vector3d& bottomLeft, const math::vector3d& topLeft, const math::vector3d& bottomRight);
	
	// Calling this function will calculate screen corners
	void SetScreenParams(const math::vector3d& center,const math::vector2d& hsize,const math::quaternion& ori);

	const math::matrix4x4& GetProjectionMatrix();
	const math::matrix4x4& GetViewMatrix();
	const math::quaternion& GetRotation();
	float GetFoV();
};

}
}


#endif
