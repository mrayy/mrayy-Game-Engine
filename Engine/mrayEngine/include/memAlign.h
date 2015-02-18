
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   23:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\memAlign.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	memAlign
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___memAlign___
#define ___memAlign___


#ifdef _WIN32
#define ALIGN16( x )					__declspec(align(16)) x
#define PACKED

#define _alloca16( x )					((void *)((((int)_alloca( (x)+15 )) + 15) & ~15))

#define ID_INLINE						__forceinline

#endif


#endif //___memAlign___
