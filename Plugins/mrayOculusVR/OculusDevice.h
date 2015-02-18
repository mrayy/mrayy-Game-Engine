

/********************************************************************
	created:	2013/10/06
	created:	6:10:2013   17:24
	filename: 	C:\Development\mrayEngine\Plugins\mrayOculusVR\OculusDevice.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOculusVR
	file base:	OculusDevice
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusDevice__
#define __OculusDevice__


#include "OVR.h"
#include "ListenerContainer.h"


namespace mray
{
namespace video
{

	class OculusDeviceImpl;
	class OculusManager;
	class OculusDevice;

	enum EMagManualCalibMessage
	{
		MagCalib_LookFrontWaiting,
		MagCalib_LookUp,
		MagCalib_LookLeft,
		MagCalib_LookRight
	};
	class IOculusDeviceListener
	{
	public:
		virtual void OnMagCalibrated(OculusDevice* device,const math::vector3d &magCenter){}

		virtual void OnMagManualCalibrationMessage(OculusDevice* device,EMagManualCalibMessage msg){}
	};

	struct OculusDeviceData
	{
	public: 
		OVR::HMDInfo deviceInfo;

		math::vector2d distortionFit;
		float		   distortionFitScale;
		math::vector2d lensOffset;

		float predictionTime;
		float accelGain;
	};
class OculusDevice:public ListenerContainer<IOculusDeviceListener*>
{
protected:
	OculusDeviceImpl* m_impl;
	friend OculusDeviceImpl;
	DECLARE_FIRE_METHOD(OnMagCalibrated,(OculusDevice* device,const math::vector3d &magCenter),(device,magCenter));
	DECLARE_FIRE_METHOD(OnMagManualCalibrationMessage,(OculusDevice* device,EMagManualCalibMessage msg),(device,msg));
public:
	OculusDevice(OVR::HMDDevice* device,OculusManager*mngr);
	virtual~OculusDevice();

	bool IsConnected();
	OVR::HMDDevice* GetDevice();
	const OculusDeviceData& GetDeviceInfo()const ;
	float GetVerticalFOV(const math::vector2d& screenSize);
	float GetDistortionScale(const math::vector2d& screenSize);
	float GetAspectRatio();

	void Update(float dt);

	math::quaternion GetOrientation();
	math::vector3d GetAcceleration();

	void StartMagManualCalibration();
	void SetMagAutoCalibration();

	void ResetOrientation();
};

}
}


#endif
