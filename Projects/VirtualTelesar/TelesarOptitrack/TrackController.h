

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
	class TrackController
	{
	protected:
		math::vector3d m_pos;
		/*math::quaternion m_oriInv;
		math::quaternion m_ori;
		math::vector3d m_initRot;
		*/
		math::matrix4x4 m_homeMat;

		math::vector3d m_lastPos;
		math::quaternion m_lastOri;

		bool m_calib;
		core::string m_name;
		int m_id;
	public:
		TrackController();
		~TrackController();

		void Init(int id,const core::string& name);
		void reset();


		void OnBodyDataReceived(int id,const math::vector3d &pos,const math::quaternion& rot);


		void Send();
		void Update(float dt);
	};
}


#endif
