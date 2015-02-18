#include "stdafx.h"


#include "SIMD_SSE.h"
#include "SIMD_Macros.h"




#include <assert.h>

namespace mray{


#if defined( WIN32 )

// with alignment
#define KFLOATINITS(   SRC0, COUNT, PRE, POST )				KFLOATINITDSS( SRC0,SRC0,SRC0,COUNT,PRE,POST )
#define KFLOATINITD(   DST, COUNT, PRE, POST )				KFLOATINITDSS( DST,DST,DST,COUNT,PRE,POST )
#define KFLOATINITDS(  DST, SRC0, COUNT, PRE, POST )		KFLOATINITDSS( DST,SRC0,SRC0,COUNT,PRE,POST )

#define KFLOATINITDSS( DST, SRC0, SRC1, COUNT, PRE, POST )\
__asm	mov		ecx,DST								\
__asm	shr		ecx,2								\
__asm	mov		ebx,COUNT							\
__asm	neg		ecx									\
__asm	mov		edx,SRC0							\
__asm	and		ecx,3								\
__asm	mov		esi,SRC1							\
__asm	sub		ebx,ecx								\
__asm	jge		noUnderFlow							\
__asm	xor		ebx,ebx								\
__asm	mov		ecx,COUNT							\
__asm	noUnderFlow:								\
__asm	mov		PRE,ecx								\
__asm	mov		eax,ebx								\
__asm	mov		edi,DST								\
__asm	and		eax,8-1								\
__asm	mov		POST,eax							\
__asm	and		ebx,0xfffffff8						\
__asm	jle		done								\
__asm	shl		ebx,2								\
__asm	lea		ecx,[ecx*4+ebx]						\
__asm	neg		ebx									\
__asm	add		edx,ecx								\
__asm	add		esi,ecx								\
__asm	add		edi,ecx								\
__asm	mov		eax,edx								\
__asm	or		eax,esi

// without alignment (pre==0)
#define KFLOATINITS_NA(   SRC0, COUNT, PRE, POST )				KFLOATINITDSS_NA( SRC0,SRC0,SRC0,COUNT,PRE,POST )
#define KFLOATINITD_NA(   DST, COUNT, PRE, POST )				KFLOATINITDSS_NA( DST,DST,DST,COUNT,PRE,POST )
#define KFLOATINITDS_NA(  DST, SRC0, COUNT, PRE, POST )			KFLOATINITDSS_NA( DST,SRC0,SRC0,COUNT,PRE,POST )
#define KFLOATINITDSS_NA( DST, SRC0, SRC1, COUNT, PRE, POST )\
__asm	mov		eax,COUNT							\
__asm	mov		PRE,0								\
__asm	and		eax,8-1								\
__asm	mov		ebx,COUNT							\
__asm	mov		POST,eax							\
__asm	and		ebx,0xfffffff8						\
__asm	je		done								\
__asm	shl		ebx,2								\
__asm	mov		edx,SRC0							\
__asm	mov		esi,SRC1							\
__asm	mov		edi,DST								\
__asm	add		edx,ebx								\
__asm	add		esi,ebx								\
__asm	add		edi,ebx								\
__asm	mov		eax,edx								\
__asm	or		eax,esi								\
__asm	or		eax,edi								\
__asm	neg		ebx									\

/*
when OPER is called:
edx = s0
esi	= s1
edi	= d
ebx	= index*4

xmm0 & xmm1	must not be trashed
*/
#define KMOVDS1( DST, SRC0 )							\
__asm	movss	xmm2,SRC0							\
__asm	movss	DST,xmm2
#define KMOVDS4( DST, SRC0 )							\
__asm	movups	xmm2,SRC0							\
__asm	movups	DST,xmm2
#define KMINDS1( DST, SRC0 )							\
__asm	movss	xmm2,SRC0							\
__asm	minss	DST,xmm2
#define KMAXDS1( DST, SRC0 )							\
__asm	movss	xmm2,SRC0							\
__asm	maxss	DST,xmm2

// general ALU operation
#define KALUDSS1( OP, DST, SRC0, SRC1 )					\
__asm	movss	xmm2,SRC0							\
__asm	OP##ss	xmm2,SRC1							\
__asm	movss	DST,xmm2
#define KALUDSS4( OP, DST, SRC0, SRC1 )					\
__asm	movups	xmm2,SRC0							\
__asm	movups	xmm3,SRC1							\
__asm	OP##ps	xmm2,xmm3							\
__asm	movups	DST,xmm2

#define KADDDSS1( DST, SRC0, SRC1 )		KALUDSS1( add, DST,SRC0,SRC1 )
#define KADDDSS4( DST, SRC0, SRC1 )		KALUDSS4( add, DST,SRC0,SRC1 )
#define KSUBDSS1( DST, SRC0, SRC1 )		KALUDSS1( sub, DST,SRC0,SRC1 )
#define KSUBDSS4( DST, SRC0, SRC1 )		KALUDSS4( sub, DST,SRC0,SRC1 )
#define KMULDSS1( DST, SRC0, SRC1 )		KALUDSS1( mul, DST,SRC0,SRC1 )
#define KMULDSS4( DST, SRC0, SRC1 )		KALUDSS4( mul, DST,SRC0,SRC1 )

#define KDIVDSS1( DST, SRC0, SRC1 )						\
__asm	movss	xmm2,SRC1							\
__asm	rcpss	xmm3,xmm2							\
__asm	mulss	xmm2,xmm3							\
__asm	mulss	xmm2,xmm3							\
__asm	addss	xmm3,xmm3							\
__asm	subss	xmm3,xmm2							\
__asm	mulss	xmm3,SRC0							\
__asm	movss	DST,xmm3
#define KDIVDSS4( DST, SRC0, SRC1 )						\
__asm	movups	xmm2,SRC1							\
__asm	rcpps	xmm3,xmm2							\
__asm	mulps	xmm2,xmm3							\
__asm	mulps	xmm2,xmm3							\
__asm	addps	xmm3,xmm3							\
__asm	subps	xmm3,xmm2							\
__asm	movups	xmm2,SRC0							\
__asm	mulps	xmm3,xmm2							\
__asm	movups	DST,xmm3
#define	KF2IDS1( SRC0 )									\
__asm	movss		xmm2,SRC0						\
__asm	cvttps2pi	mm2,xmm2						\
__asm	movd		[edi+ebx],mm2
#define	KF2IDS4( SRC0 )									\
__asm	movups		xmm2,SRC0						\
__asm	cvttps2pi	mm2,xmm2						\
__asm	movq		[edi+ebx+0],mm2					\
__asm	shufps		xmm2,xmm2,SHUFFLE_PS(1,0,3,2)	\
__asm	cvttps2pi	mm2,xmm2						\
__asm	movq		[edi+ebx+8],mm2
#define	KISQRTDS1( DST,SRC0 )							\
__asm	movss	xmm2,SRC0							\
__asm	rsqrtss	xmm3,xmm2							\
__asm	mulss	xmm2,xmm3							\
__asm	mulss	xmm2,xmm3							\
__asm	subss	xmm2,xmm1							\
__asm	mulss	xmm3,xmm0							\
__asm	mulss	xmm3,xmm2							\
__asm	movss	DST,xmm3
#define	KISQRTDS4( DST,SRC0 )							\
__asm	movups	xmm2,SRC0							\
__asm	rsqrtps	xmm3,xmm2							\
__asm	mulps	xmm2,xmm3							\
__asm	mulps	xmm2,xmm3							\
__asm	subps	xmm2,xmm1							\
__asm	mulps	xmm3,xmm0							\
__asm	mulps	xmm3,xmm2							\
__asm	movups	DST,xmm3

// this is used in vector4 implementation to shift constant V4
#define KANDREGDSV( DST, SRC0, VALUE )					\
__asm	mov		DST,SRC0							\
__asm	and		DST,VALUE

// this is used in vector4 code to operate with float arrays as sources
#define KEXPANDFLOAT( DST, SRC )						\
__asm	movss	DST,SRC								\
__asm	shufps  DST,DST,0

#define	KADDDS1( DST,SRC )		KADDDSS1( DST,DST,SRC )
#define	KADDDS4( DST,SRC )		KADDDSS4( DST,DST,SRC )
#define	KSUBDS1( DST,SRC )		KSUBDSS1( DST,DST,SRC )
#define	KSUBDS4( DST,SRC )		KSUBDSS4( DST,DST,SRC )
#define	KMULDS1( DST,SRC )		KMULDSS1( DST,DST,SRC )
#define	KMULDS4( DST,SRC )		KMULDSS4( DST,DST,SRC )
#define	KDIVDS1( DST,SRC )		KDIVDSS1( DST,DST,SRC )
#define	KDIVDS4( DST,SRC )		KDIVDSS4( DST,DST,SRC )

// handles pre & post leftovers
#define	KFLOATOPER( OPER, OPER4, COUNT )				\
__asm		mov		ecx,pre							\
__asm		mov		ebx,COUNT						\
__asm		cmp		ebx,ecx							\
__asm		cmovl	ecx,COUNT						\
__asm		test	ecx,ecx							\
__asm		je		preDone							\
__asm		xor		ebx,ebx							\
__asm	lpPre:										\
OPER									\
__asm		add		ebx,4							\
__asm		dec		ecx								\
__asm		jg		lpPre							\
__asm	preDone:									\
__asm		mov		ecx,post						\
__asm		mov		ebx,COUNT						\
__asm		sub		ebx,ecx							\
__asm		shl		ebx,2							\
__asm		cmp		ecx,4							\
__asm		jl		post4Done						\
OPER4									\
__asm		sub		ecx,4							\
__asm		add		ebx,4*4							\
__asm	post4Done:									\
__asm		test	ecx,ecx							\
__asm		je		postDone						\
__asm	lpPost:										\
OPER									\
__asm		add		ebx,4							\
__asm		dec		ecx								\
__asm		jg		lpPost							\
__asm	postDone:

// operate on a constant and a float array
#define KFLOAT_CA( ALUOP, DST, SRC, CONSTANT, COUNT )	\
int	pre,post;										\
__asm		movss	xmm0,CONSTANT					\
__asm		shufps	xmm0,xmm0,0						\
KFLOATINITDS( DST, SRC, COUNT, pre, post )	\
__asm		and		eax,15							\
__asm		jne		lpNA							\
__asm		jmp		lpA								\
__asm		align	16								\
__asm	lpA:										\
__asm		prefetchnta	[edx+ebx+64]				\
__asm		movaps	xmm1,xmm0						\
__asm		movaps	xmm2,xmm0						\
__asm		ALUOP##ps	xmm1,[edx+ebx]				\
__asm		ALUOP##ps	xmm2,[edx+ebx+16]			\
__asm		movaps	[edi+ebx],xmm1					\
__asm		movaps	[edi+ebx+16],xmm2				\
__asm		add		ebx,16*2						\
__asm		jl		lpA								\
__asm		jmp		done							\
__asm		align	16								\
__asm	lpNA:										\
__asm		prefetchnta	[edx+ebx+64]				\
__asm		movaps	xmm1,xmm0						\
__asm		movaps	xmm2,xmm0						\
__asm		movups	xmm3,[edx+ebx]					\
__asm		movups	xmm4,[edx+ebx+16]				\
__asm		ALUOP##ps	xmm1,xmm3					\
__asm		ALUOP##ps	xmm2,xmm4					\
__asm		movaps	[edi+ebx],xmm1					\
__asm		movaps	[edi+ebx+16],xmm2				\
__asm		add		ebx,16*2						\
__asm		jl		lpNA							\
__asm	done:										\
__asm		mov		edx,SRC							\
__asm		mov		edi,DST							\
__asm		KFLOATOPER( KALUDSS1( ALUOP, [edi+ebx],xmm0,[edx+ebx] ),	\
__asm					KALUDSS4( ALUOP, [edi+ebx],xmm0,[edx+ebx] ), COUNT )

// operate on two float arrays
#define KFLOAT_AA( ALUOP, DST, SRC0, SRC1, COUNT )		\
int	pre,post;										\
KFLOATINITDSS( DST, SRC0, SRC1, COUNT, pre, post )	\
__asm		and		eax,15							\
__asm		jne		lpNA							\
__asm		jmp		lpA								\
__asm		align	16								\
__asm	lpA:										\
__asm		movaps	xmm1,[edx+ebx]					\
__asm		movaps	xmm2,[edx+ebx+16]				\
__asm		ALUOP##ps	xmm1,[esi+ebx]				\
__asm		ALUOP##ps	xmm2,[esi+ebx+16]			\
__asm		prefetchnta	[edx+ebx+64]				\
__asm		prefetchnta	[esi+ebx+64]				\
__asm		movaps	[edi+ebx],xmm1					\
__asm		movaps	[edi+ebx+16],xmm2				\
__asm		add		ebx,16*2						\
__asm		jl		lpA								\
__asm		jmp		done							\
__asm		align	16								\
__asm	lpNA:										\
__asm		movups	xmm1,[edx+ebx]					\
__asm		movups	xmm2,[edx+ebx+16]				\
__asm		movups	xmm3,[esi+ebx]					\
__asm		movups	xmm4,[esi+ebx+16]				\
__asm		prefetchnta	[edx+ebx+64]				\
__asm		prefetchnta	[esi+ebx+64]				\
__asm		ALUOP##ps	xmm1,xmm3					\
__asm		ALUOP##ps	xmm2,xmm4					\
__asm		movaps	[edi+ebx],xmm1					\
__asm		movaps	[edi+ebx+16],xmm2				\
__asm		add		ebx,16*2						\
__asm		jl		lpNA							\
__asm	done:										\
__asm		mov		edx,SRC0						\
__asm		mov		esi,SRC1						\
__asm		mov		edi,DST							\
KFLOATOPER( KALUDSS1( ALUOP, [edi+ebx],[edx+ebx],[esi+ebx] ),		\
KALUDSS4( ALUOP, [edi+ebx],[edx+ebx],[esi+ebx] ), COUNT )


ALIGN8_INIT1( unsigned short SIMD_W_zero, 0 );
ALIGN8_INIT1( unsigned short SIMD_W_maxShort, 1<<15 );

ALIGN4_INIT4( unsigned long SIMD_SP_firstSignBit, (unsigned long) ( 1 << 31 ), 0, 0, 0 );
ALIGN4_INIT1( unsigned long SIMD_SP_signBit, (unsigned long) ( 1 << 31 ) );
ALIGN4_INIT1( unsigned long SIMD_SP_absMask, (unsigned long) ~( 1 << 31 ) );
ALIGN4_INIT1( unsigned long SIMD_SP_infinityMask, (unsigned long) ~( 1 << 23 ) );
ALIGN4_INIT1( unsigned long SIMD_SP_not, 0xFFFFFFFF );
ALIGN4_INIT4( unsigned long SIMD_SP_clearLast, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 );
ALIGN4_INIT4( unsigned long SIMD_SP_clearFirstThree, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF );

ALIGN4_INIT1( unsigned long SIMD_DW_mat2quatShuffle0, (3<<0)|(2<<8)|(1<<16)|(0<<24) );
ALIGN4_INIT1( unsigned long SIMD_DW_mat2quatShuffle1, (0<<0)|(1<<8)|(2<<16)|(3<<24) );
ALIGN4_INIT1( unsigned long SIMD_DW_mat2quatShuffle2, (1<<0)|(0<<8)|(3<<16)|(2<<24) );
ALIGN4_INIT1( unsigned long SIMD_DW_mat2quatShuffle3, (2<<0)|(3<<8)|(0<<16)|(1<<24) );

ALIGN4_INIT4( unsigned long SIMD_SP_quat2mat_x0, IEEE_SP_ZERO, IEEE_SP_SIGN, IEEE_SP_SIGN, IEEE_SP_SIGN );
ALIGN4_INIT4( unsigned long SIMD_SP_quat2mat_x1, IEEE_SP_SIGN, IEEE_SP_ZERO, IEEE_SP_SIGN, IEEE_SP_SIGN );
ALIGN4_INIT4( unsigned long SIMD_SP_quat2mat_x2, IEEE_SP_ZERO, IEEE_SP_SIGN, IEEE_SP_SIGN, IEEE_SP_SIGN );

ALIGN4_INIT1( float SIMD_SP_zero, 0.0f );
ALIGN4_INIT1( float SIMD_SP_half, 0.5f );
ALIGN4_INIT1( float SIMD_SP_one, 1.0f );
ALIGN4_INIT1( float SIMD_SP_two, 2.0f );
ALIGN4_INIT1( float SIMD_SP_three, 3.0f );
ALIGN4_INIT1( float SIMD_SP_four, 4.0f );
ALIGN4_INIT1( float SIMD_SP_maxShort, (1<<15) );
ALIGN4_INIT1( float SIMD_SP_tiny, 1e-10f );
ALIGN4_INIT1( float SIMD_SP_PI, math::PI32 );
ALIGN4_INIT1( float SIMD_SP_halfPI, math::HalfPI32 );
ALIGN4_INIT1( float SIMD_SP_twoPI, math::TwoPI32 );
ALIGN4_INIT1( float SIMD_SP_oneOverTwoPI, 1.0f / math::TwoPI32 );
ALIGN4_INIT1( float SIMD_SP_infinity, math::Infinity );
ALIGN4_INIT1( float SIMD_SP_negInfinity, -math::Infinity );
ALIGN4_INIT4( float SIMD_SP_lastOne, 0.0f, 0.0f, 0.0f, 1.0f );

ALIGN4_INIT1( float SIMD_SP_rsqrt_c0,  3.0f );
ALIGN4_INIT1( float SIMD_SP_rsqrt_c1, -0.5f );
ALIGN4_INIT1( float SIMD_SP_mat2quat_rsqrt_c1, -0.5f*0.5f );

ALIGN4_INIT1( float SIMD_SP_sin_c0, -2.39e-08f );
ALIGN4_INIT1( float SIMD_SP_sin_c1,  2.7526e-06f );
ALIGN4_INIT1( float SIMD_SP_sin_c2, -1.98409e-04f );
ALIGN4_INIT1( float SIMD_SP_sin_c3,  8.3333315e-03f );
ALIGN4_INIT1( float SIMD_SP_sin_c4, -1.666666664e-01f );

ALIGN4_INIT1( float SIMD_SP_cos_c0, -2.605e-07f );
ALIGN4_INIT1( float SIMD_SP_cos_c1,  2.47609e-05f );
ALIGN4_INIT1( float SIMD_SP_cos_c2, -1.3888397e-03f );
ALIGN4_INIT1( float SIMD_SP_cos_c3,  4.16666418e-02f );
ALIGN4_INIT1( float SIMD_SP_cos_c4, -4.999999963e-01f );

ALIGN4_INIT1( float SIMD_SP_atan_c0,  0.0028662257f );
ALIGN4_INIT1( float SIMD_SP_atan_c1, -0.0161657367f );
ALIGN4_INIT1( float SIMD_SP_atan_c2,  0.0429096138f );
ALIGN4_INIT1( float SIMD_SP_atan_c3, -0.0752896400f );
ALIGN4_INIT1( float SIMD_SP_atan_c4,  0.1065626393f );
ALIGN4_INIT1( float SIMD_SP_atan_c5, -0.1420889944f );
ALIGN4_INIT1( float SIMD_SP_atan_c6,  0.1999355085f );
ALIGN4_INIT1( float SIMD_SP_atan_c7, -0.3333314528f );

/*
============
SSE_InvSqrt
============
*/
float SSE_InvSqrt( float x ) {
	float y;

	__asm {
		movss		xmm0, x
			rsqrtss		xmm1, xmm0
			mulss		xmm0, xmm1
			mulss		xmm0, xmm1
			subss		xmm0, SIMD_SP_rsqrt_c0
			mulss		xmm1, SIMD_SP_rsqrt_c1
			mulss		xmm0, xmm1
			movss		y, xmm0
	}
	return y;
}

/*
============
SSE_InvSqrt4
============
*/
void SSE_InvSqrt4( float x[4] ) {
	__asm {
		mov			edi, x
			movaps		xmm0, [edi]
		rsqrtps		xmm1, xmm0
			mulps		xmm0, xmm1
			mulps		xmm0, xmm1
			subps		xmm0, SIMD_SP_rsqrt_c0
			mulps		xmm1, SIMD_SP_rsqrt_c1
			mulps		xmm0, xmm1
			movaps		[edi], xmm0
	}
}

/*
============
SSE_SinZeroHalfPI

The angle must be between zero and half PI.
============
*/
float SSE_SinZeroHalfPI( float a ) {
#if 1

	float t;

	assert( a >= 0.0f && a <= math::HalfPI32 );

	__asm {
		movss		xmm0, a
			movss		xmm1, xmm0
			mulss		xmm1, xmm1
			movss		xmm2, SIMD_SP_sin_c0
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c1
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c2
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c3
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c4
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_one
			mulss		xmm2, xmm0
			movss		t, xmm2
	}

	return t;

#else

	float s, t;

	assert( a >= 0.0f && a <= math::HalfPI32 );

	s = a * a;
	t = -2.39e-08f;
	t *= s;
	t += 2.7526e-06f;
	t *= s;
	t += -1.98409e-04f;
	t *= s;
	t += 8.3333315e-03f;
	t *= s;
	t += -1.666666664e-01f;
	t *= s;
	t += 1.0f;
	t *= a;

	return t;

#endif
}

/*
============
SSE_Sin4ZeroHalfPI

The angle must be between zero and half PI.
============
*/
void SSE_Sin4ZeroHalfPI( float a[4], float s[4] ) {
	__asm {
		mov			edi, a
			mov			esi, s
			movaps		xmm0, [edi]
		movaps		xmm1, xmm0
			mulps		xmm1, xmm1
			movaps		xmm2, SIMD_SP_sin_c0
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c1
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c2
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c3
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c4
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_one
			mulps		xmm2, xmm0
			movaps		[esi], xmm2
	}
}

/*
============
SSE_Sin
============
*/
float SSE_Sin( float a ) {
#if 1

	float t;

	__asm {
		movss		xmm1, a
			movss		xmm2, xmm1
			movss		xmm3, xmm1
			mulss		xmm2, SIMD_SP_oneOverTwoPI
			cvttss2si	ecx, xmm2
			cmpltss		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			cvtsi2ss	xmm2, ecx
			subss		xmm2, xmm3
			mulss		xmm2, SIMD_SP_twoPI
			subss		xmm1, xmm2

			movss		xmm0, SIMD_SP_PI			// xmm0 = PI
			subss		xmm0, xmm1					// xmm0 = PI - a
			movss		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movss		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltss	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movss		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			movss		xmm1, xmm0
			mulss		xmm1, xmm1
			movss		xmm2, SIMD_SP_sin_c0
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c1
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c2
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c3
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_sin_c4
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_one
			mulss		xmm2, xmm0
			movss		t, xmm2
	}

	return t;

#else

	float s, t;

	if ( ( a < 0.0f ) || ( a >= math::TwoPI32 ) ) {
		a -= floorf( a / math::TwoPI32 ) * math::TwoPI32;
	}

	a = math::PI - a;
	if ( fabs( a ) >= math::HalfPI32 ) {
		a = ( ( a < 0.0f ) ? -math::PI : math::PI ) - a;
	}

	s = a * a;
	t = -2.39e-08f;
	t *= s;
	t += 2.7526e-06f;
	t *= s;
	t += -1.98409e-04f;
	t *= s;
	t += 8.3333315e-03f;
	t *= s;
	t += -1.666666664e-01f;
	t *= s;
	t += 1.0f;
	t *= a;

	return t;

#endif
}

/*
============
SSE_Sin4
============
*/
void SSE_Sin4( float a[4], float s[4] ) {
	__asm {
		mov			edi, a
			mov			esi, s
			movaps		xmm1, [edi]
		movaps		xmm2, xmm1
			mulps		xmm2, SIMD_SP_oneOverTwoPI
			movhlps		xmm3, xmm2
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 0, 0, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 0, 0, 0 )
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm3, R_SHUFFLE_PS( 1, 0, 1, 0 )
			movaps		xmm3, xmm1
			cmpltps		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			subps		xmm2, xmm3
			mulps		xmm2, SIMD_SP_twoPI
			subps		xmm1, xmm2

			movaps		xmm0, SIMD_SP_PI			// xmm0 = PI
			subps		xmm0, xmm1					// xmm0 = PI - a
			movaps		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movaps		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltps	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movaps		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			movaps		xmm1, xmm0
			mulps		xmm1, xmm1
			movaps		xmm2, SIMD_SP_sin_c0
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c1
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c2
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c3
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_sin_c4
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_one
			mulps		xmm2, xmm0
			movaps		[esi], xmm2
	}
}

