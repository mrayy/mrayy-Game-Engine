

#include "stdafx.h"
#include "OculusDevice.h"
#include "OculusManager.h"
#include "RenderWindow.h"
#include <windows.h>
#include "OVR_CAPI_GL.h"

using namespace OVR;

namespace mray
{
namespace video
{


	class OculusDeviceImpl
	{
		ovrHmd m_device;
		OculusDevice* m_owner;
		OculusManager* m_mngr;

		OculusDeviceData m_data;

		unsigned StartTrackingCaps;

	public:
		OculusDeviceImpl(ovrHmd device, OculusDevice* o, OculusManager*mngr)
		{
			m_mngr = mngr;
			m_owner = o;
			m_device = device;
			if (!m_device)
				return;

			StartTrackingCaps = 0;


			Init();

			UpdateOVRParams();
		}

		~OculusDeviceImpl()
		{
			ovrHmd_Destroy(m_device);
		}

		void UpdateOVRParams()
		{


			m_data.eyeFov[0] = m_device->DefaultEyeFov[0];
			m_data.eyeFov[1] = m_device->DefaultEyeFov[1];

			float DesiredPixelDensity = 1;
			// Configure Stereo settings. Default pixel density is 1.0f.
			Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(m_device, ovrEye_Left, m_data.eyeFov[0], DesiredPixelDensity);
			Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(m_device, ovrEye_Right, m_data.eyeFov[1], DesiredPixelDensity);

			Sizei  rtSize(recommenedTex0Size.w + recommenedTex1Size.w,
				Alg::Max(recommenedTex0Size.h, recommenedTex1Size.h));

			m_data.texSize.set(rtSize.w, rtSize.h);
			Sizei eyeRenderSize[2];

			// Don't draw more then recommended size; this also ensures that resolution reported
			// in the overlay HUD size is updated correctly for FOV/pixel density change.            
			eyeRenderSize[0] = Sizei::Min(Sizei(rtSize.w / 2, rtSize.h), recommenedTex0Size);
			eyeRenderSize[1] = Sizei::Min(Sizei(rtSize.w / 2, rtSize.h), recommenedTex1Size);
			m_data.eyeRenderSize[0].set(eyeRenderSize[0].w, eyeRenderSize[0].h);
			m_data.eyeRenderSize[1].set(eyeRenderSize[1].w, eyeRenderSize[1].h);

			m_data.hmdResolution.x = m_device->Resolution.w;
			m_data.hmdResolution.y = m_device->Resolution.h;



			bool IsLowPersistence = true;
			bool DynamicPrediction = false;
			bool VsyncEnabled = false;

			// Hmd caps.
			unsigned hmdCaps = (VsyncEnabled ? 0 : ovrHmdCap_NoVSync);
			if (IsLowPersistence)
				hmdCaps |= ovrHmdCap_LowPersistence;

			// ovrHmdCap_DynamicPrediction - enables internal latency feedback
			if (DynamicPrediction)
				hmdCaps |= ovrHmdCap_DynamicPrediction;

			// ovrHmdCap_DisplayOff - turns off the display
			//if (DisplaySleep)
			//hmdCaps |= ovrHmdCap_DisplayOff;

			//if (!MirrorToWindow)
			//hmdCaps |= ovrHmdCap_NoMirrorToWindow;

			ovrHmd_SetEnabledCaps(m_device, hmdCaps);


		}
		bool AttachToWindow(video::RenderWindow* window)
		{
			HWND* ptr;
			window->GetCustomParam("WINDOW", (void*)&ptr);
			if (!ovrHmd_AttachToWindow(m_device, ptr, NULL, NULL))
				return false;

			unsigned sensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection;
			if (!IsTrackingConnected())
				printf("Oculus tracking device not detected!\n");
			sensorCaps |= ovrTrackingCap_Position;

			if (StartTrackingCaps != sensorCaps)
			{
				ovrHmd_ConfigureTracking(m_device, sensorCaps, 0);
				StartTrackingCaps = sensorCaps;
			}

			ovrHmd_SetEnabledCaps(m_device, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);
			return true;

		}
		void GetDevicePosSize(math::vector2di& pos, math::vector2di& size)
		{
			pos.set(m_device->WindowsPos.x, m_device->WindowsPos.y);
			size.set(m_device->Resolution.w, m_device->Resolution.h);
		}
		const OculusDeviceData& GetDeviceInfo()const
		{
			return m_data;
		}

