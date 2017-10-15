

#ifndef __GstCustomDataPlayer__
#define __GstCustomDataPlayer__

#include "IGStreamerPlayer.h"
#include "IVideoGrabber.h"

namespace mray
{
namespace video
{

class GstCustomDataPlayerImpl;

class GstCustomDataPlayer :public IGStreamerPlayer
{
protected:
	GstCustomDataPlayerImpl* m_impl;

	GstPipelineHandler* GetPipeline();

public:
	GstCustomDataPlayer();
	virtual ~GstCustomDataPlayer();

	void SetApplicationDataType(const std::string& dataType, bool autotimestamp, int payload = 98);// application/x-"dataType"

	void SetPort(int port);

	bool CreateStream();

	virtual bool IsStream();

	void SetVolume(float vol);

	virtual void Play();
	virtual void Pause();
	virtual void Stop();
	virtual bool IsLoaded();
	virtual bool IsPlaying();
	virtual void Close();

	virtual int GetPort(int i);

	bool GrabFrame();
	virtual bool HasNewFrame();
	virtual ulong GetBufferID();// incremented once per frame

	int GetDataLength();
	int GetData(void* data,int length);

};

}
}


#endif