/*
============
SSE_CosZeroHalfPI

The angle must be between zero and half PI.
============
*/
float SSE_CosZeroHalfPI( float a ) {
#if 1

	float t;

	assert( a >= 0.0f && a <= math::HalfPI32 );

	__asm {
		movss		xmm0, a
			mulss		xmm0, xmm0
			movss		xmm1, SIMD_SP_cos_c0
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c1
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c2
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c3
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c4
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_one
			movss		t, xmm1
	}

	return t;

#else

	float s, t;

	assert( a >= 0.0f && a <= math::HalfPI32 );

	s = a * a;
	t = -2.605e-07f;
	t *= s;
	t += 2.47609e-05f;
	t *= s;
	t += -1.3888397e-03f;
	t *= s;
	t += 4.16666418e-02f;
	t *= s;
	t += -4.999999963e-01f;
	t *= s;
	t += 1.0f;

	return t;

#endif
}

/*
============
SSE_Cos4ZeroHalfPI

The angle must be between zero and half PI.
============
*/
void SSE_Cos4ZeroHalfPI( float a[4], float c[4] ) {
	__asm {
		mov			edi, a
			mov			esi, c
			movaps		xmm0, [edi]
		mulps		xmm0, xmm0
			movaps		xmm1, SIMD_SP_cos_c0
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c1
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c2
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c3
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c4
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_one
			movaps		[esi], xmm2
	}
}

