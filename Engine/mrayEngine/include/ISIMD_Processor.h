
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   22:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISIMD_Processor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISIMD_Processor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISIMD_Processor___
#define ___ISIMD_Processor___

#include "CompileConfig.h"
#include "mString.h"


#include "Point4d.h"

namespace mray{
namespace math{
// 	typename vector2d;
// 	typename vector3d;
//	typename vector4d;
	class matrix4x4;
}

#ifdef _WIN32
#define VPCALL __fastcall
#else
#define VPCALL
#endif

class MRAY_DLL ISIMD_Processor
{
private:
protected:

public:

	static void initSIMD();
	static void shutdownSIMD();

public:
	ISIMD_Processor(){}
	virtual~ISIMD_Processor(){}

	virtual const mchar*VPCALL getName()=0;

	virtual void VPCALL Add(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL Add(float *dst,const float*src0,const float*src1,const const int count)=0;
	virtual void VPCALL Sub(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL Sub(float *dst,const float*src0,const float*src1,const const int count)=0;
	virtual void VPCALL Mul(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL Mul(float *dst,const float*src0,const float*src1,const const int count)=0;
	virtual void VPCALL Div(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL Div(float *dst,const float*src0,const float*src1,const const int count)=0;
	virtual void VPCALL MulAdd(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL MulAdd(float *dst,const float*src0,const float*src1,const const int count)=0;
	virtual void VPCALL MulSub(float *dst,const float c,const float*src,const const int count)=0;
	virtual void VPCALL MulSub(float *dst,const float*src0,const float*src1,const const int count)=0;

	virtual void VPCALL Dot(float*dst,const math::vector3d&c,const math::vector3d*src,const int count)=0;
	virtual void VPCALL Dot(float*dst,const math::vector3d*src0,const math::vector3d*src1,const int count)=0;
	virtual void VPCALL Dot(float&dst,const float*src0,const float*src1,const int count)=0;

	virtual void VPCALL CmpGT(uchar*dst,const float c,const float*src,const int count)=0;
	virtual void VPCALL CmpGE(uchar*dst,const float c,const float*src,const int count)=0;
	virtual void VPCALL CmpLT(uchar*dst,const float c,const float*src,const int count)=0;
	virtual void VPCALL CmpLE(uchar*dst,const float c,const float*src,const int count)=0;

	virtual void VPCALL MinMax(float&min,float&max,const float*src,const int count)=0;
	virtual void VPCALL MinMax(math::vector2d&min,math::vector2d&max,const math::vector2d*src,const int count)=0;
	virtual void VPCALL MinMax(math::vector3d&min,math::vector3d&max,const math::vector3d*src,const int count)=0;
	
	virtual void VPCALL Clamp(float*dst,const float min,const float max,const float*src,const int count)=0;
	virtual void VPCALL ClampMin(float*dst,const float min,const float*src,const int count)=0;
	virtual void VPCALL ClampMax(float*dst,const float max,const float*src,const int count)=0;

	//16 bit Aligned
	virtual void VPCALL Zero16(float*dst,const int count)=0;
	virtual void VPCALL Negate16(float*dst,const int count)=0;
	virtual void VPCALL Copy16(float*dst,const float*src,const int count)=0;
	virtual void VPCALL Add16(float*dst,const float*src1,const float*src2,const int count)=0;
	virtual void VPCALL Sub16(float*dst,const float*src1,const float*src2,const int count)=0;
	virtual void VPCALL Mult16(float*dst,const float*src1,const float c,const int count)=0;

	virtual void VPCALL AddAssign16(float*dst,const float*src,const int count)=0;
	virtual void VPCALL SubAssign16(float*dst,const float*src,const int count)=0;
	virtual void VPCALL MultAssign16(float*dst,const float c,const int count)=0;

	//Matrix
	virtual void VPCALL Mat_MultiplyVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v)=0;
	virtual void VPCALL Mat_MultiplyAddVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v)=0;
	virtual void VPCALL Mat_MultiplySubVec(math::vector4d&dst,const math::matrix4x4&m,const math::vector4d&v)=0;
	virtual void VPCALL Mat_MultiplyMat(math::matrix4x4&dst,const math::matrix4x4&m,const math::matrix4x4&v)=0;
};

}


#endif //___ISIMD_Processor___
