

#ifndef LocalSingleCameraVideoSource_h__
#define LocalSingleCameraVideoSource_h__


#include "ICameraVideoSource.h"
#include "VideoGrabberTexture.h"


namespace mray
{
namespace video
{
class ICameraVideoGrabber;
}
namespace TBee
{

class LocalSingleCameraVideoSource :public ICameraVideoSource
{
protected:
	struct CameraInfo
	{
		CameraInfo() :id(0), camera(0), videoGrabber(0)
		{
		}
		int id;

		video::VideoGrabberTexture* videoGrabber;

		video::ICameraVideoGrabber* camera;
	};


	math::vector2d m_cameraResolution;
	int m_cameraFPS;
	CameraInfo m_cameraSource;
public:
	LocalSingleCameraVideoSource(int c1 = 0);
	virtual~LocalSingleCameraVideoSource();

	void Init();
	void Open();
	void Close();
	video::ICameraVideoGrabber* GetCamera(){ return m_cameraSource.camera; }

	virtual math::vector2d GetEyeScalingFactor(int i) { return math::vector2d(1, 1); }
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i) { return math::rectf(0, 0, 1, 1); }
	virtual float GetCaptureFrameRate(int i);

	virtual bool IsLocal(){ return true; }

	bool Blit(int eye);

	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}

#endif // LocalSingleCameraVideoSource_h__
