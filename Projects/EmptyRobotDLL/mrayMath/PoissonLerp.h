

#ifndef __POISSONLERP__
#define __POISSONLERP__

#include "ILerpFunction.h"


namespace mray
{
namespace math
{
	
	template<class T,int Iterations=5>
class PoissonLerp:public ILerpFunction<T>
{
protected:
	static const _NumIterations = Iterations;

	T m_values[_NumIterations];
	T m_goal;
	float m_targetFrameRate;
	float m_smoothFactor;
public:
	PoissonLerp(const T& initial,float smoothFactor=0.8f,float targetFPS=100.0f)
	{
		m_targetFrameRate = targetFPS;
		m_smoothFactor = smoothFactor;
		m_goal = initial;
		SetInitialValue(initial);
	}
	virtual ~PoissonLerp(){}

	virtual void SetInitialValue(const T& v)
	{
		for (int i = 0; i < _NumIterations; ++i)
			m_values[i] = v;
	}
	virtual void SetGoalValue(const T& v)
	{
		m_goal = v;
	}

	virtual const T& GetValue()const
	{
		return m_values[_NumIterations - 1];
	}

	virtual bool IsDone(float err = 0.001f)
	{
		return fabs(m_goal - m_values[_NumIterations - 1]) <= err;
	}

	virtual const T& Update(float dt)
	{
		float dtExp = dt*m_targetFrameRate;
		float smooth = std::pow(m_smoothFactor, dtExp);
		smooth = math::clamp<float>(smooth, 0.0f, 1.0f);
		for (int i = 0; i < _NumIterations; ++i)
		{
			const T& p = (i == 0) ? m_goal : m_values[i - 1];
			m_values[i] = smooth*m_values[i] + (1.0f - smooth)*p;
		}
		return m_values[_NumIterations - 1];
	}
};

}
}

#endif
