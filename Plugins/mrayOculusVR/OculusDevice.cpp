

#include "stdafx.h"
#include "OculusDevice.h"
#include "OculusManager.h"

using namespace OVR;

namespace mray
{
namespace video
{


	class OculusDeviceImpl:public MessageHandler
	{
		HMDDevice* m_device;
		SensorDevice* m_sensor;
		SensorFusion m_fusion;
		Util::MagCalibration m_magCalib;
		OculusDevice* m_owner;
		OculusManager* m_mngr;

		OculusDeviceData m_data;

		bool m_magAwaitingForwardLook;
		float m_firstMagYaw ;

#define INITIAL_PREDICTION_TIME 0.05f
#define INITIAL_ACCEL_GAIN 0.05f
	public:
		OculusDeviceImpl(OVR::HMDDevice* device,OculusDevice* o,OculusManager*mngr)
		{
			m_mngr=mngr;
			m_magAwaitingForwardLook=false;
			m_owner=o;
			m_device=device;
			if(!m_device)
				return;
			m_device->GetDeviceInfo(&m_data.deviceInfo);
			m_sensor=m_device->GetSensor();
			if(!m_sensor) 
			{
				gLogManager.log("Failed to detect Oculus HMD Sensor!",ELL_WARNING);
			}else
			{
				m_fusion.AttachToSensor(m_sensor);
				m_fusion.SetDelegateMessageHandler(this);
				m_fusion.SetPredictionEnabled(true);
			}

			// Distortion fit parameters based on if we are using a 5" (Prototype, DK2+) or 7" (DK1) 
			if(m_data.deviceInfo.HScreenSize<0.14) //5.5"
			{
				m_data.distortionFit.x=0;
				m_data.distortionFit.y=1;
				m_data.distortionFitScale =1;
			}else	//7"
			{
				m_data.distortionFit.x=-1;
				m_data.distortionFit.y= 0;

				m_data.distortionFitScale=0.7;
			}

			m_data.lensOffset=CalculatePhysicalLensOffset();

			m_data.predictionTime=0;
			m_data.accelGain=0;

			Init();
		}
		~OculusDeviceImpl()
		{
			//m_mngr->RemoveDevice(m_owner);
		//	m_device->Disconnect();
			if (m_sensor)
				m_sensor->Release();
			if (m_device)
				m_device->Release();
		}

	 	const OculusDeviceData& GetDeviceInfo()const
		{
			return m_data;
		}

		void Init()
		{
			if(!m_device)
				return;
			if(m_data.predictionTime>0)
				m_fusion.SetPrediction(m_data.predictionTime);
			else SetPredictionTime(INITIAL_PREDICTION_TIME);

			if(m_data.accelGain>0)
				m_fusion.SetAccelGain(m_data.accelGain);
			else SetAccelGain(INITIAL_ACCEL_GAIN);
		}

		HMDDevice* GetDevice()const {return m_device;}
		SensorDevice* GetSensor()const{return m_sensor;}
		

		void Update(float dt)
		{
			if(!m_device)
				return;
			if(m_magCalib.IsManuallyCalibrating())
			{
			}else
			{
				m_magCalib.UpdateAutoCalibration(m_fusion);
				if(m_magCalib.IsCalibrated())
				{
					if(m_fusion.IsMagReady())
						m_fusion.SetYawCorrectionEnabled(true);
					Vector3f center=m_fusion.GetCalibratedMagnetometer();
					m_owner->__FIRE_OnMagCalibrated(m_owner,math::vector3d(center.x,center.y,center.z));
				}
				
			}
		}

		void SetPredictionTime(float dt)
		{
			if(dt>0 )
			{
				m_fusion.SetPrediction(dt);
				m_data.predictionTime=dt;
			}
		}
		void SetAccelGain(float g)
		{
			if(g>0)
			{
				m_fusion.SetAccelGain(g);
				m_data.accelGain=g;
			}
		}

