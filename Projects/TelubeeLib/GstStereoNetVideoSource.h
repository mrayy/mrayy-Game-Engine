

/********************************************************************
	created:	2014/02/09
	created:	9:2:2014   19:28
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\GstStereoNetVideoSource.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	GstStereoNetVideoSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GstStereoNetVideoSource__
#define __GstStereoNetVideoSource__

#include "ICameraVideoSource.h"

namespace mray
{
	namespace video
	{
		class ICameraVideoGrabber;
	}
namespace TBee
{
	class GStreamVideoProvider;

class GstStereoNetVideoSource:public ICameraVideoSource
{
protected:
	GStreamVideoProvider* m_providers;
	video::ITexturePtr m_remoteTex;
	core::string m_ip;
	bool m_isStereo;
	int m_captureFPS;
	int m_frameCount;
	float m_timeAcc;
	float m_lastT;

public:
	GstStereoNetVideoSource(const core::string& ip = "127.0.0.1");
	virtual~GstStereoNetVideoSource();

	void SetIP(const core::string& ip){ m_ip = ip; }

	void Init();
	void Open();
	void Close();
	bool Blit(int eye);

	virtual math::vector2d GetEyeScalingFactor(int i);
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i);
	virtual float GetCaptureFrameRate(int i) { return m_captureFPS; }

	virtual bool IsLocal(){ return false; }
	virtual void SetIsStereo(bool stereo);
	virtual bool IsStereo(){ return m_isStereo; }

};

}
}


#endif
