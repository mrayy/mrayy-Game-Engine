

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "pid.h"

using namespace std;

class PIDImpl
{
public:
	PIDImpl(  double min, double max, double Kp, double Kd, double Ki);
	~PIDImpl();
	double calculate(double dt, double setpoint, double pv);

	double _max;
	double _min;
	double _Kp;
	double _Kd;
	double _Ki;
	double _pre_error;
	double _integral;
};


PID::PID( double min, double max, double Kp, double Kd, double Ki)
{
	pimpl = new PIDImpl(min,max, Kp, Kd, Ki);
}

PID::PID()
{
	pimpl = new PIDImpl(-180, 180, 1, 0, 0);
}


void PID::SetKp(double kp)
{
	pimpl->_Kp = kp;
}
void PID::SetKi(double ki)
{
	pimpl->_Ki = ki;

}
void PID::SetKd(double kd)
{
	pimpl->_Kd = kd;

}

double PID::calculate(double dt, double setpoint, double pv)
{
	return pimpl->calculate(dt,setpoint, pv);
}
void PID::reset()
{
	pimpl->_integral = 0;
	pimpl->_pre_error = 0;
}
PID::~PID()
{
	delete pimpl;
}


/**
 * Implementation
 */
PIDImpl::PIDImpl( double min, double max, double Kp, double Kd, double Ki) :
	
	_max(max),
	_min(min),
	_Kp(Kp),
	_Kd(Kd),
	_Ki(Ki),
	_pre_error(0),
	_integral(0)
{
}

double PIDImpl::calculate(double _dt,double setpoint, double pv)
{
	if (_dt < 0.0001)_dt = 0.0001;
	if (_dt > 0.02)_dt = 0.02;


	// Calculate error
	double error = setpoint - pv;

	// Proportional term
	double Pout = _Kp * error;

	// Integral term
	_integral += error * _dt;
	double Iout = _Ki * _integral;

	// Derivative term
	double derivative = (error - _pre_error) / _dt;
	double Dout = _Kd * derivative;

	// Calculate total output
	double output = Pout + Iout + Dout;

	// Restrict to max/min
	if (output > _max)
		output = _max;
	else if (output < _min)
		output = _min;

	// Save error to previous error
	_pre_error = error;

	return output;
}

PIDImpl::~PIDImpl()
{
}

