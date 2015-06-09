

/********************************************************************
	created:	2014/02/09
	created:	9:2:2014   19:28
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\GstSingleNetVideoSource.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	GstSingleNetVideoSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GstSingleNetVideoSource__
#define __GstSingleNetVideoSource__

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

class GstSingleNetVideoSource:public ICameraVideoSource
{
protected:
	GStreamVideoProvider* m_providers;
	video::ITexturePtr m_remoteTex;
	core::string m_ip;
	int m_port;

	int m_captureFPS;
	int m_frameCount;
	float m_timeAcc;
	float m_lastT;
public:
	GstSingleNetVideoSource(const core::string& ip = "127.0.0.1",int port=5000);
	virtual~GstSingleNetVideoSource();

	void SetIP(const core::string& ip){ m_ip = ip; }
	void SetPort(int port){ m_port = port; }

	void Init();
	void Open();
	void Close();
	bool Blit(int eye);

	virtual math::vector2d GetEyeScalingFactor(int i){ return math::vector2d(1, 1); }
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i) { return math::rectf(0, 0, 1, 1); }
	virtual bool IsLocal(){ return false; }
	virtual float GetCaptureFrameRate(int i){ return m_captureFPS; }

};

}
}


#endif
