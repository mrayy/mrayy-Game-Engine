#include "stdafx.h"

#include "TheoraFrameQueue.h"
#include "TheoraVideo.h"

#include <IThreadManager.h>

namespace mray{
namespace video{


TheoraFrameQueue::TheoraFrameQueue(TheoraVideo*v,int frames){
	m_backColor=0;
	m_owner=v;
	m_mutex = OS::IThreadManager::getInstance().createMutex();

	setSize(frames);
}
TheoraFrameQueue::~TheoraFrameQueue(){
	m_frames.clear();
}

GCPtr<TheoraFrame> TheoraFrameQueue::requestFirstAvaliableFrame(){
	GCPtr<TheoraFrame> frame;
	m_mutex->lock();
	if((*m_frames.begin())->ready){
		frame=(*m_frames.begin());
	}
	m_mutex->unlock();
	return frame;
}
GCPtr<TheoraFrame> TheoraFrameQueue::requestAnyFreeFrame(){
	GCPtr<TheoraFrame> frame;
	m_mutex->lock();
	FramesList::iterator it= m_frames.begin();
	for(;it!=m_frames.end();++it){
		if(!(*it)->inUse){
			frame=(*it);
			break;
		}
	}
	m_mutex->unlock();
	return frame;
}

void TheoraFrameQueue::setFillColor(uint c){
	m_backColor=c;
	m_mutex->lock();
	FramesList::iterator it= m_frames.begin();
	for(;it!=m_frames.end();++it){
		(*it)->fillBackColor(c);
	}
	m_mutex->unlock();
}
uint TheoraFrameQueue::getFillColor(){
	return m_backColor;
}
uint TheoraFrameQueue::getFramesCount(){
	return m_frames.size();
}

void TheoraFrameQueue::pop(){
	GCPtr<TheoraFrame> frame;
	m_mutex->lock();
	frame=(*m_frames.begin());
	m_frames.erase(m_frames.begin());
	frame->ready=false;
	frame->inUse=false;
	m_frames.push_back(frame);
	m_mutex->unlock();
}
void TheoraFrameQueue::clear(){
	FramesList::iterator it= m_frames.begin();
	m_mutex->lock();
	for(;it!=m_frames.end();++it){
		(*it)->ready=false;
		(*it)->inUse=false;
	}
	m_mutex->unlock();
}

void TheoraFrameQueue::setSize(int n){
	m_mutex->lock();
	int toAdd=n-m_frames.size();
	for(int i=0;i<toAdd;++i){
		m_frames.push_back(MRAY_new TheoraFrame(m_owner));
	}
	if(toAdd<0){
		toAdd=-toAdd;
		for(int i=0;i<toAdd;++i){
			m_frames.erase(m_frames.begin());
		}
	}
	m_mutex->unlock();
}
int TheoraFrameQueue::getSize(){
	return m_frames.size();
}
int TheoraFrameQueue::getInUseCount(){
	int n=0;
	m_mutex->lock();
	FramesList::iterator it= m_frames.begin();
	for(;it!=m_frames.end();++it){
		if((*it)->inUse)
			++n;
	}
	m_mutex->unlock();
	return n;
}

}
}
