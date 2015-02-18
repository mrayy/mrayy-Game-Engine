#include "stdafx.h"


#ifdef MRAY_MEMORY_MANAGMENT

#include "CMemory.h"
void* operator new (size_t uiSize)
{
	return mray::CMemory::Allocate(uiSize,0,0,false);
}
//----------------------------------------------------------------------------
void* operator new[](size_t uiSize)
{
    return mray::CMemory::Allocate(uiSize,0,0,true);
}
//----------------------------------------------------------------------------
void* operator new (size_t uiSize, char* acFile, unsigned int uiLine)
{
    return mray::CMemory::Allocate(uiSize,acFile,uiLine,false);
}
//----------------------------------------------------------------------------
void* operator new[] (size_t uiSize, char* acFile, unsigned int uiLine)
{
    return mray::CMemory::Allocate(uiSize,acFile,uiLine,true);
}
//----------------------------------------------------------------------------
void operator delete (void* pvAddr)
{
    mray::CMemory::Deallocate((char*)pvAddr,false);
}
//----------------------------------------------------------------------------
void operator delete[] (void* pvAddr)
{
    mray::CMemory::Deallocate((char*)pvAddr,true);
}
//----------------------------------------------------------------------------
void operator delete (void* pvAddr, char*, unsigned int)
{
    mray::CMemory::Deallocate((char*)pvAddr,false);
}
//----------------------------------------------------------------------------
void operator delete[] (void* pvAddr, char*, unsigned int)
{
    mray::CMemory::Deallocate((char*)pvAddr,true);
}
/*

void* operator new (size_t uiSize){
	return mray::CMemory::newElem(uiSize);
}
void* operator new[](size_t uiSize){
	return mray::CMemory::newArr(uiSize);
}
void* operator new (size_t uiSize, char* acFile, unsigned int uiLine){
	return mray::CMemory::newElem(uiSize,acFile,uiLine);
}
void* operator new[] (size_t uiSize, char* acFile, unsigned int uiLine){
	return mray::CMemory::newArr(uiSize,acFile,uiLine);
}
void operator delete (void* pvAddr){
	return mray::CMemory::deleteElem(pvAddr);
}
void operator delete[] (void* pvAddr){
	return mray::CMemory::deleteArr(pvAddr);
}
void operator delete (void* pvAddr, char*, unsigned int){
	return mray::CMemory::deleteElem(pvAddr);
}
void operator delete[] (void* pvAddr, char*, unsigned int){
	return mray::CMemory::deleteArr(pvAddr);
}*/


#endif