		math::quaternion GetOrientation()
		{
			Quatf q= m_fusion.GetPredictedOrientation();
// 			math::vector3d a;
// 			q.GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z, OVR::Rotate_CCW, OVR::Handed_L>(&a.x, &a.y, &a.z);
// 			a.x = math::toDeg(a.x);
// 			a.y = math::toDeg(a.y);
// 			a.z = math::toDeg(a.z);
			return math::quaternion(q.w, -q.x, -q.y, q.z); //math::quaternion(a.x, a.y, -a.z);// 
			/*
			math::vector3d ret;
			m_fusion.GetPredictedOrientation().GetEulerAngles<Axis_X, Axis_Y, Axis_Z>(&ret.x,&ret.y,&ret.z);
			return ret;*/
		}
		math::vector3d GetAcceleration()
		{
			Vector3f v= m_fusion.GetAcceleration();
			return math::vector3d(v.x,v.y,v.z);
		}

		//Used to offset perspective and distortion shift
		math::vector2d CalculatePhysicalLensOffset()
		{
			float halfHSS=m_data.deviceInfo.HScreenSize*0.5;
			float halfLSD=m_data.deviceInfo.LensSeparationDistance*0.5;

			math::vector2d ret;
			ret.x=(((halfHSS-halfLSD)/halfHSS)*2.0f)-1.0f;
			ret.y=(( halfLSD         /halfHSS)*2.0f)-1.0f;
			return ret;
		}

		float CalculateScale(float fitRad)
		{
			if(!m_device)
				return 1;
			float s=fitRad;
			float ssq=s*s;
			float scale=s*(m_data.deviceInfo.DistortionK[0] + m_data.deviceInfo.DistortionK[1]*ssq + m_data.deviceInfo.DistortionK[2]*ssq*ssq + m_data.deviceInfo.DistortionK[3]*ssq*ssq*ssq );
			return scale/fitRad;
		}
		float GetDistortionScale(const math::vector2d& screenSize)
		{
			if(!m_device)
				return 1;
			float ds=0;
			if(fabs(m_data.distortionFit.x)<0.00001f && fabs(m_data.distortionFit.y)<0.00001f )
				ds=1;
			else
			{
				float stereoAspect= 0.5*screenSize.x/screenSize.y;
				float dx = m_data.distortionFit.x*m_data.distortionFitScale - m_data.lensOffset.x;
				float dy = m_data.distortionFit.y*m_data.distortionFitScale/stereoAspect;
				float fitRadius=sqrt(dx*dx+dy*dy);
				ds=CalculateScale(fitRadius);
			}
			if(ds!=0)
				return ds;
			
			return 1;
		}
		float GetVerticalFOV(const math::vector2d& screenSize)
		{
			if(!m_device)
				return 90;
			float percievedHalfScreenDistance=(m_data.deviceInfo.VScreenSize/2)*GetDistortionScale(screenSize);
			float VFov=math::toDeg(2*atan2(percievedHalfScreenDistance,m_data.deviceInfo.EyeToScreenDistance));
			return VFov;
		}
		float GetAspectRatio()
		{
			if(!m_device)
				return 1;
			return m_data.deviceInfo.HResolution*0.5f/m_data.deviceInfo.VResolution;
		}

		void ResetOrientation()
		{
			m_fusion.Reset();
		}


