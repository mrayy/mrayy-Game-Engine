

#ifndef CameraConfigurationManager_h__
#define CameraConfigurationManager_h__



namespace mray
{
namespace TBee
{

class TelubeeCameraConfiguration
{
public:
	enum ECameraRotation
	{
		None,
		CW,	//90 degrees rotation Clock Wise
		CCW,//90 degrees rotation Counter Clock Wise
		Flipped //180 degrees flipped
	};

	enum ECameraType
	{
		POVCamera,
		OmniCamera,
	};
	enum ECameraCaptureType
	{
		CaptureRaw,
		CaptureJpeg,
		CaptureH264
	};

	//http://docs.opencv.org/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
	float fov;			//horizontal field of view for the camera measured in degrees
	float cameraOffset;	//physical offset from human eye
	float stereoOffset;	//physical distance between both eyes

	ECameraRotation cameraRotation[2];

	math::vector2d OpticalCenter;
	math::vector2d FocalCoeff;
	math::vector4d KPCoeff;
	math::vector4d PixelShift;
	core::string name;

	ECameraCaptureType captureType;//RAW,JPEG,H264
	ECameraType  cameraType; //FOV,OMNI

public:
	TelubeeCameraConfiguration();

	void LoadFromXML(xml::XMLElement*e);
	xml::XMLElement* ExportToXML(xml::XMLElement*e);
};


class CameraConfigurationManager
{
protected:
	typedef std::map<core::string, TelubeeCameraConfiguration*> CamMap;
	CamMap m_cameras;
public:
	CameraConfigurationManager();
	virtual ~CameraConfigurationManager();

	void Clear();

	TelubeeCameraConfiguration* GetCameraConfiguration(const core::string& name);
	
	void LoadConfigurations(const core::string& path);
};

}
}

#endif // CameraConfigurationManager_h__
