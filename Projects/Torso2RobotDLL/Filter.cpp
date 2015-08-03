#include "stdafx.h"
#include "Filter.h"
#include <math.h>

// ローパスフィルタカットオフ周波数セット
void GeneralFilter::set_lpfreq(float _lpcutoff_freq)
{
	lpcutoff_freq = _lpcutoff_freq;
	// 同時に過去のデータ格納変数の初期化
	n_old_x1		= 0;
	n_old_x1_dot	= 0;
	n_old_time		= 0;
	A_old_x1		= 0;
	A_old_x1_dot	= 0;
	A_old_time		= 0;
	V_old_x1		= 0;
	V_old_time		= 0;

}

void GeneralFilter::clearparameter(void)
{
	n_old_x1		= 0;
	n_old_x1_dot	= 0;
	n_old_time		= 0;
	A_old_x1		= 0;
	A_old_x1_dot	= 0;
	A_old_time		= 0;
	V_old_x1		= 0;
	V_old_time		= 0;
}

double GeneralFilter::lpfilter_normal(double u, __int64 time)
{
	double y, t;
	double expft;

	double tmp_x,tmp_x_dot;

	t = (double)(time - n_old_time)/1000000;

	expft = exp(-lpcutoff_freq*t);

	tmp_x = (lpcutoff_freq*t*expft + expft)*n_old_x1 + t*expft*n_old_x1_dot 
			+ (-1.0/lpcutoff_freq*t*expft - 1.0/lpcutoff_freq/lpcutoff_freq*expft 
			+ 1.0/lpcutoff_freq/lpcutoff_freq)*u;

	tmp_x_dot = -lpcutoff_freq*lpcutoff_freq*t*expft*n_old_x1 + (-lpcutoff_freq*t*expft
			+ expft)*n_old_x1_dot + t*expft*u;
	if(n_old_time == 0.0){
		n_old_x1 = tmp_x;
		n_old_x1_dot = tmp_x_dot;
	}

	y = lpcutoff_freq*lpcutoff_freq*n_old_x1;

	n_old_x1 = tmp_x;
	n_old_x1_dot = tmp_x_dot;
	n_old_time = time;

	return y;
}

double GeneralFilter::lpfilter_genA(double u, __int64 time)
{
	double y, t;
	double expft;

	double tmp_x,tmp_x_dot;

	t = (double)(time - A_old_time)/1000000;

	expft = exp(-lpcutoff_freq*t);

	tmp_x = (lpcutoff_freq*t*expft + expft)*A_old_x1 + t*expft*A_old_x1_dot 
			+ (-1.0/lpcutoff_freq*t*expft - 1.0/lpcutoff_freq/lpcutoff_freq*expft 
			+ 1.0/lpcutoff_freq/lpcutoff_freq)*u;

	tmp_x_dot = -lpcutoff_freq*lpcutoff_freq*t*expft*A_old_x1 + (-lpcutoff_freq*t*expft
			+ expft)*A_old_x1_dot + t*expft*u;
	if(A_old_time == 0.0){
		A_old_x1 = tmp_x;
		A_old_x1_dot = tmp_x_dot;
	}
	y = lpcutoff_freq*lpcutoff_freq*(u - (lpcutoff_freq*lpcutoff_freq*A_old_x1 + 2.0*lpcutoff_freq*A_old_x1_dot));

	A_old_x1 = tmp_x;
	A_old_x1_dot = tmp_x_dot;
	A_old_time = time;

	return y;
}

double GeneralFilter::lpfilter_genV(double u, __int64 time)
{
	double y, t;
	double expft;

	double tmp_x;

	t = (double)(time - V_old_time)/1000000;

	expft = exp(-lpcutoff_freq*t);

	tmp_x = expft*V_old_x1 + (-1.0/lpcutoff_freq*expft + 1.0/lpcutoff_freq)*u;
	if(V_old_time == 0.0) V_old_x1 = tmp_x;
	y = -lpcutoff_freq*lpcutoff_freq*V_old_x1 + lpcutoff_freq*u;

	V_old_x1 = tmp_x;
	V_old_time = time;

	return y;
}