/*
============
SSE_Cos
============
*/
float SSE_Cos( float a ) {
#if 1

	float t;

	__asm {
		movss		xmm1, a
			movss		xmm2, xmm1
			movss		xmm3, xmm1
			mulss		xmm2, SIMD_SP_oneOverTwoPI
			cvttss2si	ecx, xmm2
			cmpltss		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			cvtsi2ss	xmm2, ecx
			subss		xmm2, xmm3
			mulss		xmm2, SIMD_SP_twoPI
			subss		xmm1, xmm2

			movss		xmm0, SIMD_SP_PI			// xmm0 = PI
			subss		xmm0, xmm1					// xmm0 = PI - a
			movss		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movss		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltss	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movss		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			mulss		xmm0, xmm0
			movss		xmm1, SIMD_SP_cos_c0
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c1
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c2
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c3
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_cos_c4
			mulss		xmm1, xmm0
			addss		xmm1, SIMD_SP_one
			xorps		xmm2, SIMD_SP_signBit
			xorps		xmm1, xmm2
			movss		t, xmm1
	}

	return t;

#else

	float s, t;

	if ( ( a < 0.0f ) || ( a >= math::TwoPI32 ) ) {
		a -= floorf( a / math::TwoPI32 ) * math::TwoPI32;
	}

	a = math::PI - a;
	if ( fabs( a ) >= math::HalfPI32 ) {
		a = ( ( a < 0.0f ) ? -math::PI : math::PI ) - a;
		d = 1.0f;
	} else {
		d = -1.0f;
	}

	s = a * a;
	t = -2.605e-07f;
	t *= s;
	t += 2.47609e-05f;
	t *= s;
	t += -1.3888397e-03f;
	t *= s;
	t += 4.16666418e-02f;
	t *= s;
	t += -4.999999963e-01f;
	t *= s;
	t += 1.0f;
	t *= d;

	return t;

#endif
}

/*
============
SSE_Cos4
============
*/
void SSE_Cos4( float a[4], float c[4] ) {
	__asm {
		mov			edi, a
			mov			esi, c
			movaps		xmm1, [edi]
		movaps		xmm2, xmm1
			mulps		xmm2, SIMD_SP_oneOverTwoPI
			movhlps		xmm3, xmm2
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 0, 0, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 0, 0, 0 )
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm3, R_SHUFFLE_PS( 1, 0, 1, 0 )
			movaps		xmm3, xmm1
			cmpltps		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			subps		xmm2, xmm3
			mulps		xmm2, SIMD_SP_twoPI
			subps		xmm1, xmm2

			movaps		xmm0, SIMD_SP_PI			// xmm0 = PI
			subps		xmm0, xmm1					// xmm0 = PI - a
			movaps		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movaps		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltps	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movaps		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			mulps		xmm0, xmm0
			movaps		xmm1, SIMD_SP_cos_c0
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c1
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c2
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c3
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_cos_c4
			mulps		xmm1, xmm0
			addps		xmm1, SIMD_SP_one
			xorps		xmm2, SIMD_SP_signBit
			xorps		xmm1, xmm2
			movaps		[esi], xmm1
	}
}

/*
============
SSE_SinCos
============
*/
void SSE_SinCos( float a, float &s, float &c ) {
	__asm {
		mov			edi, s
			mov			esi, c
			movss		xmm1, a
			movss		xmm2, xmm1
			movss		xmm3, xmm1
			mulss		xmm2, SIMD_SP_oneOverTwoPI
			cvttss2si	ecx, xmm2
			cmpltss		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			cvtsi2ss	xmm2, ecx
			subss		xmm2, xmm3
			mulss		xmm2, SIMD_SP_twoPI
			subss		xmm1, xmm2

			movss		xmm0, SIMD_SP_PI			// xmm0 = PI
			subss		xmm0, xmm1					// xmm0 = PI - a
			movss		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movss		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltss	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movss		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			movss		xmm1, xmm0
			mulss		xmm1, xmm1
			movss		xmm3, SIMD_SP_sin_c0
			movss		xmm4, SIMD_SP_cos_c0
			mulss		xmm3, xmm1
			mulss		xmm4, xmm1
			addss		xmm3, SIMD_SP_sin_c1
			addss		xmm4, SIMD_SP_cos_c1
			mulss		xmm3, xmm1
			mulss		xmm4, xmm1
			addss		xmm3, SIMD_SP_sin_c2
			addss		xmm4, SIMD_SP_cos_c2
			mulss		xmm3, xmm1
			mulss		xmm4, xmm1
			addss		xmm3, SIMD_SP_sin_c3
			addss		xmm4, SIMD_SP_cos_c3
			mulss		xmm3, xmm1
			mulss		xmm4, xmm1
			addss		xmm3, SIMD_SP_sin_c4
			addss		xmm4, SIMD_SP_cos_c4
			mulss		xmm3, xmm1
			mulss		xmm4, xmm1
			addss		xmm3, SIMD_SP_one
			addss		xmm4, SIMD_SP_one
			mulss		xmm3, xmm0
			xorps		xmm2, SIMD_SP_signBit
			xorps		xmm4, xmm2
			movss		[edi], xmm2
			movss		[esi], xmm3
	}
}

/*
============
SSE_SinCos4
============
*/
void SSE_SinCos4( float a[4], float s[4], float c[4] ) {
	__asm {
		mov			eax, a
			mov			edi, s
			mov			esi, c
			movaps		xmm1, [eax]
		movaps		xmm2, xmm1
			mulps		xmm2, SIMD_SP_oneOverTwoPI
			movhlps		xmm3, xmm2
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 0, 0, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 0, 0, 0 )
			cvttss2si	ecx, xmm2
			cvtsi2ss	xmm2, ecx
			cvttss2si	edx, xmm3
			cvtsi2ss	xmm3, edx
			shufps		xmm2, xmm3, R_SHUFFLE_PS( 1, 0, 1, 0 )
			movaps		xmm3, xmm1
			cmpltps		xmm3, SIMD_SP_zero
			andps		xmm3, SIMD_SP_one
			subps		xmm2, xmm3
			mulps		xmm2, SIMD_SP_twoPI
			subps		xmm1, xmm2

			movaps		xmm0, SIMD_SP_PI			// xmm0 = PI
			subps		xmm0, xmm1					// xmm0 = PI - a
			movaps		xmm1, xmm0					// xmm1 = PI - a
			andps		xmm1, SIMD_SP_signBit		// xmm1 = signbit( PI - a )
			movaps		xmm2, xmm0					// xmm2 = PI - a
			xorps		xmm2, xmm1					// xmm2 = fabs( PI - a )
			cmpnltps	xmm2, SIMD_SP_halfPI		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? 0xFFFFFFFF : 0x00000000
			movaps		xmm3, SIMD_SP_PI			// xmm3 = PI
			xorps		xmm3, xmm1					// xmm3 = PI ^ signbit( PI - a )
			andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= math::HalfPI32 ) ? ( PI ^ signbit( PI - a ) ) : 0.0f
			andps		xmm2, SIMD_SP_signBit		// xmm2 = ( fabs( PI - a ) >= math::HalfPI32 ) ? SIMD_SP_signBit : 0.0f
			xorps		xmm0, xmm2
			addps		xmm0, xmm3

			movaps		xmm0, [eax]
		movaps		xmm1, xmm0
			mulps		xmm1, xmm1
			movaps		xmm3, SIMD_SP_sin_c0
			movaps		xmm4, SIMD_SP_cos_c0
			mulps		xmm3, xmm1
			mulps		xmm4, xmm1
			addps		xmm3, SIMD_SP_sin_c1
			addps		xmm4, SIMD_SP_cos_c1
			mulps		xmm3, xmm1
			mulps		xmm4, xmm1
			addps		xmm3, SIMD_SP_sin_c2
			addps		xmm4, SIMD_SP_cos_c2
			mulps		xmm3, xmm1
			mulps		xmm4, xmm1
			addps		xmm3, SIMD_SP_sin_c3
			addps		xmm4, SIMD_SP_cos_c3
			mulps		xmm3, xmm1
			mulps		xmm4, xmm1
			addps		xmm3, SIMD_SP_sin_c4
			addps		xmm4, SIMD_SP_cos_c4
			mulps		xmm3, xmm1
			mulps		xmm4, xmm1
			addps		xmm3, SIMD_SP_one
			addps		xmm4, SIMD_SP_one
			mulps		xmm3, xmm0
			xorps		xmm2, SIMD_SP_signBit
			xorps		xmm4, xmm2
			movaps		[edi], xmm3
			movaps		[esi], xmm4
	}
}

/*
============
SSE_ATanPositive

Both 'x' and 'y' must be positive.
============
*/
float SSE_ATanPositive( float y, float x ) {
#if 1

	float t;

	assert( y >= 0.0f && x >= 0.0f );

	__asm {
		movss		xmm0, x
			movss		xmm3, xmm0
			movss		xmm1, y
			minss		xmm0, xmm1
			maxss		xmm1, xmm3
			cmpeqss		xmm3, xmm0
			rcpss		xmm2, xmm1
			mulss		xmm1, xmm2
			mulss		xmm1, xmm2
			addss		xmm2, xmm2
			subss		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
			mulss		xmm0, xmm2				// xmm0 = x / y or y / x
			movss		xmm1, xmm3
			andps		xmm1, SIMD_SP_signBit
			xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
			andps		xmm3, SIMD_SP_halfPI	// xmm3 = HalfPI32 or 0.0f
			movss		xmm1, xmm0
			mulss		xmm1, xmm1				// xmm1 = s
			movss		xmm2, SIMD_SP_atan_c0
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c1
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c2
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c3
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c4
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c5
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c6
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c7
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_one
			mulss		xmm2, xmm0
			addss		xmm2, xmm3
			movss		t, xmm2
	}

	return t;

#else

	float a, d, s, t;

	assert( y >= 0.0f && x >= 0.0f );

	if ( y > x ) {
		a = -x / y;
		d = math::HalfPI32;
	} else {
		a = y / x;
		d = 0.0f;
	}
	s = a * a;
	t = 0.0028662257f;
	t *= s;
	t += -0.0161657367f;
	t *= s;
	t += 0.0429096138f;
	t *= s;
	t += -0.0752896400f;
	t *= s;
	t += 0.1065626393f;
	t *= s;
	t += -0.1420889944f;
	t *= s;
	t += 0.1999355085f;
	t *= s;
	t += -0.3333314528f;
	t *= s;
	t += 1.0f;
	t *= a;
	t += d;

	return t;

#endif
}

/*
============
SSE_ATan4Positive

Both 'x' and 'y' must be positive.
============
*/
void SSE_ATan4Positive( float y[4], float x[4], float at[4] ) {
	__asm {
		mov			esi, x
			mov			edi, y
			mov			edx, at
			movaps		xmm0, [esi]
		movaps		xmm3, xmm0
			movaps		xmm1, [edi]
		minps		xmm0, xmm1
			maxps		xmm1, xmm3
			cmpeqps		xmm3, xmm0
			rcpps		xmm2, xmm1
			mulps		xmm1, xmm2
			mulps		xmm1, xmm2
			addps		xmm2, xmm2
			subps		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
			mulps		xmm0, xmm2				// xmm0 = x / y or y / x
			movaps		xmm1, xmm3
			andps		xmm1, SIMD_SP_signBit
			xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
			andps		xmm3, SIMD_SP_halfPI	// xmm3 = HalfPI32 or 0.0f
			movaps		xmm1, xmm0
			mulps		xmm1, xmm1				// xmm1 = s
			movaps		xmm2, SIMD_SP_atan_c0
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c1
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c2
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c3
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c4
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c5
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c6
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c7
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_one
			mulps		xmm2, xmm0
			addps		xmm2, xmm3
			movaps		[edx], xmm2
	}
}