		void Init()
		{
			if (!m_device)
				return;

		}

		void SetLowPresistenceMode(bool on)
		{
			unsigned caps = ovrHmd_GetEnabledCaps(m_device);
			if (on)
				caps |= ovrHmdCap_LowPersistence;
			else caps &= ~ovrHmdCap_LowPersistence;
			ovrHmd_SetEnabledCaps(m_device,caps);
			StartTrackingCaps = caps;
		}

		uint GetDisplayID()
		{

			DISPLAY_DEVICE d;
			d.cb = sizeof(DISPLAY_DEVICE);
			int monitor = 0;
			uint i = 0;
			int result;

			do
			{

				result = EnumDisplayDevices(0, i, &d, 0);
				if (strstr(m_device->DisplayDeviceName,d.DeviceName))
				{
					return i;
				}
				++i;
			} while (result);
			return 0;
		}

		ovrHmd GetDevice()const { return m_device; }

		void Update(float dt)
		{
			if (!m_device)
				return;
		}
		void BeginFrame()
		{
			ovrHmd_BeginFrame(m_device, 0);
		}

		void EndFrame(const ovrTexture* eyes)
		{
			static ovrPosef eyeRenderPose[2];
			eyeRenderPose[0] = ovrHmd_GetEyePose(m_device, ovrEyeType::ovrEye_Left);
			eyeRenderPose[1] = ovrHmd_GetEyePose(m_device, ovrEyeType::ovrEye_Right);

			ovrHmd_EndFrame(m_device, eyeRenderPose, eyes);
		}

		float GetEyeHeight()
		{
			return ovrHmd_GetFloat(m_device, OVR_KEY_EYE_HEIGHT, OVR_DEFAULT_EYE_HEIGHT);
		}

		bool GetEyePos(OVREye e, math::vector3d& pos, math::quaternion& ori)
		{

			ovrEyeType eye = m_device->EyeRenderOrder[(int)e];
			ovrPosef p = ovrHmd_GetEyePose(m_device, eye);
			pos.set(p.Position.x, p.Position.y, p.Position.z);
			ori = math::quaternion(p.Orientation.w, p.Orientation.x, p.Orientation.y, p.Orientation.z);
			return true;
		}
		math::vector3d GetCameraPosition()
		{
			ovrTrackingState s = ovrHmd_GetTrackingState(m_device, 0);
			return math::vector3d(s.CameraPose.Position.x, s.CameraPose.Position.y, s.CameraPose.Position.z);

		}
		math::quaternion GetCameraOrientation()
		{

			ovrTrackingState s = ovrHmd_GetTrackingState(m_device, 0);
			return math::quaternion(s.CameraPose.Orientation.w, -s.CameraPose.Orientation.x, -s.CameraPose.Orientation.y, s.CameraPose.Orientation.z);

		}
		math::vector3d GetPosition()
		{

			ovrEyeType eye = m_device->EyeRenderOrder[0];
			ovrPosef p = ovrHmd_GetEyePose(m_device, eye);
			return math::vector3d(p.Position.x, p.Position.y, p.Position.z);
			ovrTrackingState s = ovrHmd_GetTrackingState(m_device, 0);
			return math::vector3d(s.HeadPose.ThePose.Position.x, s.HeadPose.ThePose.Position.y, -s.HeadPose.ThePose.Position.z);

		}
		math::quaternion GetOrientation()
		{

			ovrTrackingState s = ovrHmd_GetTrackingState(m_device, 0);
			return math::quaternion(s.HeadPose.ThePose.Orientation.w, s.HeadPose.ThePose.Orientation.x, s.HeadPose.ThePose.Orientation.y, s.HeadPose.ThePose.Orientation.z);

		}

		float GetIPD()
		{
			return ovrHmd_GetFloat(m_device, OVR_KEY_IPD, OVR_DEFAULT_IPD);
		}

