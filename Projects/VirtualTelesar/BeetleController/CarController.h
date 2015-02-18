

#ifndef _CAR_CONTROLLER_
#define _CAR_CONTROLLER_


#include "IUDPClient.h"

namespace mray
{

	class PositionEstimator
	{
		typedef math::vector3d T;
		std::vector<T> m_samples;
		T m_mean;
		T m_variance;
	public:
		void Reset()
		{
			m_samples.clear();
			m_mean=0;
			m_variance=0;
		}
		void AddSample(const T& v)
		{
			m_samples.push_back(v);
		}
		void Calculate()
		{
			if(m_samples.size()==0)
				return;
			m_mean=m_samples[0];
			for(int i=1;i<m_samples.size();++i)
			{
				m_mean+=m_samples[i];
			}
			m_mean/=(float)m_samples.size();
			m_variance=math::sqr(m_samples[0]-m_mean);
			for(int i=1;i<m_samples.size();++i)
			{
				m_variance+=math::sqr(m_samples[i]-m_mean);
			}
			m_variance/=(float)m_samples.size();
			m_variance.x=sqrt(m_variance.x);
			m_variance.y=sqrt(m_variance.y);
			m_variance.z=sqrt(m_variance.z);
		}
		const T& Mean()const		
		{
			return m_mean;
		}
		const T& Variance()const		
		{
			return m_variance;
		}
	};
	class CarController
	{
	protected:
		math::vector2d m_speedVector;
		math::vector3d m_headRotation;

		PositionEstimator m_calibratedPosition;
		PositionEstimator m_calibratedOrintation;


		network::IUDPClient* m_udpClient;

		enum EMode
		{
			EIdle,
			ECalibrating,
			EControlling
		}m_mode;

		float m_calibrationTime;
		float m_time;
	public:
		CarController();
		~CarController();

		void Init();

		bool IsIdle(){return m_mode==EIdle;}
		bool IsCalibrating(){return m_mode==ECalibrating;}
		bool IsControlling(){return m_mode==EControlling;}

		void Stop(){m_mode=EIdle;}

		void StartCalibrating(float len)
		{
			m_time=0;
			m_calibrationTime=len;
			m_mode=ECalibrating;
			m_calibratedPosition.Reset();
			m_calibratedOrintation.Reset();
		}

		void OnBodyDataReceived(const math::vector3d &pos,const math::quaternion& rot);


		void Update(float dt);
	};
}


#endif