/*
============
SSE_ATan
============
*/
float SSE_ATan( float y, float x ) {
#if 1

	float t;

	__asm {
		movss		xmm0, x
			movss		xmm3, xmm0
			movss		xmm4, xmm0
			andps		xmm0, SIMD_SP_absMask
			movss		xmm1, y
			xorps		xmm4, xmm1
			andps		xmm1, SIMD_SP_absMask
			andps		xmm4, SIMD_SP_signBit
			minss		xmm0, xmm1
			maxss		xmm1, xmm3
			cmpeqss		xmm3, xmm0
			rcpss		xmm2, xmm1
			mulss		xmm1, xmm2
			mulss		xmm1, xmm2
			addss		xmm2, xmm2
			subss		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
			mulss		xmm0, xmm2				// xmm0 = x / y or y / x
			xorps		xmm0, xmm4
			movss		xmm1, xmm3
			andps		xmm1, SIMD_SP_signBit
			xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
			orps		xmm4, SIMD_SP_halfPI	// xmm4 = +/- HalfPI32
			andps		xmm3, xmm4				// xmm3 = +/- HalfPI32 or 0.0f
			movss		xmm1, xmm0
			mulss		xmm1, xmm1				// xmm1 = s
			movss		xmm2, SIMD_SP_atan_c0
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c1
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c2
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c3
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c4
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c5
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c6
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_atan_c7
			mulss		xmm2, xmm1
			addss		xmm2, SIMD_SP_one
			mulss		xmm2, xmm0
			addss		xmm2, xmm3
			movss		t, xmm2
	}

	return t;

#else

	float a, d, s, t;

	if ( fabs( y ) > fabs( x ) ) {
		a = -x / y;
		d = math::HalfPI32;
		*((unsigned long *)&d) ^= ( *((unsigned long *)&x) ^ *((unsigned long *)&y) ) & (1<<31);
	} else {
		a = y / x;
		d = 0.0f;
	}

	s = a * a;
	t = 0.0028662257f;
	t *= s;
	t += -0.0161657367f;
	t *= s;
	t += 0.0429096138f;
	t *= s;
	t += -0.0752896400f;
	t *= s;
	t += 0.1065626393f;
	t *= s;
	t += -0.1420889944f;
	t *= s;
	t += 0.1999355085f;
	t *= s;
	t += -0.3333314528f;
	t *= s;
	t += 1.0f;
	t *= a;
	t += d;

	return t;

#endif
}

/*
============
SSE_ATan4
============
*/
void SSE_ATan4( float y[4], float x[4], float at[4] ) {
	__asm {
		mov			esi, x
			mov			edi, y
			mov			edx, at
			movaps		xmm0, [esi]
		movaps		xmm3, xmm0
			movaps		xmm4, xmm0
			andps		xmm0, SIMD_SP_absMask
			movaps		xmm1, [edi]
		xorps		xmm4, xmm1
			andps		xmm1, SIMD_SP_absMask
			andps		xmm4, SIMD_SP_signBit
			minps		xmm0, xmm1
			maxps		xmm1, xmm3
			cmpeqps		xmm3, xmm0
			rcpps		xmm2, xmm1
			mulps		xmm1, xmm2
			mulps		xmm1, xmm2
			addps		xmm2, xmm2
			subps		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
			mulps		xmm0, xmm2				// xmm0 = x / y or y / x
			xorps		xmm0, xmm4
			movaps		xmm1, xmm3
			andps		xmm1, SIMD_SP_signBit
			xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
			orps		xmm4, SIMD_SP_halfPI	// xmm4 = +/- HalfPI32
			andps		xmm3, xmm4				// xmm3 = +/- HalfPI32 or 0.0f
			movaps		xmm1, xmm0
			mulps		xmm1, xmm1				// xmm1 = s
			movaps		xmm2, SIMD_SP_atan_c0
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c1
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c2
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c3
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c4
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c5
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c6
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_atan_c7
			mulps		xmm2, xmm1
			addps		xmm2, SIMD_SP_one
			mulps		xmm2, xmm0
			addps		xmm2, xmm3
			movaps		[edx], xmm2
	}
}

/*
============
SIMD_SSE::GetName
============
*/
const mchar * SIMD_SSE::getName( void ) {
	return mT("SSE");
}

/*
============
SIMD_SSE::Add

dst[i] = constant + src[i];
============
*/
void SIMD_SSE::Add( float *dst, const float constant, const float *src, const int count ) {
	KFLOAT_CA( add, dst, src, constant, count )
}

/*
============
SIMD_SSE::Add

dst[i] = src0[i] + src1[i];
============
*/
void SIMD_SSE::Add( float *dst, const float *src0, const float *src1, const int count ) {
	KFLOAT_AA( add, dst, src0, src1, count )
}

/*
============
SIMD_SSE::Sub

dst[i] = constant - src[i];
============
*/
void SIMD_SSE::Sub( float *dst, const float constant, const float *src, const int count ) {
	KFLOAT_CA( sub, dst, src, constant, count )
}

/*
============
SIMD_SSE::Sub

dst[i] = src0[i] - src1[i];
============
*/
void SIMD_SSE::Sub( float *dst, const float *src0, const float *src1, const int count ) {
	KFLOAT_AA( sub, dst, src0, src1, count )
}

/*
============
SIMD_SSE::Mul

dst[i] = constant * src[i];
============
*/
void SIMD_SSE::Mul( float *dst, const float constant, const float *src, const int count ) {
	KFLOAT_CA( mul, dst, src, constant, count )
}

/*
============
SIMD_SSE::Mul

dst[i] = src0[i] * src1[i];
============
*/
void SIMD_SSE::Mul( float *dst, const float *src0, const float *src1, const int count ) {
	KFLOAT_AA( mul, dst, src0, src1, count )
}

/*
============
SIMD_SSE::Div

dst[i] = constant / src[i];
============
*/
void SIMD_SSE::Div( float *dst, const float constant, const float *src, const int count ) {
	int pre, post;

	//	1 / x = 2 * rcpps(x) - (x * rcpps(x) * rcpps(x));
	__asm
	{
		movss	xmm1,constant
			shufps	xmm1,xmm1,0

			KFLOATINITDS( dst, src, count, pre, post )
			and		eax,15
			jne		lpNA
			jmp		lpA
			align	16
lpA:
		movaps	xmm2,[edx+ebx]
		movaps	xmm3,[edx+ebx+16]
		rcpps	xmm4,xmm2
			rcpps	xmm5,xmm3
			prefetchnta	[edx+ebx+64]
		mulps	xmm2,xmm4
			mulps	xmm2,xmm4
			mulps	xmm3,xmm5
			mulps	xmm3,xmm5
			addps	xmm4,xmm4
			addps	xmm5,xmm5
			subps	xmm4,xmm2
			subps	xmm5,xmm3
			mulps	xmm4,xmm1
			mulps	xmm5,xmm1
			movaps	[edi+ebx],xmm4
			movaps	[edi+ebx+16],xmm5
			add		ebx,16*2
			jl		lpA
			jmp		done
			align	16
lpNA:
		movups	xmm2,[edx+ebx]
		movups	xmm3,[edx+ebx+16]
		rcpps	xmm4,xmm2
			rcpps	xmm5,xmm3
			prefetchnta	[edx+ebx+64]
		mulps	xmm2,xmm4
			mulps	xmm2,xmm4
			mulps	xmm3,xmm5
			mulps	xmm3,xmm5
			addps	xmm4,xmm4
			addps	xmm5,xmm5
			subps	xmm4,xmm2
			subps	xmm5,xmm3
			mulps	xmm4,xmm1
			mulps	xmm5,xmm1
			movaps	[edi+ebx],xmm4
			movaps	[edi+ebx+16],xmm5
			add		ebx,16*2
			jl		lpNA
done:
		mov		edx,src
			mov		edi,dst
			KFLOATOPER( KDIVDSS1( [edi+ebx],xmm1,[edx+ebx] ),
			KDIVDSS4( [edi+ebx],xmm1,[edx+ebx] ), count )
	}
}

/*
============
SIMD_SSE::Div

dst[i] = src0[i] / src1[i];
============
*/
void SIMD_SSE::Div( float *dst, const float *src0, const float *src1, const int count ) {
	int		pre,post;

	//	1 / x = 2 * rcpps(x) - (x * rcpps(x) * rcpps(x));
	__asm
	{
		KFLOATINITDSS( dst, src0, src1, count, pre, post )
			and		eax,15
			jne		lpNA
			jmp		lpA
			align	16
lpA:
		movaps	xmm2,[esi+ebx]
		movaps	xmm3,[esi+ebx+16]
		rcpps	xmm4,xmm2
			rcpps	xmm5,xmm3
			prefetchnta	[esi+ebx+64]
		mulps	xmm2,xmm4
			mulps	xmm2,xmm4
			mulps	xmm3,xmm5
			mulps	xmm3,xmm5
			addps	xmm4,xmm4
			addps	xmm5,xmm5
			subps	xmm4,xmm2
			subps	xmm5,xmm3
			mulps	xmm4,[edx+ebx]
		mulps	xmm5,[edx+ebx+16]
		movaps	[edi+ebx],xmm4
			movaps	[edi+ebx+16],xmm5
			add		ebx,16*2
			jl		lpA
			jmp		done
			align	16
lpNA:
		movups	xmm2,[esi+ebx]
		movups	xmm3,[esi+ebx+16]
		rcpps	xmm4,xmm2
			rcpps	xmm5,xmm3
			prefetchnta	[esi+ebx+64]
		mulps	xmm2,xmm4
			mulps	xmm2,xmm4
			mulps	xmm3,xmm5
			mulps	xmm3,xmm5
			addps	xmm4,xmm4
			addps	xmm5,xmm5
			subps	xmm4,xmm2
			subps	xmm5,xmm3
			movups	xmm2,[edx+ebx]
		movups	xmm3,[edx+ebx+16]
		mulps	xmm4,xmm2
			mulps	xmm5,xmm3
			movaps	[edi+ebx],xmm4
			movaps	[edi+ebx+16],xmm5
			add		ebx,16*2
			jl		lpNA
done:
		mov		edx,src0
			mov		esi,src1
			mov		edi,dst
			KFLOATOPER( KDIVDSS1( [edi+ebx],[edx+ebx],[esi+ebx] ),
			KDIVDSS4( [edi+ebx],[edx+ebx],[esi+ebx] ), count )
	}
}
/*
============
Simd_MulAdd

assumes count >= 7
============
*/
static void Simd_MulAdd( float *dst, const float constant, const float *src, const int count ) {
	__asm	mov			esi, dst
		__asm	mov			edi, src
		__asm	mov			eax, count
		__asm	shl			eax, 2
		__asm	mov			ecx, esi
		__asm	mov			edx, eax
		__asm	or			ecx, edi
		__asm	fld			constant
		__asm	and			ecx, 15
		__asm	jz			SimdMulAdd16
		__asm	and			ecx, 3
		__asm	jnz			SimdMulAdd8
		__asm	mov			ecx, esi
		__asm	xor			ecx, edi
		__asm	and			ecx, 15
		__asm	jnz			MulAdd8
		__asm	mov			ecx, esi
		__asm	and			ecx, 15
		__asm	neg			ecx
		__asm	add			ecx, 16
		__asm	sub			eax, ecx
		__asm	add			edi, ecx
		__asm	add			esi, ecx
		__asm	neg			ecx
		__asm	mov			edx, eax
		__asm loopPreMulAdd16:
	__asm	fld			st
		__asm	fmul		dword ptr [edi+ecx]
	__asm	fadd		dword ptr [esi+ecx]
	__asm	fstp		dword ptr [esi+ecx]
	__asm	add			ecx, 4
		__asm	jl			loopPreMulAdd16
		__asm SimdMulAdd16:
	__asm	and			eax, ~15
		__asm	movss		xmm1, constant
		__asm	shufps		xmm1, xmm1, 0x00
		__asm	add			esi, eax
		__asm	add			edi, eax
		__asm	neg			eax
		__asm	align		16
		__asm loopMulAdd16:
	__asm	movaps		xmm0, [edi+eax]
	__asm	mulps		xmm0, xmm1
		__asm	addps		xmm0, [esi+eax]
	__asm	movaps		[esi+eax], xmm0
		__asm	add			eax, 16
		__asm	jl			loopMulAdd16
		__asm	jmp			postMulAdd
		__asm MulAdd8:
	__asm	mov			ecx, esi
		__asm	and			ecx, 7
		__asm	jz			SimdMulAdd8
		__asm	sub			eax, ecx
		__asm	add			esi, ecx
		__asm	add			edi, ecx
		__asm	neg			ecx
		__asm	mov			edx, eax
		__asm loopPreMulAdd8:
	__asm	fld			st
		__asm	fmul		dword ptr [edi+ecx]
	__asm	fadd		dword ptr [esi+ecx]
	__asm	fstp		dword ptr [esi+ecx]
	__asm	add			ecx, 4
		__asm	jl			loopPreMulAdd8
		__asm SimdMulAdd8:
	__asm	and			eax, ~15
		__asm	movss		xmm1, constant
		__asm	shufps		xmm1, xmm1, 0x00
		__asm	add			esi, eax
		__asm	add			edi, eax
		__asm	neg			eax
		__asm	align		16
		__asm loopMulAdd8:
	__asm	movlps		xmm0, [edi+eax]
	__asm	movhps		xmm0, [edi+eax+8]
	__asm	mulps		xmm0, xmm1
		__asm	movlps		xmm2, [esi+eax]
	__asm	movhps		xmm2, [esi+eax+8]
	__asm	addps		xmm0, xmm2
		__asm	movlps		[esi+eax], xmm0
		__asm	movhps		[esi+eax+8], xmm0
		__asm	add			eax, 16
		__asm	jl			loopMulAdd8
		__asm	jmp			postMulAdd
		__asm postMulAdd:
	__asm	and			edx, 15
		__asm	jz			MulAddDone
		__asm	add			esi, edx
		__asm	add			edi, edx
		__asm	neg			edx
		__asm loopPostMulAdd:
	__asm	fld			st
		__asm	fmul		dword ptr [edi+edx]
	__asm	fadd		dword ptr [esi+edx]
	__asm	fstp		dword ptr [esi+edx]
	__asm	add			edx, 4
		__asm	jl			loopPostMulAdd
		__asm MulAddDone:
	__asm	fstp		st
}

