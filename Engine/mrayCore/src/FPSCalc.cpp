#include "stdafx.h"


#include "FPSCalc.h"

#include "ITimer.h"


namespace mray{

namespace core{

	class MRAY_CORE_DLL FPSCalcImpl
	{

		float m_lastFps;
		float m_lastTime;
		float m_frameTime;
		float m_fps;
		float m_startTime;
		uint m_frames;
		float m_allFrames;
		float m_allFps;

		float m_worstFPS;
		float m_bestFPS;

		bool m_isFixedFPS;

		float m_timeFactor;
	public:

		FPSCalcImpl() :m_lastFps(60), m_fps(99999),
			m_startTime(0), m_frames(60), m_frameTime(0), m_lastTime(0), m_worstFPS(999999), m_bestFPS(0), m_allFrames(0),
			m_allFps(0), m_timeFactor(1), m_isFixedFPS(0)
		{
			m_lastTime = 0;
		}

		void setFixedFPS(float fps, bool set){
			m_isFixedFPS = set;
			if (m_isFixedFPS)
				m_fps = fps;
		}

		void setTimeFactor(float t){
			m_timeFactor = t;
			if (m_timeFactor < 0)
				m_timeFactor = 0;
		}

		float getFPS()const
		{
			return m_fps;
		}

		float dt()const{
			return m_frameTime*m_timeFactor;
		}

		void resetTime(float t){
			m_startTime = m_lastTime = t;
			m_frames = 0;
			m_frameTime = 0;
		}
		void regFrame(float time)
		{

			if (m_isFixedFPS){
				m_frameTime = m_fps*0.001f;
				m_allFps += m_frameTime;
				return;
			}
			m_allFps++;
			m_frames++;
			float dtime = time - m_startTime;

			if (time > m_lastTime)
				m_frameTime = (time - m_lastTime)*0.001f;
			else
				m_frameTime = 0;
			m_allFrames += m_frameTime;

			m_lastTime = time;
			//	if(dtime>1000)
			if (dtime > 1000)//update fps each 1/8 sec (125 ms)
			{
				float f = (float)(m_frames * 1000) / ((float)dtime);
				m_fps = math::lerp<float>(m_lastFps, f, 0.8);
				//	m_frameTime = 1.0f / m_fps;
				if (m_fps > m_bestFPS)
					m_bestFPS = m_fps;
				if (m_fps < m_worstFPS)
					m_worstFPS = m_fps;
				m_lastFps = f;
				m_startTime = time;
				m_frames = 0;
			}

		}



		float getWorstFPS()const{
			return m_worstFPS;
		}
		float getBestFPS()const{
			return m_bestFPS;;
		}
		float getAverageFPS()const{
			return (m_allFps) / (float)m_allFrames;
		}

	};

	FPSCalc::FPSCalc()
	{
		m_impl = new FPSCalcImpl();
	}

	FPSCalc::~FPSCalc()
	{
		delete m_impl;
	}

void FPSCalc::setFixedFPS(float fps,bool set){
	m_impl->setFixedFPS(fps, set);
}

void FPSCalc::setTimeFactor(float t){
	m_impl->setTimeFactor(t);

}

float FPSCalc::getFPS()const
{
	return m_impl->getFPS();
}

float FPSCalc::dt()const{
	return m_impl->dt();
}

void FPSCalc::resetTime(float t){
	m_impl->resetTime(t);
} 
void FPSCalc::regFrame(float time)
{
	m_impl->regFrame(time);
}



float FPSCalc::getWorstFPS()const{
	return m_impl->getWorstFPS();
}
float FPSCalc::getBestFPS()const{
	return m_impl->getBestFPS();;
}
float FPSCalc::getAverageFPS()const{
	return m_impl->getAverageFPS();
}






}
}
