
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   19:39
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IOSSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IOSSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IOSSystem___
#define ___IOSSystem___

#include "CompileConfig.h"
#include "mTypes.h"

namespace mray{

	class MRAY_CORE_DLL IOSSystem
{
private:
protected:
public:

	static void* memCopy(void*dst,const void*src,uint size);
	static void* memCopy(void*dst,uint dstSz,const void*src,uint srcSz);
	static void* memSet(void*dst,int v,uint sz);
	static void* memMove(void*dst,void* src,uint sz);
};

}


#endif //___IOSSystem___