		math::vector3d GetAcceleration()
		{
			ovrTrackingState s= ovrHmd_GetTrackingState(m_device, 0);
			return math::vector3d(s.HeadPose.LinearAcceleration.x, s.HeadPose.LinearAcceleration.y, s.HeadPose.LinearAcceleration.z);
		}
		math::vector3d GetAngularVelocity()
		{
			ovrTrackingState s = ovrHmd_GetTrackingState(m_device, 0);
			return math::vector3d(s.HeadPose.AngularVelocity.x, s.HeadPose.AngularVelocity.y, s.HeadPose.AngularVelocity.z);
		}
		void GetRenderScaleAndOffset(ovrFovPort fov, const math::vector2di& textureSize, const math::recti& renderVP, math::vector2d& scale, math::vector2d& offset)
		{
			ovrSizei sz;
			sz.w = textureSize.x;
			sz.h = textureSize.y;
			ovrRecti r;
			r.Pos.x = renderVP.ULPoint.x;
			r.Pos.y = renderVP.ULPoint.y;
			r.Size.w = renderVP.getWidth();
			r.Size.h = renderVP.getHeight();
			ovrVector2f so[2];
			ovrHmd_GetRenderScaleAndOffset(fov, sz, r, so);
			scale.set(so[0].x, so[0].y);
			offset.set(so[1].x, so[1].y);
		}

		void ResetOrientation()
		{
			ovrHmd_RecenterPose(m_device);
		}

		bool IsHMDConnected()
		{
			return (ovrHmd_GetTrackingState(m_device, 0).StatusFlags | ovrStatus_HmdConnected) != 0;
		}

		bool IsTrackingConnected()
		{
			return ovrHmd_GetTrackingState(m_device, 0.0f).StatusFlags & ovrStatus_PositionConnected;
		}
		bool IsPositionTracking()
		{
			return ovrHmd_GetTrackingState(m_device, 0.0f).StatusFlags & ovrStatus_PositionTracked;
		}
		bool IsExtendedDesktop()
		{
			return m_device->HmdCaps & ovrHmdCap_ExtendDesktop;
		}
	};

OculusDevice::OculusDevice(ovrHmd device,OculusManager*m)
{
	m_impl=new OculusDeviceImpl(device,this,m);

}

OculusDevice::~OculusDevice()
{
	delete m_impl;
}
ovrHmd OculusDevice::GetDevice()
{
	return m_impl->GetDevice();
}
const OculusDeviceData& OculusDevice::GetDeviceInfo()const
{
	return m_impl->GetDeviceInfo();
}
bool OculusDevice::IsConnected()
{
	return m_impl->GetDevice()!=0;
}

float OculusDevice::GetIPD()
{
	return m_impl->GetIPD() ;
}
float OculusDevice::GetEyeHeight()
{
	return m_impl->GetEyeHeight();
}
math::vector3d OculusDevice::GetCameraPosition()
{
	return m_impl->GetCameraPosition();
}
math::quaternion OculusDevice::GetCameraOrientation()
{
	return m_impl->GetCameraOrientation();
}
math::vector3d OculusDevice::GetPosition()
{
	return m_impl->GetPosition();
}
math::quaternion OculusDevice::GetOrientation()
{
	return m_impl->GetOrientation();
}
void OculusDevice::SetLowPresistenceMode(bool on)
{
	m_impl->SetLowPresistenceMode(on);
}

bool OculusDevice::AttachToWindow(video::RenderWindow* window)
{
	return m_impl->AttachToWindow(window);
}
void OculusDevice::GetDevicePosSize(math::vector2di& pos, math::vector2di& size)
{
	m_impl->GetDevicePosSize(pos, size);
}
void OculusDevice::Update(float dt)
{
	m_impl->Update(dt);
}
math::vector3d OculusDevice::GetAcceleration()
{
	return m_impl->GetAcceleration();
}

math::vector3d OculusDevice::GetAngularVelocity()
{
	return m_impl->GetAngularVelocity();
}

void OculusDevice::ResetOrientation()
{
	m_impl->ResetOrientation();
}
bool OculusDevice::IsHMDConnected()
{
	return m_impl->IsHMDConnected();
}

bool OculusDevice::IsTrackingConnected()
{
	return m_impl->IsTrackingConnected();
}

bool OculusDevice::IsPositionTracking()
{
	return m_impl->IsPositionTracking();
}

bool OculusDevice::IsExtendedDesktop()
{
	return m_impl->IsExtendedDesktop();
}

void OculusDevice::GetRenderScaleAndOffset(ovrFovPort fov, const math::vector2di& textureSize, const math::recti& renderVP, math::vector2d& scale, math::vector2d& offset)
{
	
	return m_impl->GetRenderScaleAndOffset(fov,textureSize,renderVP,scale,offset);
}


}
}

