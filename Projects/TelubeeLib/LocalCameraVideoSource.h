

/********************************************************************
	created:	2014/02/09
	created:	9:2:2014   19:14
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\LocalCameraVideoSource.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	LocalCameraVideoSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LocalCameraVideoSource__
#define __LocalCameraVideoSource__


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

	enum EUSBCameraType
	{
		ECam_DirectShow,
		ECam_PointGray
	};

class LocalCameraVideoSource:public ICameraVideoSource
{
protected:
	struct CameraInfo
	{
		CameraInfo() :id(0), camera(0), videoGrabber(0)
		{
		}
		int id;
		math::vector2di offsets;

		video::VideoGrabberTexture* videoGrabber;

		video::ICameraVideoGrabber* camera;
	};

	bool m_started;
	EUSBCameraType m_camConnection;
	math::vector2d m_cameraResolution;
	int m_cameraFPS;
	float m_exposureValue;
	float m_gainValue;
	float m_gammaValue;
	float m_WBValue;
	CameraInfo m_cameraSource[2];
public:
	LocalCameraVideoSource(int c1=0,int c2=1,EUSBCameraType camType=ECam_DirectShow);
	virtual~LocalCameraVideoSource();

	void SetCameraConnectionType(EUSBCameraType cam){ m_camConnection = cam; }
	EUSBCameraType GetCameraConnectionType(){ return m_camConnection; }
	void Init();
	void Open();
	void Close();
	video::ICameraVideoGrabber* GetCamera(int i){ return m_cameraSource[i].camera; }

	void SetCameraResolution(const math::vector2d& res, int fps);

	void SetCameraID(int i, int cam);
	int GetCameraID(int i){ return m_cameraSource[i].id; }

	virtual math::vector2d GetEyeScalingFactor(int i) { return math::vector2d(1, 1); }
	virtual math::vector2d GetEyeResolution(int i);
	virtual video::ITexturePtr GetEyeTexture(int i);
	virtual math::rectf GetEyeTexCoords(int i) { return math::rectf(0, 0, 1, 1); }

	virtual float GetCaptureFrameRate(int i);
	virtual bool IsLocal(){ return true; }

	bool Blit(int eye);

	void SetCameraParameterValue(const core::string& namne, const core::string& value);
	core::string GetCameraParameterValue(const core::string& namne);

	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}

#endif
