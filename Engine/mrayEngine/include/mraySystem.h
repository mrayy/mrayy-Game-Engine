
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   19:39
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\mraySystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	mraySystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___mraySystem___
#define ___mraySystem___

#include "CompileConfig.h"
#include "mTypes.h"

namespace mray{

class MRAY_DLL mraySystem
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


#endif //___mraySystem___
