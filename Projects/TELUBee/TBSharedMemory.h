/********************************************************************
	created:	2012/07/29
	created:	29:7:2012   16:31
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VirtualTelesarApp\TBSharedMemory.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VirtualTelesarApp
	file base:	TBSharedMemory
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___TBSharedMemory___
#define ___TBSharedMemory___

#include "CompileConfig.h"


namespace mray
{
namespace TBee
{
	class TBSharedMemoryImpl;

class  TBSharedMemory
{
protected:
	TBSharedMemoryImpl* m_impl;
public:
	TBSharedMemory();
	virtual~TBSharedMemory();

	void Start();
	virtual void Close();

	int GetSendBuffer(char* buff,int len);

};

}
}

#endif