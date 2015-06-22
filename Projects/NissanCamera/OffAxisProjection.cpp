
#include "stdafx.h"
#include "OffAxisProjection.h"


namespace mray
{
namespace video
{

OffAxisProjection::OffAxisProjection()
{
	m_dirty = true;
	m_znear = 0.1;
	m_zfar = 100;
	SetScreenCorners(math::vector3d(-1, -1, 1), math::vector3d(-1, 1, 1), math::vector3d(1, 1, 1));
}
OffAxisProjection::~OffAxisProjection()
{

}
void OffAxisProjection::_UpdateMatrix()
{
	if (!m_dirty)
		return;
	m_dirty = false;

	math::vector3d va, vb, vc;
	math::vector3d vr, vu, vn;
	float l, r, b, t, d;

	// Compute an orthonormal basis for the screen

	vr = m_pb - m_pa;
	vu = m_pc - m_pa;

	vr.Normalize();
	vu.Normalize();

	vn = -vr.crossProduct(vu);

	// Compute Screen corner vectors
	va = m_pa - m_pos;
	vb = m_pb - m_pos;
	vc = m_pc - m_pos;

	//Find the distance from the eye to screen plane
	d = -va.dotProduct(vn);

	//Find the extent of the perpendicular projection
	l = vr.dotProduct(va)*m_znear / d;
	r = vr.dotProduct(vb)*m_znear / d;
	b = vu.dotProduct(va)*m_znear / d;
	t = vu.dotProduct(vc)*m_znear / d;

	//Set Projection Matrix

	m_projection.m4x4[0][0] = 2.0f*m_znear / (r - l);
	m_projection.m4x4[0][1] = 0.0f;
	m_projection.m4x4[0][2] = (r + l) / (r - l);
	m_projection.m4x4[0][3] = 0.0;

	m_projection.m4x4[1][0] = 0.0;
	m_projection.m4x4[1][1] = 2.0f*m_znear / (t - b);
	m_projection.m4x4[1][2] = (t + b) / (t - b);
	m_projection.m4x4[1][3] = 0.0;

	m_projection.m4x4[2][0] = 0.0;
	m_projection.m4x4[2][1] = 0.0;
	m_projection.m4x4[2][2] = (m_znear + m_zfar) / (m_znear - m_zfar);
	m_projection.m4x4[2][3] = 2.0f*m_znear*m_zfar / (m_znear - m_zfar);

	m_projection.m4x4[3][0] = 0.0;
	m_projection.m4x4[3][1] = 0.0;
	m_projection.m4x4[3][2] = -1.0;
	m_projection.m4x4[3][3] = 0.0;

	m_projection.flagNotIdentity();

	math::matrix4x4 rot,tran;

	rot.m4x4[0][0] = vr.x;
	rot.m4x4[0][1] = vr.y;
	rot.m4x4[0][2] = vr.z;
	rot.m4x4[0][3] = 0;

	rot.m4x4[1][0] = vu.x;
	rot.m4x4[1][1] = vu.y;
	rot.m4x4[1][2] = vu.z;
	rot.m4x4[1][3] = 0;

	rot.m4x4[2][0] = vn.x;
	rot.m4x4[2][1] = vn.y;
	rot.m4x4[2][2] = vn.z;
	rot.m4x4[2][3] = 0;

	rot.m4x4[3][0] = 0;
	rot.m4x4[3][1] = 0;
	rot.m4x4[3][2] = 0;
	rot.m4x4[3][3] = 1;

	rot.flagNotIdentity();
	//rot = rot.getTransposed();
	tran(0, 3) = -m_pos.x;
	tran(1, 3) = -m_pos.y;
	tran(2, 3) = -m_pos.z;
	tran.flagNotIdentity();

	m_view = rot*tran;

	m_rotation.fromMatrix(math::MathUtil::CreateLookAtMatrix((m_pb + m_pc)*0.5f, m_pos, vu));

	
	float ba = (m_pb - m_pa).Length();
	float ca = (m_pc - m_pa).Length();
	float vlen = va.Length();
	m_fov = math::toDeg(atan2(ba+ca, vlen));
}
void OffAxisProjection::SetEyePosition(const math::vector3d& pos)
{
	m_pos = pos;
	m_dirty = true;
}
void OffAxisProjection::SetZNearFar(float near, float far)
{
	m_znear = near;
	m_zfar = far;
	m_dirty = true;
}
void OffAxisProjection::SetScreenCorners(const math::vector3d& bottomLeft, const math::vector3d& topLeft, const math::vector3d& bottomRight)
{
	m_pa = bottomLeft;
	m_pb = bottomRight;
	m_pc = topLeft;
	m_dirty = true;
}

void OffAxisProjection::SetScreenParams(const math::vector3d& center, const math::vector2d& hsize, const math::quaternion& ori)
{
	math::vector3d pa, pb, pc;

	pa = center + ori*math::vector3d(-hsize.x, -hsize.y, 0);
	pb = center + ori*math::vector3d(-hsize.x, hsize.y, 0);
	pc = center + ori*math::vector3d(hsize.x, -hsize.y, 0);

	SetScreenCorners(pa, pb, pc);
}

const math::matrix4x4& OffAxisProjection::GetProjectionMatrix()
{
	_UpdateMatrix();
	return m_projection;
}
const math::matrix4x4& OffAxisProjection::GetViewMatrix()
{
	_UpdateMatrix();
	return m_view;
}
const math::quaternion& OffAxisProjection::GetRotation()
{
	_UpdateMatrix();
	return m_rotation;
}
float OffAxisProjection::GetFoV()
{
	_UpdateMatrix();
	return m_fov;
}

}
}