#define MULADD_FEW( OPER )																				\
switch( count ) {																						\
case 0:																								\
return;																							\
case 1:																								\
dst[0] OPER c * src[0];																			\
return;																							\
case 2:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1];													\
return;																							\
case 3:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2];							\
return;																							\
case 4:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
return;																							\
case 5:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4];																			\
return;																							\
case 6:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5];													\
return;																							\
case 7:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5]; dst[6] OPER c * src[6];							\
return;																							\
case 8:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5]; dst[6] OPER c * src[6]; dst[7] OPER c * src[7];	\
return;																							\
case 9:																								\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5]; dst[6] OPER c * src[6]; dst[7] OPER c * src[7];	\
dst[8] OPER c * src[8];																			\
return;																							\
case 10:																							\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5]; dst[6] OPER c * src[6]; dst[7] OPER c * src[7];	\
dst[8] OPER c * src[8]; dst[9] OPER c * src[9];													\
return;																							\
case 11:																							\
dst[0] OPER c * src[0]; dst[1] OPER c * src[1]; dst[2] OPER c * src[2]; dst[3] OPER c * src[3];	\
dst[4] OPER c * src[4]; dst[5] OPER c * src[5]; dst[6] OPER c * src[6]; dst[7] OPER c * src[7];	\
dst[8] OPER c * src[8]; dst[9] OPER c * src[9]; dst[10] OPER c * src[10];						\
return;																							\
	}

/*
============
SIMD_SSE::MulAdd

dst[i] += constant * src[i];
============
*/
void SIMD_SSE::MulAdd( float *dst, const float constant, const float *src, const int count ) {
	float c = constant;
	MULADD_FEW( += )
		Simd_MulAdd( dst, constant, src, count );
}

/*
============
SIMD_SSE::MulAdd

dst[i] += src0[i] * src1[i];
============
*/
void SIMD_SSE::MulAdd( float *dst, const float *src0, const float *src1, const int count ) {
	for ( int i = 0; i < count; i++ ) {
		dst[i] += src0[i] + src1[i];
	}
}

/*
============
SIMD_SSE::MulSub

dst[i] -= constant * src[i];
============
*/
void SIMD_SSE::MulSub( float *dst, const float constant, const float *src, const int count ) {
	float c = constant;
	MULADD_FEW( -= )
		Simd_MulAdd( dst, -constant, src, count );
}

/*
============
SIMD_SSE::MulSub

dst[i] -= src0[i] * src1[i];
============
*/
void SIMD_SSE::MulSub( float *dst, const float *src0, const float *src1, const int count ) {
	for ( int i = 0; i < count; i++ ) {
		dst[i] -= src0[i] + src1[i];
	}
}

/*
============
SIMD_SSE::Dot

dst[i] = constant * src[i];
============
*/
void SIMD_SSE::Dot( float *dst, const math::vector3d &constant, const math::vector3d *src, const int count ) {
	__asm
	{
		mov			eax, count
			mov			edi, constant
			mov			edx, eax
			mov			esi, src
			mov			ecx, dst
			and			eax, ~3

			movss		xmm4, [edi+0]
		shufps		xmm4, xmm4, R_SHUFFLE_PS( 0, 0, 0, 0 )
			movss		xmm5, [edi+4]
		shufps		xmm5, xmm5, R_SHUFFLE_PS( 0, 0, 0, 0 )
			movss		xmm6, [edi+8]
		shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 0, 0, 0 )

			jz			done4
			imul		eax, 12
			add			esi, eax
			neg			eax

loop4:
		movlps		xmm1, [esi+eax+ 0]
		movlps		xmm2, [esi+eax+ 8]
		movlps		xmm3, [esi+eax+16]
		movhps		xmm1, [esi+eax+24]
		movhps		xmm2, [esi+eax+32]
		movhps		xmm3, [esi+eax+40]
		movaps		xmm0, xmm1
			shufps		xmm0, xmm2, R_SHUFFLE_PS( 0, 2, 1, 3 )
			shufps		xmm1, xmm3, R_SHUFFLE_PS( 1, 3, 0, 2 )
			shufps		xmm2, xmm3, R_SHUFFLE_PS( 0, 2, 1, 3 )
			add			ecx, 16
			add			eax, 4*12
			mulps		xmm0, xmm4
			mulps		xmm1, xmm5
			mulps		xmm2, xmm6
			addps		xmm0, xmm1
			addps		xmm0, xmm2
			shufps		xmm0, xmm0, R_SHUFFLE_PS( 0, 2, 1, 3 )
			movlps		[ecx-16+0], xmm0
			movhps		[ecx-16+8], xmm0
			jl			loop4

done4:
		and			edx, 3
			jz			done1

loop1:
		movss		xmm0, [esi+eax+0]
		movss		xmm1, [esi+eax+4]
		movss		xmm2, [esi+eax+8]
		mulss		xmm0, xmm4
			mulss		xmm1, xmm5
			mulss		xmm2, xmm6
			add			ecx, 4
			addss		xmm0, xmm1
			add			eax, 12
			addss		xmm0, xmm2
			dec			edx
			movss		[ecx-4], xmm0
			jnz			loop1

done1:
	}
}

/*
============
SIMD_SSE::Dot

dst[i] = src0[i] * src1[i];
============
*/
void SIMD_SSE::Dot( float *dst, const math::vector3d *src0, const math::vector3d *src1, const int count ) {
	__asm
	{
		mov			eax, count
			mov			edi, src0
			mov			edx, eax
			mov			esi, src1
			mov			ecx, dst
			and			eax, ~3

			jz			done4
			imul		eax, 12
			add			edi, eax
			add			esi, eax
			neg			eax

loop4:
		movlps		xmm0, [esi+eax]						// 0, 1, X, X
		movlps		xmm3, [edi+eax]						// 0, 1, X, X
		movlps		xmm1, [esi+eax+8]					// 2, 3, X, X
		movlps		xmm4, [edi+eax+8]					// 2, 3, X, X
		movhps		xmm0, [esi+eax+24]					// 0, 1, 6, 7
		movhps		xmm3, [edi+eax+24]					// 0, 1, 6, 7
		movhps		xmm1, [esi+eax+32]					// 2, 3, 8, 9
		movhps		xmm4, [edi+eax+32]					// 2, 3, 8, 9
		movlps		xmm2, [esi+eax+16]					// 4, 5, X, X
		movlps		xmm5, [edi+eax+16]					// 4, 5, X, X
		movhps		xmm2, [esi+eax+40]					// 4, 5, 10, 11
		movhps		xmm5, [edi+eax+40]					// 4, 5, 10, 11

		add			ecx, 16
			add			eax, 48

			mulps		xmm0, xmm3
			mulps		xmm1, xmm4
			mulps		xmm2, xmm5
			movaps		xmm7, xmm0
			shufps		xmm7, xmm1, R_SHUFFLE_PS( 0, 2, 1, 3 )	// 0, 6, 3, 9
			shufps		xmm0, xmm2, R_SHUFFLE_PS( 1, 3, 0, 2 )	// 1, 7, 4, 10
			shufps		xmm1, xmm2, R_SHUFFLE_PS( 0, 2, 1, 3 )	// 2, 8, 5, 11
			addps		xmm7, xmm0
			addps		xmm7, xmm1
			shufps		xmm7, xmm7, R_SHUFFLE_PS( 0, 2, 1, 3 )

			movlps		[ecx-16+0], xmm7
			movhps		[ecx-16+8], xmm7
			jl			loop4

done4:
		and			edx, 3
			jz			done1

loop1:
		movss		xmm0, [esi+eax+0]
		movss		xmm3, [edi+eax+0]
		movss		xmm1, [esi+eax+4]
		movss		xmm4, [edi+eax+4]
		movss		xmm2, [esi+eax+8]
		movss		xmm5, [edi+eax+8]
		mulss		xmm0, xmm3
			mulss		xmm1, xmm4
			mulss		xmm2, xmm5
			add			ecx, 4
			addss		xmm0, xmm1
			add			eax, 12
			addss		xmm0, xmm2
			dec			edx
			movss		[ecx-4], xmm0
			jnz			loop1

done1:
	}
}

/*
============
SIMD_SSE::Dot

dot = src1[0] * src2[0] + src1[1] * src2[1] + src1[2] * src2[2] + ...
============
*/
void SIMD_SSE::Dot( float &dot, const float *src1, const float *src2, const int count ) {
	switch( count ) {
case 0:
	dot = 0.0f;
	return;
case 1:
	dot = src1[0] * src2[0];
	return;
case 2:
	dot = src1[0] * src2[0] + src1[1] * src2[1];
	return;
case 3:
	dot = src1[0] * src2[0] + src1[1] * src2[1] + src1[2] * src2[2];
	return;
default:
	__asm {
		mov			ecx, src1
			mov			edx, src2
			mov			eax, ecx
			or			eax, edx
			and			eax, 15
			jz			alignedDot
			// unaligned
			mov			eax, count
			shr			eax, 2
			shl			eax, 4
			add			ecx, eax
			add			edx, eax
			neg			eax
			movups		xmm0, [ecx+eax]
		movups		xmm1, [edx+eax]
		mulps		xmm0, xmm1
			add			eax, 16
			jz			doneDot
loopUnalignedDot:
		movups		xmm1, [ecx+eax]
		movups		xmm2, [edx+eax]
		mulps		xmm1, xmm2
			addps		xmm0, xmm1
			add			eax, 16
			jl			loopUnalignedDot
			jmp			doneDot
			// aligned
alignedDot:
		mov			eax, count
			shr			eax, 2
			shl			eax, 4
			add			ecx, eax
			add			edx, eax
			neg			eax
			movaps		xmm0, [ecx+eax]
		movaps		xmm1, [edx+eax]
		mulps		xmm0, xmm1
			add			eax, 16
			jz			doneDot
loopAlignedDot:
		movaps		xmm1, [ecx+eax]
		movaps		xmm2, [edx+eax]
		mulps		xmm1, xmm2
			addps		xmm0, xmm1
			add			eax, 16
			jl			loopAlignedDot
doneDot:
	}
	switch( count & 3 ) {
case 1:
	__asm {
		movss	xmm1, [ecx]
		movss	xmm2, [edx]
		mulss	xmm1, xmm2
			addss	xmm0, xmm1
	}
	break;
case 2:
	__asm {
		xorps	xmm2, xmm2
			movlps	xmm1, [ecx]
		movlps	xmm2, [edx]
		mulps	xmm1, xmm2
			addps	xmm0, xmm1
	}
	break;
case 3:
	__asm {
		movss	xmm1, [ecx]
		movhps	xmm1, [ecx+4]
		movss	xmm2, [edx]
		movhps	xmm2, [edx+4]
		mulps	xmm1, xmm2
			addps	xmm0, xmm1
	}
	break;
	}
	__asm {
		movhlps		xmm1, xmm0
			addps		xmm0, xmm1
			movaps		xmm1, xmm0
			shufps		xmm1, xmm1, R_SHUFFLE_PS( 1, 0, 0, 0 )
			addss		xmm0, xmm1
			mov			eax, dot
			movss		[eax], xmm0
	}
	return;
	}
}

