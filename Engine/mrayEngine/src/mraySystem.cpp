#include "stdafx.h"

#include "mraySystem.h"
#include <memory>

#include <asmlib.h>

//#define USE_OPTIMIZED_INSTRUCIONS

namespace mray{


void* mraySystem::memCopy(void*dst,const void*src,uint size){
#ifdef USE_OPTIMIZED_INSTRUCIONS
	A_memcpy(dst,src,size);
#else
	::memcpy(dst,src,size);
#endif
	return dst;
}
void* mraySystem::memCopy(void*dst,uint dstSz,const void*src,uint srcSz){
#ifdef USE_OPTIMIZED_INSTRUCIONS
	if(dstSz<srcSz)
		srcSz=dstSz;
	A_memcpy(dst,src,srcSz);
#else
	::memcpy_s(dst,dstSz,src,srcSz);
#endif
	return dst;
}

void* mraySystem::memSet(void*dst,int v,uint sz){
#ifdef USE_OPTIMIZED_INSTRUCIONS
	A_memset(dst,v,sz);
#else
	::memset(dst,v,sz);
#endif
	return dst;
}

void* mraySystem::memMove(void*dst,void* src,uint sz){
#ifdef USE_OPTIMIZED_INSTRUCIONS
	A_memmove(dst,src,sz);
#else
	::memmove(dst,src,sz);
#endif
	return dst;
}

}