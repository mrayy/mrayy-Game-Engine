

/********************************************************************
	created:	2012/11/18
	created:	18:11:2012   1:19
	filename: 	D:\Development\mrayEngine\Plugins\mraySound\DirectSoundOutputStream.h
	file path:	D:\Development\mrayEngine\Plugins\mraySound
	file base:	DirectSoundOutputStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DirectSoundOutputStream__
#define __DirectSoundOutputStream__



#include "compileConfig.h"
#include "mString.h"

namespace mray
{
namespace sound
{

	struct OutputStreamDeviceInfo
	{
		int id;
		core::string name;
		core::string description;
	};
	struct DirectSoundOutDeviceData;
	class DirectSoundOutputStream;

	class IDirectSoundOutputListener
	{
	public:
		virtual uint GetAudioSamples(DirectSoundOutputStream* stream,uchar* buffer,uint samples)=0;
		virtual bool OnTimerTick(DirectSoundOutputStream* stream){return true;} // this call back check whether or not to stop the stream
	};

	class DirectSoundOutputStreamData;
class MRAY_SOUND_DLL DirectSoundOutputStream
{
protected:
	DirectSoundOutputStreamData* m_data;
	core::string m_name;

	IDirectSoundOutputListener* m_listener;

public:
	DirectSoundOutputStream();
	virtual~DirectSoundOutputStream();

	void SetName(const core::string& name){m_name=name;}
	const core::string& GetName()const{return m_name;}

	virtual void SetListener(IDirectSoundOutputListener* l){m_listener=l;}
	virtual IDirectSoundOutputListener* GetListener(){return m_listener;}

	virtual void ListDevices(std::vector<OutputStreamDeviceInfo> &lst);
	virtual bool CreateOutputStream(int device,int bps,uchar numChannels,uint samplesPerSec);
	virtual bool StartOutputStream();
	virtual void PlaySamples(const void* buffer,uint len,bool looping);
	virtual void StopOutputStream();
	virtual void PauseOutputStream();

	virtual void SetFrequency(uint v);
	virtual uint GetFrequency();
	virtual void SetVolume(uint v);
	virtual uint GetVolume();
	virtual void SetPan(uint v);
	virtual uint GetPan();
	virtual uint GetCurrentPosition();
	virtual uint GetPlayedSamples();

	virtual uint GetBytesPerSecond();

	virtual bool CanStream(){return true;}

	virtual bool IsStreaming(){return true;}
	virtual bool IsPlaying();
	virtual bool IsLooping();

	void _TimerCallback();
};

}
}

#endif
