

/********************************************************************
	created:	2010/09/01
	created:	1:9:2010   22:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ValueController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ValueController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ValueController_h__
#define ValueController_h__

#include "GCPtr.h"

namespace mray
{

template <class T>
class ControllerValue
{
public:
	virtual ~ControllerValue(){}
	virtual void SetValue(const T &v)=0;
	virtual T GetValue()=0;
};

template <class T>
class ControllerFunction
{
protected:
	bool m_isDeltaIntegrate;
	T m_deltaIntegrate;

	T _getAdjustedInput(const T &v)
	{
		if(!m_isDeltaIntegrate)
			return v;
		m_deltaIntegrate+=v;
		return m_deltaIntegrate;
	}
public:
	ControllerFunction(bool deltaIntegrate)
	{
		m_isDeltaIntegrate=deltaIntegrate;
		m_deltaIntegrate=0;
	}
	virtual ~ControllerFunction()
	{
	}
	virtual T CalculateValue(const T &input)=0;
};



template <class T>
class ValueController
{
private:
protected:
	GCPtr<ControllerValue<T>> m_src;
	GCPtr<ControllerValue<T>> m_dst;
	GCPtr<ControllerFunction<T>> m_function;
	bool m_enabled;
public:
	ValueController(const GCPtr<ControllerValue<T>>& src,const GCPtr<ControllerValue<T>>& dst,const GCPtr<ControllerFunction<T>>& f):
	  m_src(src),m_dst(dst),m_function(f),m_enabled(true)
	{
	}
	virtual ~ValueController()
	{
		delete m_dst;
	}

	void SetSource(const GCPtr<ControllerValue<T>>& src)
	{
		m_src=src;
	}
	void SetDestination(const GCPtr<ControllerValue<T>>& dst)
	{
		m_dst=dst;
	}
	void SetFunction(const GCPtr<ControllerFunction<T>>&  f)
	{
		m_function=f;
	}

	const GCPtr<ControllerValue<T>>& GetSource( )
	{
		return m_src;
	}
	const GCPtr<ControllerValue<T>>& GetDestination( )
	{
		return m_dst;
	}
	const GCPtr<ControllerFunction<T>>& GetFunction( )
	{
		return m_function;
	}

	void Enable(bool e)
	{
		m_enabled=e;
	}
	bool IsEnabled()
	{
		return m_enabled;
	}

	void Update()
	{
		if(m_enabled)
			m_dst->SetValue(m_function->Calculate(m_src->GetValue()));
	}
	
};

}
#endif // ValueController_h__
