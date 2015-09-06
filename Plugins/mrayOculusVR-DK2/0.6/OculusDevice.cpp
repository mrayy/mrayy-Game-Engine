

#include "stdafx.h"
#include "../SRC/GL/CAPI_GLE.h"
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

	static GLEContext _glContext;
	class OculusDeviceImpl
	{
		ovrHmd m_device;
		OculusDevice* m_owner;
		OculusManager* m_mngr;

		OculusDeviceData m_data;

		unsigned StartTrackingCaps;
		ovrGLTexture* mirrorTexture;
		GLuint mirrorFBO = 0;
		ovrEyeRenderDesc EyeRenderDesc[2];
		ovrPosef         EyeRenderPose[2];
		ovrVector3f      ViewOffset[2];
		ovrTrackingState hmdState;

	public:
		OculusDeviceImpl(ovrHmd device, OculusDevice* o, OculusManager*mngr)
		{
			m_mngr = mngr;
			m_owner = o;
			m_device = device;
			if (!m_device)
				return;

			StartTrackingCaps = 0;

			if (!_glContext.IsInitialized())
				_glContext.Init();

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
			bool DynamicPrediction = true;
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

		void CreateEyeBuffers()
		{
			// Make eye render buffers
			ovrSwapTextureSet* TextureSet[2];
			for (int i = 0; i < 2; i++)
			{
				ovrSizei size = ovrHmd_GetFovTextureSize(m_device, (ovrEyeType)i, m_device->DefaultEyeFov[i], 1);
				if (ovrHmd_CreateSwapTextureSetGL(m_device, GL_SRGB8_ALPHA8, size.w, size.h, &TextureSet[i]) != ovrSuccess)
				{
					ovrErrorInfo err;
					ovr_GetLastErrorInfo(&err);
					printf("Failed to create swap texture! %s\n",err.ErrorString);
				}
			}

			ovrSizei windowSize = { m_device->Resolution.w / 2, m_device->Resolution.h / 2 };
			// Create mirror texture and an FBO used to copy mirror texture to back buffer
			ovrHmd_CreateMirrorTextureGL(m_device, GL_RGBA, windowSize.w, windowSize.h, (ovrTexture**)&mirrorTexture);
			// Configure the mirror read buffer
			mirrorFBO = 0;
			glGenFramebuffers(1, &mirrorFBO);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
			glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
		bool AttachToWindow(video::RenderWindow* window)
		{
			HWND* ptr;
			window->GetCustomParam("WINDOW", (void*)&ptr);

			ovrHmd_SetEnabledCaps(m_device, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

			unsigned sensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection;
			if (!IsTrackingConnected())
				printf("Oculus tracking device not detected!\n");
			sensorCaps |= ovrTrackingCap_Position;

			if (StartTrackingCaps != sensorCaps)
			{
				ovrHmd_ConfigureTracking(m_device, sensorCaps, 0);
				StartTrackingCaps = sensorCaps;
			}

			return true;

		}
		void GetDevicePosSize(math::vector2di& pos, math::vector2di& size)
		{
		}
		const OculusDeviceData& GetDeviceInfo()const
		{
			return m_data;
		}

		void Init()
		{
			if (!m_device)
				return;


			EyeRenderDesc[0] = ovrHmd_GetRenderDesc(m_device, ovrEye_Left, m_device->DefaultEyeFov[0]);
			EyeRenderDesc[1] = ovrHmd_GetRenderDesc(m_device, ovrEye_Right, m_device->DefaultEyeFov[1]);
			CreateEyeBuffers();
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
			// Get eye poses, feeding in correct IPD offset
			ViewOffset[0] = EyeRenderDesc[0].HmdToEyeViewOffset;
			ViewOffset[1]=EyeRenderDesc[1].HmdToEyeViewOffset;
			ovrFrameTiming   ftiming = ovrHmd_GetFrameTiming(m_device, 0);
			hmdState = ovrHmd_GetTrackingState(m_device, ftiming.DisplayMidpointSeconds);
			ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);

		}

		void EndFrame(const ovrTexture* eyes)
		{

			// Do distortion rendering, Present and flush/sync

			// Set up positional data.
			ovrViewScaleDesc viewScaleDesc;
			viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
			viewScaleDesc.HmdToEyeViewOffset[0] = ViewOffset[0];
			viewScaleDesc.HmdToEyeViewOffset[1] = ViewOffset[1];

			ovrLayerEyeFov ld;
			ld.Header.Type = ovrLayerType_EyeFov;
			ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.
			/*
			for (int eye = 0; eye < 2; eye++)
			{
				ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureSet;
				ld.Viewport[eye] = Recti(eyeRenderTexture[eye]->GetSize());
				ld.Fov[eye] = HMD->DefaultEyeFov[eye];
				ld.RenderPose[eye] = EyeRenderPose[eye];
			}

			ovrLayerHeader* layers = &ld.Header;
			ovrResult result = ovrHmd_SubmitFrame(HMD, 0, &viewScaleDesc, &layers, 1);
			isVisible = result == ovrSuccess;*/

			glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			GLint w = mirrorTexture->OGL.Header.TextureSize.w;
			GLint h = mirrorTexture->OGL.Header.TextureSize.h;
			glBlitFramebuffer(0, h, w, 0,
				0, 0, w, h,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			/*
			static ovrPosef eyeRenderPose[2];
			eyeRenderPose[0] = ovrHmd_GetEyePose(m_device, ovrEyeType::ovrEye_Left);
			eyeRenderPose[1] = ovrHmd_GetEyePose(m_device, ovrEyeType::ovrEye_Right);

			ovrHmd_EndFrame(m_device, eyeRenderPose, eyes);*/
		}

		float GetEyeHeight()
		{
			return ovrHmd_GetFloat(m_device, OVR_KEY_EYE_HEIGHT, OVR_DEFAULT_EYE_HEIGHT);
		}

		bool GetEyePos(OVREye e, math::vector3d& pos, math::quaternion& ori)
		{

			ovrPosef &p = EyeRenderPose[(int)e];
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
// 			ovrFrameTiming   ftiming = ovrHmd_GetFrameTiming(m_device, 0);
// 			ovrTrackingState hmdState = ovrHmd_GetTrackingState(m_device, ftiming.DisplayMidpointSeconds);
			return math::vector3d(hmdState.HeadPose.ThePose.Position.x, hmdState.HeadPose.ThePose.Position.y, -hmdState.HeadPose.ThePose.Position.z);

		}
		math::quaternion GetOrientation()
		{
// 			ovrFrameTiming   ftiming = ovrHmd_GetFrameTiming(m_device, 0);
// 			ovrTrackingState hmdState = ovrHmd_GetTrackingState(m_device, ftiming.DisplayMidpointSeconds);
			return math::quaternion(hmdState.HeadPose.ThePose.Orientation.w, hmdState.HeadPose.ThePose.Orientation.x, hmdState.HeadPose.ThePose.Orientation.y, hmdState.HeadPose.ThePose.Orientation.z);

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
			return true;
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

