

/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   13:27
	filename: 	C:\Development\mrayEngine\Projects\AugTel\SurfaceWrapper.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	SurfaceWrapper
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SurfaceWrapper__
#define __SurfaceWrapper__




namespace mray
{
namespace AugTel
{

class SurfaceWrapper
{
protected:
	math::vector2d m_source[4];
	math::vector2d m_dest[4];

	math::matrix4x4 m_srcMat;
	math::matrix4x4 m_dstMat;
	math::matrix4x4 m_wrapMat;

	bool m_dirty;
	void ComputeSquareToQuad(float x0,
		float y0,
		float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3, float* mat);
	void ComputeQuadToSquare(float x0,
		float y0,
		float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3, float* mat);
	void ComputeWrap();

public:
	SurfaceWrapper();
	virtual~SurfaceWrapper();

	void setIdentity();
	void setSource(const math::vector2d& p0, const math::vector2d& p1, const math::vector2d& p2, const math::vector2d& p3);
	void setDestination(const math::vector2d& p0, const math::vector2d& p1, const math::vector2d& p2, const math::vector2d& p3);

	math::vector2d Wrap(const math::vector2d& p);


};

}
}


#endif
