

#ifndef __PIDCONTROLLER__
#define __PIDCONTROLLER__


namespace mray
{
class PIDController
{
protected:
	float _kp, _ki, _kd;

	float _errI;
	float _errD;
	float _maxErr;
public:
	PIDController(float kp, float ki, float kd, float maxErr) :_kp(kp), _ki(ki), _kd(kd), _errI(0), _errD(0), _maxErr(maxErr)
	{}
	~PIDController(){}

	float SetValue(float current, float target)
	{
		float p, i, d;


		float err = -(current - target);

		p = err*_kp;

		_errI += err;
		if (_errI > _maxErr)
			_errI = _maxErr;
		if (_errI < -_maxErr)
			_errI = -_maxErr;

		i = _errI*_kd;

		d = _kd*(_errD - err);
		_errD = err;

		return p + i + d;
	}

	void Reset()
	{
		_errI = 0;
		_errD = 0;
	}

};

}


#endif
