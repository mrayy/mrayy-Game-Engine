#ifndef _PID_H_
#define _PID_H_

class PIDImpl;
class PID
{
public:
	// Kp -  proportional gain
	// Ki -  Integral gain
	// Kd -  derivative gain
	// max - maximum value of manipulated variable
	// min - minimum value of manipulated variable
	PID();
	PID(double min, double max, double Kp, double Kd, double Ki);

	void SetKp(double kp);
	void SetKi(double ki);
	void SetKd(double kd);

	// Returns the manipulated variable given a setpoint and current process value
	double calculate(double dt, double setpoint, double pv);
	void reset();
	~PID();

private:
	PIDImpl *pimpl;
};

#endif