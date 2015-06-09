

#ifndef __GSTGstLocalCameraVideoSource__
#define __GSTGstLocalCameraVideoSource__

#include "ICameraVideoSource.h"
#include "VideoGrabberTexture.h"
#include "GstPlayerBin.h"


namespace mray
{

namespace TBee
{

class GstLocalCameraVideoSource :public ICameraVideoSource
{
protected:

	GCPtr<video::GstPlayerBin> m_player;
	video::VideoGrabberTexture* m_playerGrabber;
	bool m_started;

	math::vector2d m_cameraResolution;
	int m_cameraFPS;
	int m_cameraSource[2];
public:
	GstLocalCameraVideoSource(int c1 = 0, int c2 = 1);
	virtual~GstLocalCameraVideoSource();

	void Init();
	void Open();
	void Close();
	bool Blit(int eye);

	void SetCameraResolution(const math::vector2d& res, int fps);

	void SetCameraID(int i, int cam);
	int GetCameraID(int i){ return m_cameraSource[i]; }

	virtual math::vector2d GetEyeScalingFactor(int i);
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i);
	virtual float GetCaptureFrameRate(int i);

	virtual bool IsStereo(){ return m_cameraSource[0] != m_cameraSource[1]; }

	virtual bool IsLocal(){ return true; }


	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}


#endif
