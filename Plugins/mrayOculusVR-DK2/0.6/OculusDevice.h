

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
	class RenderWindow;

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
		ovrFovPort eyeFov[2];
		math::vector2di texSize;//recommended texture size to be used for rendering (1 pixel density)
		math::vector2di eyeRenderSize[2];
		math::vector2di hmdResolution;
	};
	enum class OVREye
	{
		Left,
		Right
	};
class OculusDevice:public ListenerContainer<IOculusDeviceListener*>
{
protected:
	OculusDeviceImpl* m_impl;
	friend OculusDeviceImpl;
	DECLARE_FIRE_METHOD(OnMagCalibrated,(OculusDevice* device,const math::vector3d &magCenter),(device,magCenter));
	DECLARE_FIRE_METHOD(OnMagManualCalibrationMessage,(OculusDevice* device,EMagManualCalibMessage msg),(device,msg));
public:
	OculusDevice(ovrHmd device,OculusManager*mngr);
	virtual~OculusDevice();

	bool IsConnected();
	ovrHmd GetDevice();
	const OculusDeviceData& GetDeviceInfo()const ;

	bool AttachToWindow(video::RenderWindow* window);
	void GetDevicePosSize(math::vector2di& pos, math::vector2di& size);

	void Update(float dt);

	void BeginFrame();
	void EndFrame(const ovrTexture* eyes);

	void SetLowPresistenceMode(bool on);

	bool IsHMDConnected();
	bool IsTrackingConnected();
	bool IsPositionTracking();
	bool IsExtendedDesktop();

	uint GetDisplayID(); //return the assigned monitor id for this hmd

	float GetIPD();
	float GetEyeHeight();
	//bool GetEyePos(OVREye eye, math::vector3d& pos, math::quaternion& ori);
	math::vector3d GetCameraPosition();
	math::quaternion GetCameraOrientation();
	math::vector3d GetPosition();
	math::quaternion GetOrientation();
	math::vector3d GetAcceleration();
	math::vector3d GetAngularVelocity();

	void GetRenderScaleAndOffset(ovrFovPort fov, const math::vector2di& textureSize, const math::recti& renderVP, math::vector2d& scale, math::vector2d& offset);

	void ResetOrientation();
};

}
}


#endif