//
//	cmpeqps		==		Equal
//	cmpneqps	!=		Not Equal
//	cmpltps		<		Less Than
//  cmpnltps	>=		Not Less Than
//	cmpnleps	>		Not Less Or Equal
//
#define FLIP	not al
#define NOFLIP

#define COMPARECONSTANT( DST, SRC0, CONSTANT, COUNT, CMP, CMPSIMD, DOFLIP )				\
int i, cnt, pre, post;																\
float *aligned;																		\
\
/* if the float array is not aligned on a 4 byte boundary */						\
if ( ((int) SRC0) & 3 ) {															\
/* unaligned memory access */													\
pre = 0;																		\
cnt = COUNT >> 2;																\
post = COUNT - (cnt<<2);														\
__asm	mov			edx, cnt													\
__asm	test		edx, edx													\
__asm	je			doneCmp														\
__asm	push		ebx															\
__asm	neg			edx															\
__asm	mov			esi, SRC0													\
__asm	prefetchnta	[esi+64]													\
__asm	movss		xmm1, CONSTANT												\
__asm	shufps		xmm1, xmm1, R_SHUFFLE_PS( 0, 0, 0, 0 )						\
__asm	mov			edi, DST													\
__asm	mov			ecx, 0x01010101												\
__asm loopNA:																	\
__asm	movups		xmm0, [esi]													\
__asm	prefetchnta	[esi+128]													\
__asm	CMPSIMD		xmm0, xmm1													\
__asm	movmskps	eax, xmm0													\
__asm	DOFLIP																	\
__asm	mov			ah, al														\
__asm	shr			ah, 1														\
__asm	mov			bx, ax														\
__asm	shl			ebx, 14														\
__asm	mov			bx, ax														\
__asm	and			ebx, ecx													\
__asm	mov			dword ptr [edi], ebx										\
__asm	add			esi, 16														\
__asm	add			edi, 4														\
__asm	inc			edx															\
__asm	jl			loopNA														\
__asm	pop			ebx															\
}																					\
else {																				\
/* aligned memory access */														\
aligned = (float *) ((((int) SRC0) + 15) & ~15);								\
if ( (int)aligned > ((int)src0) + COUNT ) {										\
pre = COUNT;																\
post = 0;																	\
}																				\
else {																			\
pre = aligned - SRC0;														\
cnt = (COUNT - pre) >> 2;													\
post = COUNT - pre - (cnt<<2);												\
__asm	mov			edx, cnt												\
__asm	test		edx, edx												\
__asm	je			doneCmp													\
__asm	push		ebx														\
__asm	neg			edx														\
__asm	mov			esi, aligned											\
__asm	prefetchnta	[esi+64]												\
__asm	movss		xmm1, CONSTANT											\
__asm	shufps		xmm1, xmm1, R_SHUFFLE_PS( 0, 0, 0, 0 )					\
__asm	mov			edi, DST												\
__asm	add			edi, pre												\
__asm	mov			ecx, 0x01010101											\
__asm loopA:																\
__asm	movaps		xmm0, [esi]												\
__asm	prefetchnta	[esi+128]												\
__asm	CMPSIMD		xmm0, xmm1												\
__asm	movmskps	eax, xmm0												\
__asm	DOFLIP																\
__asm	mov			ah, al													\
__asm	shr			ah, 1													\
__asm	mov			bx, ax													\
__asm	shl			ebx, 14													\
__asm	mov			bx, ax													\
__asm	and			ebx, ecx												\
__asm	mov			dword ptr [edi], ebx									\
__asm	add			esi, 16													\
__asm	add			edi, 4													\
__asm	inc			edx														\
__asm	jl			loopA													\
__asm	pop			ebx														\
}																				\
}																					\
doneCmp:																			\
double c = constant;																\
for ( i = 0; i < pre; i++ ) {														\
dst[i] = src0[i] CMP c;															\
}																					\
for ( i = count - post; i < count; i++ ) {											\
dst[i] = src0[i] CMP c;															\
}

#define COMPAREBITCONSTANT( DST, BITNUM, SRC0, CONSTANT, COUNT, CMP, CMPSIMD, DOFLIP )	\
int i, cnt, pre, post;																\
float *aligned;																		\
\
/* if the float array is not aligned on a 4 byte boundary */						\
if ( ((int) SRC0) & 3 ) {															\
/* unaligned memory access */													\
pre = 0;																		\
cnt = COUNT >> 2;																\
post = COUNT - (cnt<<2);														\
__asm	mov			edx, cnt													\
__asm	test		edx, edx													\
__asm	je			doneCmp														\
__asm	push		ebx															\
__asm	neg			edx															\
__asm	mov			esi, SRC0													\
__asm	prefetchnta	[esi+64]													\
__asm	movss		xmm1, CONSTANT												\
__asm	shufps		xmm1, xmm1, R_SHUFFLE_PS( 0, 0, 0, 0 )						\
__asm	mov			edi, DST													\
__asm	mov			cl, bitNum													\
__asm loopNA:																	\
__asm	movups		xmm0, [esi]													\
__asm	prefetchnta	[esi+128]													\
__asm	CMPSIMD		xmm0, xmm1													\
__asm	movmskps	eax, xmm0													\
__asm	DOFLIP																	\
__asm	mov			ah, al														\
__asm	shr			ah, 1														\
__asm	mov			bx, ax														\
__asm	shl			ebx, 14														\
__asm	mov			bx, ax														\
__asm	and			ebx, 0x01010101												\
__asm	shl			ebx, cl														\
__asm	or			ebx, dword ptr [edi]										\
__asm	mov			dword ptr [edi], ebx										\
__asm	add			esi, 16														\
__asm	add			edi, 4														\
__asm	inc			edx															\
__asm	jl			loopNA														\
__asm	pop			ebx															\
}																					\
else {																				\
/* aligned memory access */														\
aligned = (float *) ((((int) SRC0) + 15) & ~15);								\
if ( (int)aligned > ((int)src0) + COUNT ) {										\
pre = COUNT;																\
post = 0;																	\
}																				\
else {																			\
pre = aligned - SRC0;														\
cnt = (COUNT - pre) >> 2;													\
post = COUNT - pre - (cnt<<2);												\
__asm	mov			edx, cnt												\
__asm	test		edx, edx												\
__asm	je			doneCmp													\
__asm	push		ebx														\
__asm	neg			edx														\
__asm	mov			esi, aligned											\
__asm	prefetchnta	[esi+64]												\
__asm	movss		xmm1, CONSTANT											\
__asm	shufps		xmm1, xmm1, R_SHUFFLE_PS( 0, 0, 0, 0 )					\
__asm	mov			edi, DST												\
__asm	add			edi, pre												\
__asm	mov			cl, bitNum												\
__asm loopA:																\
__asm	movaps		xmm0, [esi]												\
__asm	prefetchnta	[esi+128]												\
__asm	CMPSIMD		xmm0, xmm1												\
__asm	movmskps	eax, xmm0												\
__asm	DOFLIP																\
__asm	mov			ah, al													\
__asm	shr			ah, 1													\
__asm	mov			bx, ax													\
__asm	shl			ebx, 14													\
__asm	mov			bx, ax													\
__asm	and			ebx, 0x01010101											\
__asm	shl			ebx, cl													\
__asm	or			ebx, dword ptr [edi]									\
__asm	mov			dword ptr [edi], ebx									\
__asm	add			esi, 16													\
__asm	add			edi, 4													\
__asm	inc			edx														\
__asm	jl			loopA													\
__asm	pop			ebx														\
}																				\
}																					\
doneCmp:																			\
float c = constant;																	\
for ( i = 0; i < pre; i++ ) {														\
dst[i] |= ( src0[i] CMP c ) << BITNUM;											\
}																					\
for ( i = count - post; i < count; i++ ) {											\
dst[i] |= ( src0[i] CMP c ) << BITNUM;											\
}

/*
============
SIMD_SSE::CmpGT

dst[i] = src0[i] > constant;
============
*/
void SIMD_SSE::CmpGT( byte *dst, const float constant, const float *src0, const int count ) {
	COMPARECONSTANT( dst, src0, constant, count, >, cmpnleps, NOFLIP )
}

/*
============
SIMD_SSE::CmpGE

dst[i] = src0[i] >= constant;
============
*/
void SIMD_SSE::CmpGE( byte *dst, const float constant, const float *src0, const int count ) {
	COMPARECONSTANT( dst, src0, constant, count, >=, cmpnltps, NOFLIP )
}

/*
============
SIMD_SSE::CmpLT

dst[i] = src0[i] < constant;
============
*/
void SIMD_SSE::CmpLT( byte *dst, const float constant, const float *src0, const int count ) {
	COMPARECONSTANT( dst, src0, constant, count, <, cmpltps, NOFLIP )
}


/*
============
SIMD_SSE::CmpLE

dst[i] = src0[i] <= constant;
============
*/
void SIMD_SSE::CmpLE( byte *dst, const float constant, const float *src0, const int count ) {
	COMPARECONSTANT( dst, src0, constant, count, <=, cmpnleps, FLIP )
}


/*
============
SIMD_SSE::MinMax
============
*/
void SIMD_SSE::MinMax( float &min, float &max, const float *src, const int count ) {
	int i, pre, post;

	min = math::Infinity; max = -math::Infinity;

	__asm
	{
		push		ebx
			mov			eax, min
			mov			ebx, max
			movss		xmm0, [eax]
		movss		xmm1, [ebx]
		shufps		xmm0, xmm0, 0
			shufps		xmm1, xmm1, 0

			KFLOATINITS( src, count, pre, post )
			and			eax, 15
			jz			lpA
			jmp			lpNA
			align		16
lpNA:
		movups		xmm2, [edx+ebx]
		movups		xmm3, [edx+ebx+16]
		minps		xmm0, xmm2
			maxps		xmm1, xmm2
			prefetchnta	[edx+ebx+64]
		minps		xmm0, xmm3
			maxps		xmm1, xmm3
			add			ebx, 16*2
			jl			lpNA
			jmp			done2
lpA:
		movaps		xmm2, [edx+ebx]
		movaps		xmm3, [edx+ebx+16]
		minps		xmm0, xmm2
			maxps		xmm1, xmm2
			prefetchnta	[edx+ebx+64]
		minps		xmm0, xmm3
			maxps		xmm1, xmm3
			add			ebx, 16*2
			jl			lpA
			jmp			done2
			align		16
done2:
		movaps		xmm2, xmm0
			movaps		xmm3, xmm1
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 2, 3, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 2, 3, 0 )
			minss		xmm0, xmm2
			maxss		xmm1, xmm3
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 2, 3, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 2, 3, 0 )
			minss		xmm0, xmm2
			maxss		xmm1, xmm3
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 1, 2, 3, 0 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 1, 2, 3, 0 )
			minss		xmm0, xmm2
			maxss		xmm1, xmm3
			mov			eax, min
			mov			ebx, max
			movss		[eax], xmm0
			movss		[ebx], xmm1
done:
		pop			ebx
	}

	for ( i = 0; i < pre; i++ ) {
		float tmp = src[i];
		if ( tmp > max ) {
			max = tmp;
		}
		if ( tmp < min ) {
			min = tmp;
		}
	}
	for ( i = count - post; i < count; i++ ) {
		float tmp = src[i];
		if ( tmp > max ) {
			max = tmp;
		}
		if ( tmp < min ) {
			min = tmp;
		}
	}
}

/*
============
SIMD_SSE::MinMax
============
*/
void SIMD_SSE::MinMax( math::vector2d &min, math::vector2d &max, const math::vector2d *src, const int count ) {
	__asm {
		mov			eax, count
			test		eax, eax
			movss		xmm0, math::Infinity
			xorps		xmm1, xmm1
			shufps		xmm0, xmm0, R_SHUFFLE_PS( 0, 0, 0, 0 )
			subps		xmm1, xmm0
			jz			done
			mov			esi, src
			test		eax, 1
			jz			startLoop
			movlps		xmm2, [esi]
		shufps		xmm2, xmm2, R_SHUFFLE_PS( 0, 1, 0, 1 )
			add			esi, 2*4
			dec			eax
			minps		xmm0, xmm2
			maxps		xmm1, xmm2
			jz			done
startLoop:
		shl			eax, 3
			add			esi, eax
			neg			eax
loopVert:
		movlps		xmm2, [esi+eax]
		movhps		xmm2, [esi+eax+8]
		add			eax, 4*4
			minps		xmm0, xmm2
			maxps		xmm1, xmm2
			jl			loopVert
done:
		movaps		xmm2, xmm0
			shufps		xmm2, xmm2, R_SHUFFLE_PS( 2, 3, 0, 1 )
			minps		xmm0, xmm2
			mov			esi, min
			movlps		[esi], xmm0
			movaps		xmm3, xmm1
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 2, 3, 0, 1 )
			maxps		xmm1, xmm3
			mov			edi, max
			movlps		[edi], xmm1
	}
}

