

/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   23:15
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\PIDController.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	PIDController
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PIDController___
#define ___PIDController___



namespace mray
{
namespace VT
{
	template <class T,class Val>
class PIDController
{
protected:

	float m_kp;
	float m_ki;
	float m_kd;

	T m_lastError;
	T m_integral;
public:
	PIDController(float kp,float ki,float kd):m_kp(kp),m_ki(ki),m_kd(kd),m_lastError(Val::Zero),m_integral(Val::Zero)
	{}
	virtual~PIDController(){}

	void SetPropetionalGain(float kp){m_kp=kp;}
	float GetPropetionalGain(){return m_kp;}

	void SetIntegralGain(float ki){m_ki=ki;}
	float GetIntegralGain(){return m_ki;}

	void SetDifferentialGain(float kd){m_kd=kd;}
	float GetDifferentialGain(){return m_kd;}

	void Reset()
	{
		m_lastError=Val::Zero;
		m_integral=Val::Zero;
	}
	float Update(float dt,const T& currentVal,const T& desiredValue)
	{
		T error=desiredValue-currentVal;
		T diff=(error-m_lastError)/dt;
		m_lastError=error;
		m_integral+=error*dt;
		return -(error*m_kp+m_integral*m_ki+diff*m_kd);
	}
	
};

class FloatValue
{
public:
	static const float Zero;
};

class Vector3Value
{
public:
	static const math::vector3d Zero;
};

class QuaternionValue
{
public:
	static const math::quaternion Zero;
};


typedef PIDController<float,FloatValue> PIDControllerF;
typedef PIDController<math::vector3d,Vector3Value> PIDControllerV3;
typedef PIDController<math::quaternion,QuaternionValue> PIDControllerQ;

}
}

#endif
