

#ifndef GstStreamerVideoSource_h__
#define GstStreamerVideoSource_h__

#include "ICameraVideoSource.h"
#include "GstPlayerBin.h"
#include "GstNetworkVideoPlayer.h"

namespace mray
{
	namespace video
	{
		class VideoGrabberTexture;
	}
namespace TBee
{
class GstStreamerVideoSource :public ICameraVideoSource
{
protected:
	GCPtr<video::GstPlayerBin> m_player;
	video::VideoGrabberTexture* m_playerGrabber;
	core::string m_ip;
	uint m_vport;
	uint m_aport;
	uint m_clockPort;
	bool m_rtcp;
	bool m_isStereo;
	bool m_useAudio;
public:
	GstStreamerVideoSource(const core::string& ip, uint videoport, uint audioport, uint clockPort, bool rtcp, bool useAudio);
	virtual ~GstStreamerVideoSource();

	void SetIP(const core::string& ip, uint videoport, uint audioport, uint clockPort, bool rtcp){ m_ip = ip; m_vport = videoport; m_aport = audioport; m_rtcp = rtcp; m_clockPort = clockPort; }

	void Init();
	void Open();
	void Close();
	bool Blit(int eye);

	virtual math::vector2d GetEyeScalingFactor(int i);
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i);
	virtual float GetCaptureFrameRate(int i) ;

	virtual bool IsLocal(){ return false; }
	virtual void SetIsStereo(bool stereo);
	virtual bool IsStereo(){ return m_isStereo; }

	video::GstNetworkVideoPlayer* GetVideoPlayer();
};

}
}

#endif // GstStreamerVideoSource_h__

