

#ifndef GstNetworkAudioPlayer_h__
#define GstNetworkAudioPlayer_h__



#include "IGStreamerPlayer.h"
#include "mstring.h"

namespace mray
{
namespace video
{

class GstNetworkAudioPlayerImpl;

class GstNetworkAudioPlayer :public IGStreamerPlayer
{
protected:
	GstNetworkAudioPlayerImpl* m_impl;
public:
	GstNetworkAudioPlayer();
	virtual ~GstNetworkAudioPlayer();

	//set ip address for the target host
	// audioport: port for the audio stream , audio rtcp is allocated as audioPort+1 and audioPort+2
	void SetIPAddress(const core::string& ip,  int audioPort );
	bool CreateStream();

	virtual bool IsStream();

	void SetVolume(float vol);

	virtual void Play();
	virtual void Pause();
	virtual void Stop();
	virtual bool IsLoaded();
	virtual bool IsPlaying();
	virtual void Close();

};

}
}
#endif // GstNetworkAudioPlayer_h__
