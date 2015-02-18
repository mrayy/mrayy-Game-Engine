#include "stdafx.h"

#include "CatagoryManager.h"
#include "common.h"


namespace mray{


CatagoryManager::CatagoryManager(const core::string&name){
	m_catagory=name;
	m_lastBit=0;
}
CatagoryManager::~CatagoryManager(){
	m_bitsMap.clear();
}

void CatagoryManager::setCatagoryName(const core::string&name){
	m_catagory=name;
}
const core::string& CatagoryManager::getCatagoryName(){
	return m_catagory;
}
bool CatagoryManager::addBit(const core::string&name){
	BitMap::iterator it=m_bitsMap.find(name);
	if(it!=m_bitsMap.end())
		return false;
	ulong bit=BIT(m_lastBit);
	m_bitsMap.insert(BitMap::value_type(name,SBitStruct(name,bit)));
	++m_lastBit;
	return true;
}
void CatagoryManager::setCombineBits(const core::string&name,ulong bits){
	BitMap::iterator it=m_bitsMap.find(name);
	if(it!=m_bitsMap.end())
		return;
	m_bitsMap.insert(BitMap::value_type(name,SBitStruct(name,bits)));
}

ulong CatagoryManager::getBit(const core::string&name){
	BitMap::iterator it=m_bitsMap.find(name);
	if(it==m_bitsMap.end())
		return 0;
	return it->second.m_bits;
}


}