		void UpdateManualMagCalibration() 
		{
			float tyaw,yaw, pitch, roll;
			Quatf hmdOrient = m_fusion.GetOrientation();
			// Note that yaw and pitch are used from two different Euler angle combinations.  This
			// is done so that pitch (looking "up" or "down") is not dependent on yaw angle
			hmdOrient.GetEulerAngles<Axis_X, Axis_Z, Axis_Y>(&pitch, &roll, &yaw);
			hmdOrient.GetEulerAngles<Axis_Y, Axis_Z, Axis_X>(&tyaw, &roll, &pitch);
			Vector3f mag = m_fusion.GetMagnetometer();
			float dtr = Math<float>::DegreeToRadFactor;

			switch(m_magCalib.NumberOfSamples())
			{
			case 0:
				if (m_magAwaitingForwardLook)
					m_owner->__FIRE_OnMagCalibrated(m_owner,MagCalib_LookFrontWaiting);
				else
					if (fabs(pitch) < 10.0f*dtr)
					{
						m_magCalib.InsertIfAcceptable(hmdOrient, mag);
						m_firstMagYaw = yaw;
						m_magAwaitingForwardLook = false;
						m_fusion.SetMagReference();
					}
					break;
			case 1:
				m_owner->__FIRE_OnMagCalibrated(m_owner,MagCalib_LookUp);
				yaw -= m_firstMagYaw ;
				if (yaw < -Math<float>::Pi)
					yaw += Math<float>::TwoPi;
				if (yaw > Math<float>::Pi)
					yaw -= Math<float>::TwoPi;
				if ((pitch > 50.0f*dtr) && (fabs(yaw) < 20.0f*dtr))
					m_magCalib.InsertIfAcceptable(hmdOrient, mag);
				break;
			case 2:
				m_owner->__FIRE_OnMagCalibrated(m_owner,MagCalib_LookLeft);
				yaw -= m_firstMagYaw ;
				if (yaw < -Math<float>::Pi)
					yaw += Math<float>::TwoPi;
				if (yaw > Math<float>::Pi)
					yaw -= Math<float>::TwoPi;
				if (yaw > 60.0f*dtr) 
					m_magCalib.InsertIfAcceptable(hmdOrient, mag);
				break;
			case 3:
				m_owner->__FIRE_OnMagCalibrated(m_owner,MagCalib_LookRight);
				yaw -= m_firstMagYaw ;
				if (yaw < -Math<float>::Pi)
					yaw += Math<float>::TwoPi;
				if (yaw > Math<float>::Pi)
					yaw -= Math<float>::TwoPi;
				if (yaw < -60.0f*dtr)
					m_magCalib.InsertIfAcceptable(hmdOrient, mag);
				break;
			case 4:
				if (!m_magCalib.IsCalibrated()) 
				{
					m_magCalib.SetCalibration(m_fusion);
					if (m_fusion.IsMagReady())
						m_fusion.SetYawCorrectionEnabled(true);
					Vector3f mc = m_magCalib.GetMagCenter();
					m_owner->__FIRE_OnMagCalibrated(m_owner,math::vector3d(mc.x,mc.y,mc.z));
				}
			}
		}

		void StartMagManualCalibration()
		{
			m_magAwaitingForwardLook=true;
			m_magCalib.BeginManualCalibration(m_fusion);
		}
		void SetMagAutoCalibration()
		{
			m_magCalib.BeginAutoCalibration(m_fusion);
		}
	};

OculusDevice::OculusDevice(OVR::HMDDevice* device,OculusManager*m)
{
	m_impl=new OculusDeviceImpl(device,this,m);

}

OculusDevice::~OculusDevice()
{
	delete m_impl;
}
OVR::HMDDevice* OculusDevice::GetDevice()
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


void OculusDevice::Update(float dt)
{
	m_impl->Update(dt);
}
math::quaternion OculusDevice::GetOrientation()
{
	return m_impl->GetOrientation();
}
math::vector3d OculusDevice::GetAcceleration()
{
	return m_impl->GetAcceleration();
}

void OculusDevice::StartMagManualCalibration()
{
	m_impl->StartMagManualCalibration();
}

void OculusDevice::SetMagAutoCalibration()
{
	m_impl->SetMagAutoCalibration();
}
float OculusDevice::GetVerticalFOV(const math::vector2d& screenSize)
{
	return m_impl->GetVerticalFOV(screenSize);
}
float OculusDevice::GetDistortionScale(const math::vector2d& screenSize)
{
	return m_impl->GetDistortionScale(screenSize);

}

float OculusDevice::GetAspectRatio()
{
	return m_impl->GetAspectRatio();
}

void OculusDevice::ResetOrientation()
{
	m_impl->ResetOrientation();
}

}
}

