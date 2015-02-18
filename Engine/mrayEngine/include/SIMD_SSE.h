
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   23:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SIMD_SSE.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SIMD_SSE
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SIMD_SSE___
#define ___SIMD_SSE___

#include "SIMD_Default.h"

namespace mray{

class MRAY_DLL SIMD_SSE:public SIMD_Default
{
private:
protected:
public:
	SIMD_SSE(){}
	virtual~SIMD_SSE(){}

	virtual const mchar*VPCALL getName();

	virtual void VPCALL Add(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL Add(float *dst,const float*src0,const float*src1,const const int count);
	virtual void VPCALL Sub(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL Sub(float *dst,const float*src0,const float*src1,const const int count);
	virtual void VPCALL Mul(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL Mul(float *dst,const float*src0,const float*src1,const const int count);
	virtual void VPCALL Div(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL Div(float *dst,const float*src0,const float*src1,const const int count);
	virtual void VPCALL MulAdd(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL MulAdd(float *dst,const float*src0,const float*src1,const const int count);
	virtual void VPCALL MulSub(float *dst,const float c,const float*src,const const int count);
	virtual void VPCALL MulSub(float *dst,const float*src0,const float*src1,const const int count);

	virtual void VPCALL Dot(float*dst,const math::vector3d&c,const math::vector3d*src,const int count);
	virtual void VPCALL Dot(float*dst,const math::vector3d*src0,const math::vector3d*src1,const int count);
	virtual void VPCALL Dot(float&dst,const float*src0,const float*src1,const int count);

	virtual void VPCALL CmpGT(uchar*dst,const float c,const float*src,const int count);
	virtual void VPCALL CmpGE(uchar*dst,const float c,const float*src,const int count);
	virtual void VPCALL CmpLT(uchar*dst,const float c,const float*src,const int count);
	virtual void VPCALL CmpLE(uchar*dst,const float c,const float*src,const int count);

	virtual void VPCALL MinMax(float&min,float&max,const float*src,const int count);
	virtual void VPCALL MinMax(math::vector2d&min,math::vector2d&max,const math::vector2d*src,const int count);
	virtual void VPCALL MinMax(math::vector3d&min,math::vector3d&max,const math::vector3d*src,const int count);

	virtual void VPCALL Clamp(float*dst,const float min,const float max,const float*src,const int count);
	virtual void VPCALL ClampMin(float*dst,const float min,const float*src,const int count);
	virtual void VPCALL ClampMax(float*dst,const float max,const float*src,const int count);

	//16 bit Aligned
	virtual void VPCALL Zero16(float*dst,const int count);
	virtual void VPCALL Negate16(float*dst,const int count);
	virtual void VPCALL Copy16(float*dst,const float*src,const int count);
	virtual void VPCALL Add16(float*dst,const float*src1,const float*src2,const int count);
	virtual void VPCALL Sub16(float*dst,const float*src1,const float*src2,const int count);
	virtual void VPCALL Mult16(float*dst,const float*src1,const float c,const int count);

	virtual void VPCALL AddAssign16(float*dst,const float*src,const int count);
	virtual void VPCALL SubAssign16(float*dst,const float*src,const int count);
	virtual void VPCALL MultAssign16(float*dst,const float c,const int count);

	//Matrix
	virtual void VPCALL Mat_MultiplyVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v);
	virtual void VPCALL Mat_MultiplyAddVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v);
	virtual void VPCALL Mat_MultiplySubVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v);
	virtual void VPCALL Mat_MultiplyMat(math::matrix4x4&dst,const math::matrix4x4&m,const math::matrix4x4&v);
};

}


#endif //___SIMD_SSE___
