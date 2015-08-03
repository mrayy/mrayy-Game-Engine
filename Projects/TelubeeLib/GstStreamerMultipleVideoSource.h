

#ifndef GstStreamerMultipleVideoSource_h__
#define GstStreamerMultipleVideoSource_h__

#include "ICameraVideoSource.h"
#include "GstPlayerBin.h"
#include "GstNetworkMultipleVideoPlayer.h"
#include "VideoGrabberTexture.h"

namespace mray
{
namespace TBee
{
class GstStreamerMultipleVideoSource :public ICameraVideoSource
{
protected:
	GCPtr<video::GstPlayerBin> m_player;
	std::vector<GCPtr<video::VideoGrabberTexture>> m_playerGrabber;
	video::GstNetworkMultipleVideoPlayer*m_playerImpl;
	core::string m_ip;
	uint m_vport;
	uint m_aport;
	uint m_clockPort;
	uint m_count;
	bool m_rtcp;
	bool m_isStereo;
	bool m_useAudio;
public:
	GstStreamerMultipleVideoSource(const core::string& ip, uint videoport,uint count, uint audioport, uint clockPort, bool rtcp, bool useAudio);
	virtual ~GstStreamerMultipleVideoSource();

	void SetIP(const core::string& ip, uint videoport, uint count, uint audioport, uint clockPort, bool rtcp){ m_ip = ip; m_vport = videoport; m_count = count; m_aport = audioport; m_rtcp = rtcp; m_clockPort = clockPort; }

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

	video::GstNetworkMultipleVideoPlayer* GetVideoPlayer();
};

}
}

#endif // GstStreamerMultipleVideoSource_h__

