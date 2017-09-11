#include "stdafx.h"

#include "IThreadManager.h"
#include "TraceManager.h"
#include "IOSystem.h"

#include "MultiThreadListener.h"



namespace mray{
namespace OS{

IThreadManager::IThreadManager(){
}
IThreadManager::~IThreadManager(){
//	traceFunction(OS);
	shutdown();
}

void IThreadManager::killThread(IThread* thread){
	traceFunction(OS);
	if(!thread)
		return;
	ThreadsList::iterator it=m_threads.begin();
	for (;it!=m_threads.end();++it)
	{
		if((*it)==thread){
			m_threads.erase(it);
			break;
		}
	}
	thread->terminate();
}

void IThreadManager::shutdown(){
	traceFunction(OS);
	ThreadsList::iterator it=m_threads.begin();
	bool isDone=false;
	while(!isDone)
	{
		isDone=true;
		for (;it!=m_threads.end();++it)
		{
			if((*it)->isActive()){
				(*it)->terminate();
				isDone=false;
			}
		}
	}
	m_threads.clear();
}

void IThreadManager::registerThreadListener(MultiThreadListener* l){
	m_listener.push_back(l);
}
void IThreadManager::removeThreadListener(MultiThreadListener* l){
	ThreadsListenerList::iterator p=m_listener.begin();
	ThreadsListenerList::iterator end=m_listener.end();
	for(;p!=end;++p)
	{
		if((*p)==l){
			m_listener.erase(p);
			break;
		}
	}
}

void IThreadManager::preThreadStart(){
	ThreadsListenerList::iterator p=m_listener.begin();
	ThreadsListenerList::iterator end=m_listener.end();
	for(;p!=end;++p)
	{
		(*p)->preThreadStart();
	}
}
void IThreadManager::postThreadStart(){
	ThreadsListenerList::iterator p=m_listener.begin();
	ThreadsListenerList::iterator end=m_listener.end();
	for(;p!=end;++p)
	{
		(*p)->postThreadStart();
	}
}
void IThreadManager::registerThread(){
	ThreadsListenerList::iterator p=m_listener.begin();
	ThreadsListenerList::iterator end=m_listener.end();
	for(;p!=end;++p)
	{
		(*p)->registerThread();
	}
}
void IThreadManager::unregisterThread(){
	ThreadsListenerList::iterator p=m_listener.begin();
	ThreadsListenerList::iterator end=m_listener.end();
	for(;p!=end;++p)
	{
		(*p)->unregisterThread();
	}
}

}
}
