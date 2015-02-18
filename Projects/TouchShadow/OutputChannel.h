

/********************************************************************
	created:	2013/08/30
	created:	30:8:2013   18:04
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\OutputChannel.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	OutputChannel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OutputChannel__
#define __OutputChannel__

#include "Stk.h"
#include <list>

namespace mray
{
	enum EChannelDestination
	{
		EChannel_Left,
		EChannel_Right
	};
	class AudioSoundInterface;
typedef std::vector<short> FramesList;
class OutputChannel
{
protected:

	struct ScheduledFrames
	{
		ScheduledFrames():pos(0){}
		ScheduledFrames(FramesList* f):pos(0),frames(f),strength(1)
		{}
		FramesList* frames;
		int pos;
		float strength;
	};
	int m_channelID;
	EChannelDestination m_channelDst;
	OS::IMutex* m_mutex;
	AudioSoundInterface* m_owner;

	std::list<ScheduledFrames> m_scheduledFrames;
public:
	OutputChannel(EChannelDestination dst,AudioSoundInterface* owner);
	virtual~OutputChannel();


	AudioSoundInterface* GetOwner()const{return m_owner;}

	void SetChannelInfo(int id)
	{
		m_channelID=id;
	}
	int GetChannelID()const{return m_channelID;}
	EChannelDestination GetDestinationChannel()const{return m_channelDst;}
	
	uint ReadFrames(short* frames,uint len);

	void ScheduleFrames(FramesList* f,float strength);
	int GetSchedulesCount(){return m_scheduledFrames.size();}
	void FlushScheduledFrames(){Lock();m_scheduledFrames.clear();Unlock();}

	void Lock();
	void Unlock();

	bool isDone(){return m_scheduledFrames.size()==0;}

};

}


#endif
