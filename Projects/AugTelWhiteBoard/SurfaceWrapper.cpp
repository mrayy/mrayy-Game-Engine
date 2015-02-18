
#include "stdafx.h"
#include "SurfaceWrapper.h"



namespace mray
{
namespace AugTel
{


SurfaceWrapper::SurfaceWrapper()
{
	setIdentity();
}
SurfaceWrapper::~SurfaceWrapper()
{
}

void SurfaceWrapper::setIdentity()
{
	setSource(math::vector2d(0, 0), math::vector2d(1, 0), math::vector2d(0, 1), math::vector2d(1, 1));
	setDestination(math::vector2d(0, 0), math::vector2d(1, 0), math::vector2d(0, 1), math::vector2d(1, 1));
	ComputeWrap();
}
void SurfaceWrapper::setSource(const math::vector2d& p0, const math::vector2d& p1, const math::vector2d& p2, const math::vector2d& p3)
{
	m_source[0] = p0;
	m_source[1] = p1;
	m_source[2] = p2;
	m_source[3] = p3;
	m_dirty = true;
}
void SurfaceWrapper::setDestination(const math::vector2d& p0, const math::vector2d& p1, const math::vector2d& p2, const math::vector2d& p3)
{
	m_dest[0] = p0;
	m_dest[1] = p1;
	m_dest[2] = p2;
	m_dest[3] = p3;
	m_dirty = true;
}
void multMats(float* srcMat, float* dstMat, float* resMat) {
	// DSTDO/CBB: could be faster, but not called often enough to matter
	for (int r = 0; r < 4; r++) {
		int ri = r * 4;
		for (int c = 0; c < 4; c++) {
			resMat[ri + c] = (srcMat[ri] * dstMat[c] +
				srcMat[ri + 1] * dstMat[c + 4] +
				srcMat[ri + 2] * dstMat[c + 8] +
				srcMat[ri + 3] * dstMat[c + 12]);
		}
	}
}

void SurfaceWrapper::ComputeWrap()
{
	ComputeQuadToSquare(m_source[0].x, m_source[0].y, m_source[1].x, m_source[1].y, m_source[2].x, m_source[2].y, m_source[3].x, m_source[3].y, m_srcMat.getMatPointer());
	ComputeSquareToQuad(m_dest[0].x, m_dest[0].y, m_dest[1].x, m_dest[1].y, m_dest[2].x, m_dest[2].y, m_dest[3].x, m_dest[3].y, m_dstMat.getMatPointer());
	multMats(m_srcMat.getMatPointer(), m_dstMat.getMatPointer(), m_wrapMat.getMatPointer());
	m_dirty = false;
}
void SurfaceWrapper::ComputeSquareToQuad(float x0,
	float y0,
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3, float* mat)
{

	float dx1 = x1 - x2, dy1 = y1 - y2;
	float dx2 = x3 - x2, dy2 = y3 - y2;
	float sx = x0 - x1 + x2 - x3;
	float sy = y0 - y1 + y2 - y3;
	float g = (sx * dy2 - dx2 * sy) / (dx1 * dy2 - dx2 * dy1);
	float h = (dx1 * sy - sx * dy1) / (dx1 * dy2 - dx2 * dy1);
	float a = x1 - x0 + g * x1;
	float b = x3 - x0 + h * x3;
	float c = x0;
	float d = y1 - y0 + g * y1;
	float e = y3 - y0 + h * y3;
	float f = y0;

	mat[0] = a;	mat[1] = d;	mat[2] = 0;	mat[3] = g;
	mat[4] = b;	mat[5] = e;	mat[6] = 0;	mat[7] = h;
	mat[8] = 0;	mat[9] = 0;	mat[10] = 1;	mat[11] = 0;
	mat[12] = c;	mat[13] = f;	mat[14] = 0;	mat[15] = 1;
}

void SurfaceWrapper::ComputeQuadToSquare(
	float x0,
	float y0,
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3, float* mat)
{
	ComputeSquareToQuad(x0, y0, x1, y1, x2, y2, x3, y3, mat);


	float a = mat[0], d = mat[1],	/* ignore */		g = mat[3];
	float b = mat[4], e = mat[5],	/* 3rd col*/		h = mat[7];
	/* ignore 3rd row */
	float c = mat[12], f = mat[13];

	float A = e - f * h;
	float B = c * h - b;
	float C = b * f - c * e;
	float D = f * g - d;
	float E = a - c * g;
	float F = c * d - a * f;
	float G = d * h - e * g;
	float H = b * g - a * h;
	float I = a * e - b * d;

	// Probably unnecessary since 'I' is also scaled by the determinant,
	//   and 'I' scales the homogeneous coordinate, which, in turn,
	//   scales the X,Y coordinates.
	// Determinant  =   a * (e - f * h) + b * (f * g - d) + c * (d * h - e * g);
	float idet = 1.0f / (a * A + b * D + c * G);


	mat[0] = A * idet;	mat[1] = D * idet;	mat[2] = 0;	mat[3] = G * idet;
	mat[4] = B * idet;	mat[5] = E * idet;	mat[6] = 0;	mat[7] = H * idet;
	mat[8] = 0;	mat[9] = 0;	mat[10] = 1;	mat[11] = 0;
	mat[12] = C * idet;	mat[13] = F * idet;	mat[14] = 0;	mat[15] = I * idet;
}

math::vector2d SurfaceWrapper::Wrap(const math::vector2d& p)
{
	if (m_dirty)
		ComputeWrap();
	math::vector4d result;

	float z = 0;
	float* mat = m_wrapMat.getMatPointer();
	result[0] = (float)(p.x * mat[0] + p.y*mat[4] + z*mat[8] + 1 * mat[12]);
	result[1] = (float)(p.x * mat[1] + p.y*mat[5] + z*mat[9] + 1 * mat[13]);
	result[2] = (float)(p.x * mat[2] + p.y*mat[6] + z*mat[10] + 1 * mat[14]);
	result[3] = (float)(p.x * mat[3] + p.y*mat[7] + z*mat[11] + 1 * mat[15]);
	
	return math::vector2d(result.x / result.w, result.y / result.w);
}


}
}
