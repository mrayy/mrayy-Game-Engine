#include "stdafx.h"

#include "IGCCollector.h"
#include <assert.h>


namespace mray
{
	IGCCollector *IGCCollector::m_instance=0;



	IGCCollector&IGCCollector::getInstance(){
		assert(m_instance!=0 && "GCCollector Not Created!");
		return *m_instance;
	}
	void IGCCollector::shutdown(){
		if(m_instance){
			//delete m_instance;
			m_instance->collect();
		}
		
		//m_instance=0;
	}


	GCInfo* IGCCollector::GetInfo(const void* addr)
	{
		if(!m_instance)
			Initialize();
		return m_instance->_getInfo(addr);
	}
	void IGCCollector::AddInfo(GCInfo*info)
	{
		if(!m_instance)
			Initialize();
		return m_instance->_addInfo(info);
	}
}