/*
============
SIMD_SSE::MinMax
============
*/
void SIMD_SSE::MinMax( math::vector3d &min, math::vector3d &max, const math::vector3d *src, const int count ) {
	__asm {

		movss		xmm0, math::Infinity
			xorps		xmm1, xmm1
			shufps		xmm0, xmm0, R_SHUFFLE_PS( 0, 0, 0, 0 )
			subps		xmm1, xmm0
			movaps		xmm2, xmm0
			movaps		xmm3, xmm1

			mov			esi, src
			mov			eax, count
			and			eax, ~3
			jz			done4
			imul		eax, 12
			add			esi, eax
			neg			eax

loop4:
		//		prefetchnta	[esi+4*12]

		movss		xmm4, [esi+eax+0*12+8]
		movhps		xmm4, [esi+eax+0*12+0]
		minps		xmm0, xmm4
			maxps		xmm1, xmm4

			movss		xmm5, [esi+eax+1*12+0]
		movhps		xmm5, [esi+eax+1*12+4]
		minps		xmm2, xmm5
			maxps		xmm3, xmm5

			movss		xmm6, [esi+eax+2*12+8]
		movhps		xmm6, [esi+eax+2*12+0]
		minps		xmm0, xmm6
			maxps		xmm1, xmm6

			movss		xmm7, [esi+eax+3*12+0]
		movhps		xmm7, [esi+eax+3*12+4]
		minps		xmm2, xmm7
			maxps		xmm3, xmm7

			add			eax, 4*12
			jl			loop4

done4:
		mov			eax, count
			and			eax, 3
			jz			done1
			imul		eax, 12
			add			esi, eax
			neg			eax

loop1:
		movss		xmm4, [esi+eax+0*12+8]
		movhps		xmm4, [esi+eax+0*12+0]
		minps		xmm0, xmm4
			maxps		xmm1, xmm4

			add			eax, 12
			jl			loop1

done1:
		shufps		xmm2, xmm2, R_SHUFFLE_PS( 3, 1, 0, 2 )
			shufps		xmm3, xmm3, R_SHUFFLE_PS( 3, 1, 0, 2 )
			minps		xmm0, xmm2
			maxps		xmm1, xmm3
			mov			esi, min
			movhps		[esi], xmm0
			movss		[esi+8], xmm0
			mov			edi, max
			movhps		[edi], xmm1
			movss		[edi+8], xmm1
	}
}

/*
============
SIMD_SSE::Clamp
============
*/
void SIMD_SSE::Clamp( float *dst,  const float min, const float max, const float *src,const int count ) {
	int	i, pre, post;

	__asm
	{
		movss	xmm0,min
			movss	xmm1,max
			shufps	xmm0,xmm0,0
			shufps	xmm1,xmm1,0

			KFLOATINITDS( dst, src, count, pre, post )
			and		eax,15
			jne		lpNA
			jmp		lpA
			align	16
lpA:
		movaps	xmm2,[edx+ebx]
		movaps	xmm3,[edx+ebx+16]
		maxps	xmm2,xmm0
			maxps	xmm3,xmm0
			prefetchnta	[edx+ebx+64]
		minps	xmm2,xmm1
			minps	xmm3,xmm1
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpA
			jmp		done

			align	16
lpNA:
		movups	xmm2,[edx+ebx]
		movups	xmm3,[edx+ebx+16]
		maxps	xmm2,xmm0
			maxps	xmm3,xmm0
			prefetchnta	[edx+ebx+64]
		minps	xmm2,xmm1
			minps	xmm3,xmm1
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpNA
done:
	}

	for ( i = 0; i < pre; i++ ) {
		if ( src[i] < min )
			dst[i] = min;
		else if ( src[i] > max )
			dst[i] = max;
		else
			dst[i] = src[i];
	}

	for( i = count - post; i < count; i++ ) {
		if ( src[i] < min )
			dst[i] = min;
		else if ( src[i] > max )
			dst[i] = max;
		else
			dst[i] = src[i];
	}
}

/*
============
SIMD_SSE::ClampMin
============
*/
void SIMD_SSE::ClampMin( float *dst, const float min, const float *src, const int count ) {
	int	i, pre, post;

	__asm
	{
		movss	xmm0,min
			shufps	xmm0,xmm0,0

			KFLOATINITDS( dst, src, count, pre, post )
			and		eax,15
			jne		lpNA
			jmp		lpA
			align	16
lpA:
		movaps	xmm2,[edx+ebx]
		movaps	xmm3,[edx+ebx+16]
		maxps	xmm2,xmm0
			prefetchnta	[edx+ebx+64]
		maxps	xmm3,xmm0
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpA
			jmp		done

			align	16
lpNA:
		movups	xmm2,[edx+ebx]
		movups	xmm3,[edx+ebx+16]
		maxps	xmm2,xmm0
			prefetchnta	[edx+ebx+64]
		maxps	xmm3,xmm0
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpNA
done:
	}

	for( i = 0; i < pre; i++ ) {
		if ( src[i] < min )
			dst[i] = min;
		else
			dst[i] = src[i];
	}
	for( i = count - post; i < count; i++ ) {
		if ( src[i] < min )
			dst[i] = min;
		else
			dst[i] = src[i];
	}
}

/*
============
SIMD_SSE::ClampMax
============
*/
void SIMD_SSE::ClampMax( float *dst,  const float max,const float *src, const int count ) {
	int	i, pre, post;

	__asm
	{
		movss	xmm1,max
			shufps	xmm1,xmm1,0

			KFLOATINITDS( dst, src, count, pre, post )
			and		eax,15
			jne		lpNA
			jmp		lpA
			align	16
lpA:
		movaps	xmm2,[edx+ebx]
		movaps	xmm3,[edx+ebx+16]
		minps	xmm2,xmm1
			prefetchnta	[edx+ebx+64]
		minps	xmm3,xmm1
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpA
			jmp		done

			align	16
lpNA:
		movups	xmm2,[edx+ebx]
		movups	xmm3,[edx+ebx+16]
		minps	xmm2,xmm1
			prefetchnta	[edx+ebx+64]
		minps	xmm3,xmm1
			movaps	[edi+ebx],xmm2
			movaps	[edi+ebx+16],xmm3
			add		ebx,16*2
			jl		lpNA
done:
	}

	for( i = 0; i < pre; i++ ) {
		if ( src[i] > max )
			dst[i] = max;
		else
			dst[i] = src[i];
	}

	for( i = count - post; i < count; i++ ) {
		if ( src[i] > max )
			dst[i] = max;
		else
			dst[i] = src[i];
	}
}

/*
============
SIMD_SSE::Zero16
============
*/
void SIMD_SSE::Zero16( float *dst, const int count ) {
	__asm {
		mov		edx, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneZero16
			shl		eax, 4
			add		edx, eax
			neg		eax
			xorps	xmm0, xmm0
loopZero16:
		movaps	[edx+eax], xmm0
			add		eax, 16
			jl		loopZero16
doneZero16:
	}
}

/*
============
SIMD_SSE::Negate16
============
*/
void SIMD_SSE::Negate16( float *dst, const int count ) {
	__asm {
		mov		edx, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneNegate16
			shl		eax, 4
			add		edx, eax
			neg		eax
			movss	xmm0, SIMD_SP_signBit
			shufps	xmm0, xmm0, R_SHUFFLE_PS( 0, 0, 0, 0 )
loopNegate16:
		movaps	xmm1, [edx+eax]
		xorps	xmm1, xmm0
			movaps	[edx+eax], xmm1
			add		eax, 16
			jl		loopNegate16
doneNegate16:
	}
}

/*
============
SIMD_SSE::Copy16
============
*/
void SIMD_SSE::Copy16( float *dst, const float *src, const int count ) {
	__asm {
		mov		ecx, src
			mov		edx, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneCopy16
			shl		eax, 4
			add		ecx, eax
			add		edx, eax
			neg		eax
loopCopy16:
		movaps	xmm0, [ecx+eax]
		movaps	[edx+eax], xmm0
			add		eax, 16
			jl		loopCopy16
doneCopy16:
	}
}

/*
============
SIMD_SSE::Add16
============
*/
void SIMD_SSE::Add16( float *dst, const float *src1, const float *src2, const int count ) {
	__asm {
		mov		ecx, src1
			mov		edx, src2
			mov		esi, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneAdd16
			shl		eax, 4
			add		esi, eax
			add		ecx, eax
			add		edx, eax
			neg		eax
loopAdd16:
		movaps	xmm0, [ecx+eax]
		addps	xmm0, [edx+eax]
		movaps	[esi+eax], xmm0
			add		eax, 16
			jl		loopAdd16
doneAdd16:
	}
}

/*
============
SIMD_SSE::Sub16
============
*/
void SIMD_SSE::Sub16( float *dst, const float *src1, const float *src2, const int count ) {
	__asm {
		mov		ecx, src1
			mov		edx, src2
			mov		esi, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneSub16
			shl		eax, 4
			add		esi, eax
			add		ecx, eax
			add		edx, eax
			neg		eax
loopSub16:
		movaps	xmm0, [ecx+eax]
		subps	xmm0, [edx+eax]
		movaps	[esi+eax], xmm0
			add		eax, 16
			jl		loopSub16
doneSub16:
	}
}

/*
============
SIMD_SSE::Mul16
============
*/
void SIMD_SSE::Mult16( float *dst, const float *src1, const float constant, const int count ) {
	__asm {
		mov		ecx, dst
			mov		edx, src1
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneMulScalar16
			movss	xmm1, constant
			shl		eax, 4
			add		ecx, eax
			add		edx, eax
			neg		eax
			shufps	xmm1, xmm1, 0x00
loopMulScalar16:
		movaps	xmm0, [edx+eax]
		mulps	xmm0, xmm1
			movaps	[ecx+eax], xmm0
			add		eax, 16
			jl		loopMulScalar16
doneMulScalar16:
	}
}

/*
============
SIMD_SSE::AddAssign16
============
*/
void SIMD_SSE::AddAssign16( float *dst, const float *src, const int count ) {
	__asm {
		mov		ecx, dst
			mov		edx, src
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneAddAssign16
			shl		eax, 4
			add		ecx, eax
			add		edx, eax
			neg		eax
loopAddAssign16:
		movaps	xmm0, [ecx+eax]
		addps	xmm0, [edx+eax]
		movaps	[ecx+eax], xmm0
			add		eax, 16
			jl		loopAddAssign16
doneAddAssign16:
	}
}

/*
============
SIMD_SSE::SubAssign16
============
*/
void SIMD_SSE::SubAssign16( float *dst, const float *src, const int count ) {
	__asm {
		mov		ecx, dst
			mov		edx, src
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneSubAssign16
			shl		eax, 4
			add		ecx, eax
			add		edx, eax
			neg		eax
loopSubAssign16:
		movaps	xmm0, [ecx+eax]
		subps	xmm0, [edx+eax]
		movaps	[ecx+eax], xmm0
			add		eax, 16
			jl		loopSubAssign16
doneSubAssign16:
	}
}

/*
============
SIMD_SSE::MulAssign16
============
*/
void SIMD_SSE::MultAssign16( float *dst, const float constant, const int count ) {
	__asm {
		mov		ecx, dst
			mov		eax, count
			add		eax, 3
			shr		eax, 2
			jz		doneMulAssign16
			movss	xmm1, constant
			shl		eax, 4
			add		ecx, eax
			neg		eax
			shufps	xmm1, xmm1, 0x00
loopMulAssign16:
		movaps	xmm0, [ecx+eax]
		mulps	xmm0, xmm1
			movaps	[ecx+eax], xmm0
			add		eax, 16
			jl		loopMulAssign16
doneMulAssign16:
	}
}

