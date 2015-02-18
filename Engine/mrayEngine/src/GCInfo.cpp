#include "stdafx.h"

#include "GCInfo.h"
#include "IGCCollector.h"

namespace mray{

GCInfo::GCInfo(const void* addr){
	refCount=1;
	m_memAddr=addr;/*
	if(!size)
		m_isArray=0;
	else m_isArray=1;

	m_arraySize=size;*/
}

const void*GCInfo::getAddr()const 
{
	return m_memAddr;
}/*
bool GCInfo::isArray(){
	return m_isArray;
}
unsigned int GCInfo::arraySize(){
	return m_arraySize;
}
*/
void GCInfo::addRef(){
	refCount++;
}

}

