

#ifndef ___GCInfo___
#define ___GCInfo___

#include "compileConfig.h"

namespace mray{


class MRAY_DLL GCInfo{
	int refCount;
	const void* m_memAddr;
	//bool m_isArray;
	//unsigned int m_arraySize;
public:
	GCInfo(const void* addr);

	const void*getAddr()const ;
	//bool isArray();
	//unsigned int arraySize();

	int getRefCount()const {return refCount;}
	
	void addRef();
	template <class T>
	void subRef(bool imediateRemove=false){
		refCount--;
		if(refCount<=0)
			IGCCollector::getInstance().addToDeadList<T>(this,imediateRemove);
	}
};

}




#endif