/*
============
SIMD_SSE::MatX_MultiplyVecX

optimizes the following matrix multiplications:

NxN * Nx1
Nx6 * 6x1
6xN * Nx1

with N in the range [1-6]
============
*/
void SIMD_SSE::Mat_MultiplyVec( math::vector4d &dst, const math::matrix4x4 &mat, const math::vector4d &vec ) {
#define STORE1( offset, reg1, reg2 )		\
__asm movss		[eax+offset], reg1
#define STORE2LO( offset, reg1, reg2 )		\
__asm movlps	[eax+offset], reg1
#define STORE2HI( offset, reg1, reg2 )		\
__asm movhps	[eax+offset], reg1
#define STORE4( offset, reg1, reg2 )		\
__asm movlps	[eax+offset], reg1		\
__asm movhps	[eax+offset+8], reg1
#define STOREC		=

	const float *mPtr;
	float dstPtr[4], vPtr[4];


	mPtr = mat.getMatPointer();
	vec.getAsArray(vPtr);
	dst.getAsArray(dstPtr);
	__asm {
		mov			esi, vPtr
			mov			edi, mPtr
			mov			eax, dstPtr
			movlps		xmm6, qword ptr [esi ]
		movlps		xmm0, qword ptr [edi ]
		shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm0, qword ptr [edi+16]
		mulps		xmm0, xmm6
			movlps		xmm7, qword ptr [esi+ 8]
		movlps		xmm2, qword ptr [edi+ 8]
		shufps		xmm7, xmm7, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm2, qword ptr [edi+24]
		mulps		xmm2, xmm7
			movlps		xmm1, qword ptr [edi+32]
		movhps		xmm1, qword ptr [edi+48]
		mulps		xmm1, xmm6
			movlps		xmm3, qword ptr [edi+40]
		addps		xmm0, xmm2
			movhps		xmm3, qword ptr [edi+56]
		mulps		xmm3, xmm7
			movaps		xmm4, xmm0
			addps		xmm1, xmm3
			shufps		xmm4, xmm1, R_SHUFFLE_PS( 0, 2, 0, 2 )
			shufps		xmm0, xmm1, R_SHUFFLE_PS( 1, 3, 1, 3 )
			addps		xmm0, xmm4
			STORE4( 0, xmm0, xmm2 )
	}

#undef STOREC
#undef STORE4
#undef STORE2HI
#undef STORE2LO
#undef STORE1
}

/*
============
SIMD_SSE::MatX_MultiplyAddVecX

optimizes the following matrix multiplications:

NxN * Nx1
Nx6 * 6x1
6xN * Nx1

with N in the range [1-6]
============
*/
void SIMD_SSE::Mat_MultiplyAddVec( math::vector4d &dst, const math::matrix4x4 &mat, const math::vector4d &vec ) {
#define STORE1( offset, reg1, reg2 )		\
__asm movss		reg2, [eax+offset]		\
__asm addss		reg2, reg1				\
__asm movss		[eax+offset], reg2
#define STORE2LO( offset, reg1, reg2 )		\
__asm movlps	reg2, [eax+offset]		\
__asm addps		reg2, reg1				\
__asm movlps	[eax+offset], reg2
#define STORE2HI( offset, reg1, reg2 )		\
__asm movhps	reg2, [eax+offset]		\
__asm addps		reg2, reg1				\
__asm movhps	[eax+offset], reg2
#define STORE4( offset, reg1, reg2 )		\
__asm movlps	reg2, [eax+offset]		\
__asm movhps	reg2, [eax+offset+8]	\
__asm addps		reg2, reg1				\
__asm movlps	[eax+offset], reg2		\
__asm movhps	[eax+offset+8], reg2
#define STOREC		+=

	const float *mPtr;
	float dstPtr[4],vPtr[4];

	mPtr = mat.getMatPointer();
	vec.getAsArray(vPtr);
	dst.getAsArray(dstPtr);
	__asm {
		mov			esi, vPtr
			mov			edi, mPtr
			mov			eax, dstPtr
			movlps		xmm6, qword ptr [esi ]
		movlps		xmm0, qword ptr [edi ]
		shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm0, qword ptr [edi+16]
		mulps		xmm0, xmm6
			movlps		xmm7, qword ptr [esi+ 8]
		movlps		xmm2, qword ptr [edi+ 8]
		shufps		xmm7, xmm7, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm2, qword ptr [edi+24]
		mulps		xmm2, xmm7
			movlps		xmm1, qword ptr [edi+32]
		movhps		xmm1, qword ptr [edi+48]
		mulps		xmm1, xmm6
			movlps		xmm3, qword ptr [edi+40]
		addps		xmm0, xmm2
			movhps		xmm3, qword ptr [edi+56]
		mulps		xmm3, xmm7
			movaps		xmm4, xmm0
			addps		xmm1, xmm3
			shufps		xmm4, xmm1, R_SHUFFLE_PS( 0, 2, 0, 2 )
			shufps		xmm0, xmm1, R_SHUFFLE_PS( 1, 3, 1, 3 )
			addps		xmm0, xmm4
			STORE4( 0, xmm0, xmm2 )
	}

#undef STOREC
#undef STORE4
#undef STORE2HI
#undef STORE2LO
#undef STORE1
}

/*
============
SIMD_SSE::Mat_MultiplySubVec

============
*/
void SIMD_SSE::Mat_MultiplySubVec( math::vector4d &dst, const math::matrix4x4 &mat, const math::vector4d &vec ) {
#define STORE1( offset, reg1, reg2 )		\
__asm movss		reg2, [eax+offset]		\
__asm subss		reg2, reg1				\
__asm movss		[eax+offset], reg2
#define STORE2LO( offset, reg1, reg2 )		\
__asm movlps	reg2, [eax+offset]		\
__asm subps		reg2, reg1				\
__asm movlps	[eax+offset], reg2
#define STORE2HI( offset, reg1, reg2 )		\
__asm movhps	reg2, [eax+offset]		\
__asm subps		reg2, reg1				\
__asm movhps	[eax+offset], reg2
#define STORE4( offset, reg1, reg2 )		\
__asm movlps	reg2, [eax+offset]		\
__asm movhps	reg2, [eax+offset+8]	\
__asm subps		reg2, reg1				\
__asm movlps	[eax+offset], reg2		\
__asm movhps	[eax+offset+8], reg2
#define STOREC		-=

	const float *mPtr;
	float dstPtr[4],vPtr[4];

	mPtr = mat.getMatPointer();
	vec.getAsArray(vPtr);
	dst.getAsArray(dstPtr);
	__asm {
		mov			esi, vPtr
			mov			edi, mPtr
			mov			eax, dstPtr
			movlps		xmm6, qword ptr [esi ]
		movlps		xmm0, qword ptr [edi ]
		shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm0, qword ptr [edi+16]
		mulps		xmm0, xmm6
			movlps		xmm7, qword ptr [esi+ 8]
		movlps		xmm2, qword ptr [edi+ 8]
		shufps		xmm7, xmm7, R_SHUFFLE_PS( 0, 1, 0, 1 )
			movhps		xmm2, qword ptr [edi+24]
		mulps		xmm2, xmm7
			movlps		xmm1, qword ptr [edi+32]
		movhps		xmm1, qword ptr [edi+48]
		mulps		xmm1, xmm6
			movlps		xmm3, qword ptr [edi+40]
		addps		xmm0, xmm2
			movhps		xmm3, qword ptr [edi+56]
		mulps		xmm3, xmm7
			movaps		xmm4, xmm0
			addps		xmm1, xmm3
			shufps		xmm4, xmm1, R_SHUFFLE_PS( 0, 2, 0, 2 )
			shufps		xmm0, xmm1, R_SHUFFLE_PS( 1, 3, 1, 3 )
			addps		xmm0, xmm4
			STORE4( 0, xmm0, xmm2 )
	}

#undef STOREC
#undef STORE4
#undef STORE2HI
#undef STORE2LO
#undef STORE1
}


/*
============
SIMD_SSE::MatX_MultiplyMatX

optimizes the following matrix multiplications:

NxN * Nx6
6xN * Nx6
Nx6 * 6xN
6x6 * 6xN

with N in the range [1-6].

The hot cache clock cycle counts are generally better for the SIMD version than the
FPU version. At times up to 40% less clock cycles on a P3. In practise however,
the results are poor probably due to memory access.
============
*/
void SIMD_SSE::Mat_MultiplyMat( math::matrix4x4 &dst, const math::matrix4x4 &m1, const math::matrix4x4 &m2 ) {
	float *dstPtr;
	const float *m1Ptr, *m2Ptr;


	dstPtr = dst.getMatPointer();
	m1Ptr = m1.getMatPointer();
	m2Ptr = m2.getMatPointer();

#define MUL_Nx4_4x6_FIRST4COLUMNS_INIT						\
__asm mov			esi, m2Ptr								\
__asm mov			edi, m1Ptr								\
__asm mov			eax, dstPtr								\
__asm movlps		xmm0, [esi+ 0*4]						\
__asm movhps		xmm0, [esi+ 2*4]						\
__asm movlps		xmm1, [esi+ 6*4]						\
__asm movhps		xmm1, [esi+ 8*4]						\
__asm movlps		xmm2, [esi+12*4]						\
__asm movhps		xmm2, [esi+14*4]						\
__asm movlps		xmm3, [esi+18*4]						\
__asm movhps		xmm3, [esi+20*4]

#define MUL_Nx4_4x6_FIRST4COLUMNS_ROW( row )				\
__asm movss			xmm4, [edi+row*16+0*4]					\
__asm shufps		xmm4, xmm4, R_SHUFFLE_PS( 0, 0, 0, 0 )	\
__asm mulps			xmm4, xmm0								\
__asm movss			xmm5, [edi+row*16+1*4]					\
__asm shufps		xmm5, xmm5, R_SHUFFLE_PS( 0, 0, 0, 0 )	\
__asm mulps			xmm5, xmm1								\
__asm addps			xmm4, xmm5								\
__asm movss			xmm6, [edi+row*16+2*4]					\
__asm shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 0, 0, 0 )	\
__asm mulps			xmm6, xmm2								\
__asm addps			xmm4, xmm6								\
__asm movss			xmm7, [edi+row*16+3*4]					\
__asm shufps		xmm7, xmm7, R_SHUFFLE_PS( 0, 0, 0, 0 )	\
__asm mulps			xmm7, xmm3								\
__asm addps			xmm4, xmm7								\
__asm movlps		[eax+row*24+0], xmm4					\
__asm movhps		[eax+row*24+8], xmm4

#define MUL_Nx4_4x6_LAST2COLUMNS_INIT						\
__asm movlps		xmm0, [esi+ 4*4]						\
__asm movlps		xmm1, [esi+10*4]						\
__asm movlps		xmm2, [esi+16*4]						\
__asm movlps		xmm3, [esi+22*4]						\
__asm shufps		xmm0, xmm1, R_SHUFFLE_PS( 0, 1, 0, 1 )	\
__asm shufps		xmm1, xmm0, R_SHUFFLE_PS( 0, 1, 0, 1 )	\
__asm shufps		xmm2, xmm3, R_SHUFFLE_PS( 0, 1, 0, 1 )	\
__asm shufps		xmm3, xmm2, R_SHUFFLE_PS( 0, 1, 0, 1 )

#define MUL_Nx4_4x6_LAST2COLUMNS_ROW2( row )				\
__asm movlps		xmm7, [edi+row*32+ 0*4]					\
__asm movhps		xmm7, [edi+row*32+ 4*4]					\
__asm movaps		xmm6, xmm7								\
__asm shufps		xmm6, xmm6, R_SHUFFLE_PS( 0, 0, 3, 3 )	\
__asm mulps			xmm6, xmm0								\
__asm shufps		xmm7, xmm7, R_SHUFFLE_PS( 1, 1, 2, 2 )	\
__asm mulps			xmm7, xmm1								\
__asm addps			xmm6, xmm7								\
__asm movlps		xmm4, [edi+row*32+ 2*4]					\
__asm movhps		xmm4, [edi+row*32+ 6*4]					\
__asm movaps		xmm5, xmm4								\
__asm shufps		xmm5, xmm5, R_SHUFFLE_PS( 0, 0, 3, 3 )	\
__asm mulps			xmm5, xmm2								\
__asm addps			xmm6, xmm5								\
__asm shufps		xmm4, xmm4, R_SHUFFLE_PS( 1, 1, 2, 2 )	\
__asm mulps			xmm4, xmm3								\
__asm addps			xmm6, xmm4								\
__asm movlps		[eax+row*48+ 4*4], xmm6					\
__asm movhps		[eax+row*48+10*4], xmm6

	MUL_Nx4_4x6_FIRST4COLUMNS_INIT
		MUL_Nx4_4x6_FIRST4COLUMNS_ROW( 0 )
		MUL_Nx4_4x6_FIRST4COLUMNS_ROW( 1 )
		MUL_Nx4_4x6_FIRST4COLUMNS_ROW( 2 )
		MUL_Nx4_4x6_FIRST4COLUMNS_ROW( 3 )
		MUL_Nx4_4x6_LAST2COLUMNS_INIT
		MUL_Nx4_4x6_LAST2COLUMNS_ROW2( 0 )
		MUL_Nx4_4x6_LAST2COLUMNS_ROW2( 1 )

}
#endif

}
