#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include <iostream>
#include "GCCollector.h"
#include "IThread.h"
#include "GCPtr.h"
#include "CMemory.h"
#include "NullMutex.h"

namespace mray{



bool IGCCollector::m_isDestroyed=false;

void IGCCollector::Initialize()
{
	if(!m_instance)
		m_instance=new GCCollector();
}

class GCThreadFunction:public OS::IThreadFunction
{
protected:
	virtual void setup(){
	}
	virtual void execute(OS::IThread*caller,void*arg){
		int timer=0;
		int MaxSleepCount=0;
		int MaxDeadSize=10;
		while(caller->isActive()){
			int deadSize=GCCollector::getInstance().deadSize();
			if(deadSize>=MaxDeadSize || timer>=MaxSleepCount && deadSize>0){
#ifdef _DEBUG	
		//std::cout<<"Before - ActiveList= "<<GCCollector::getInstance().activeSize()<<" - deadList= "<<GCCollector::getInstance().deadSize()<<std::endl;
#endif
			GCCollector::getInstance().collect();
#ifdef _DEBUG	
		//std::cout<<"After  - ActiveList= "<<GCCollector::getInstance().activeSize()<<" - deadList= "<<GCCollector::getInstance().deadSize()<<std::endl;
#endif
			timer=0;
			}
			timer++;
			Sleep(100);
		}
		do{
			GCCollector::getInstance().collect();
#ifdef _DEBUG	
		//std::cout<<"GCThread done - ActiveList= "<<GCCollector::getInstance().activeSize()<<" - deadList= "<<GCCollector::getInstance().deadSize()<<std::endl;
#endif
		}
		while(GCCollector::getInstance().deadSize()>0);

#ifdef _DEBUG
//	CMemory::GenerateReport("mem.log");
#endif
	}


public:
	GCThreadFunction(){
	}
	virtual~GCThreadFunction(){

#ifdef _DEBUG
		//std::cout<<"thread Done!"<<std::endl;
#endif
	}
};


GCCollector::GCCollector():m_memList(0),m_deadList(0)
{
	m_thread=0;
	m_multiThreaded=false;
	startupMT();
}


GCCollector::~GCCollector(){
	if(m_thread){
		m_thread->terminate();
		delete m_thread;
		m_thread=0;
	}else{
		do{
			GCCollector::getInstance().collect();
#ifdef _DEBUG	
		//std::cout<<"GCThread done - ActiveList= "<<GCCollector::getInstance().activeSize()<<" - deadList= "<<GCCollector::getInstance().deadSize()<<std::endl;
#endif
		}
		while(GCCollector::getInstance().deadSize()>0);
	}
	//delete the NULL Pointer
	GCInfo*info= _getInfo(0);
	if(info)
		delete info;
	m_memList.lock();
// 	TMemoryListIT it= m_memList().begin();
// 	TMemoryListIT end= m_memList().end();
//  	for(;it!=end;++it){
//  		GCInfo*info=it->second;
//  	}
	m_memList().clear();
	m_memList.unlock();
}


GCInfo*GCCollector::_getInfo(const void* addr){
	m_memList.lock();
	TMemoryListIT it=m_memList().find(addr);
	m_memList.unlock();
	if(it==m_memList().end())return 0;
	return it->second;
}
void GCCollector::_addInfo(GCInfo*info){
	m_memList.lock();
	m_memList().insert(TMemoryList::value_type(info->getAddr(),info));
	m_memList.unlock();
}

bool GCCollector::collect(){
#ifdef _DEBUG
	//printf("GC::collect()\n");
#endif
/*
	m_deadList.lock();
	bool freed=false;
	do{
		TDeadMemoryListIT it=m_deadList().begin();
		for(;it!=m_deadList().end();){
			IInfoTemplate*info= (*it);

			info->dispose();
			delete info;

			freed=true;
			TDeadMemoryListIT it2=it;
			it2++;
			m_deadList().remove(it);
			it=it2;
		}
	}while(m_deadList().getSize()>0);
	m_deadList.unlock();
	return freed;*/
	bool freed=false;
	m_deadList.lock();
	TDeadMemoryListIT it=m_deadList().begin();
	IInfoTemplate*lastinfo=0;
	for(;it!=m_deadList().end();++it){
		IInfoTemplate*info= (*it);

		info->dispose();

		lastinfo=info;
		//delete info;

		freed=true;/*
		TDeadMemoryListIT it2=it;
		it2++;
		m_deadList().remove(it);
		it=it2;*/
	}
	m_deadList().clear();
	m_deadList.unlock();
	return freed;
}

int GCCollector::activeSize(){
	return m_memList().size();
}
int GCCollector::deadSize(){
	return m_deadList().size();
}


void GCCollector::startupMT()
{
#ifdef MRAY_GC_MT
	if(m_thread)return;
	m_thread = new GCThread();
	m_thread->start(0);
#endif
}
void GCCollector::shutdown(){

	m_isDestroyed=true;
	IGCCollector::shutdown();
	if(m_instance){
		//delete m_instance;
		//m_instance=0;
	}
}


void GCCollector::SetMultiThreaded(bool mt)
{
	m_multiThreaded=mt;
	if(m_thread)
	{
		if (m_multiThreaded)
		{
			m_thread->start(0);
		}else
			m_thread->terminate();
	}
}
void GCCollector::SetThread(OS::IThread*t)
{
	m_thread=t;
	if(m_thread)
		m_thread->setThreadFunction(new GCThreadFunction());
	if(m_thread)
	{
		if (m_multiThreaded)
		{
			m_thread->start(0);
		}else
			m_thread->terminate();
	}
}
OS::IThread* GCCollector::GetThread()
{
	return m_thread;
}

void GCCollector::SetMutex(OS::IMutex*memMtx,OS::IMutex*deadMtx)
{
	m_memList.setMutex(memMtx);
	m_deadList.setMutex(deadMtx);
}



}


