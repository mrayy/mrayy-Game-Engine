

#include "stdafx.h"
#include "OutputChannel.h"
#include "IThreadManager.h"
#include "MutexLocks.h"

namespace mray
{
OutputChannel::OutputChannel(EChannelDestination dst,AudioSoundInterface* owner):m_owner(owner)
{
	m_channelDst=dst;
	m_mutex=OS::IThreadManager::getInstance().createMutex();
}
OutputChannel::~OutputChannel()
{
	delete m_mutex;
}

void OutputChannel::Lock()
{
	m_mutex->lock();
}

void OutputChannel::Unlock()
{
	m_mutex->unlock();
}

uint OutputChannel::ReadFrames(short* frames,uint len)
{
	OS::ScopedLock l(m_mutex);
	int samples=0;

	while(m_scheduledFrames.size()!=0 && samples<len)
	{
		ScheduledFrames& f= *m_scheduledFrames.begin();
		int read=math::Min<int>(len,math::Max<int>(0,f.frames->size()-f.pos));
		if(read==0)
		{
			m_scheduledFrames.pop_front();
			continue;
		}
		if(f.strength==1)
			memcpy(frames,&(*f.frames)[f.pos],read*sizeof(short));
		else
		{
			short* src=&(*f.frames)[f.pos];
			for(int i=0;i<read;++i)
				frames[i]=*src*f.strength;
		}
		samples+=read;
		frames+=read;
		f.pos+=read;
		len-=read;
	}
	return samples;
}

void OutputChannel::ScheduleFrames(FramesList* f,float strength)
{
	Lock();
	ScheduledFrames frame(f);
	frame.strength=strength;
	m_scheduledFrames.push_back(frame);
	Unlock();
}

}

