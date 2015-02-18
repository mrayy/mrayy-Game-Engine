#include "stdafx.h"

#include "Win32GCCollector.h"


namespace mray{

void Win32GCCollector::inner_eraseAddress(void*addr){

}
void Win32GCCollector::inner_addToDeadList(IInfoTemplate*info){

}


Win32GCCollector::Win32GCCollector(){

}
Win32GCCollector::~Win32GCCollector(){

}

bool Win32GCCollector::isMultiThreaded(){
	return false;
}
GCInfo*Win32GCCollector::getInfo(const void* addr){
	return 0;

}
void Win32GCCollector::addInfo(GCInfo*info){

}

bool Win32GCCollector::collect(){
	return false;

}

int Win32GCCollector::activeSize(){
	return 0;
}
int Win32GCCollector::deadSize(){
	return 0;
}


}