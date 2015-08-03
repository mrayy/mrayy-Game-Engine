#ifndef __MOVINGAVERAGE__
#define __MOVINGAVERAGE__
#include <stdio.h>


// 10 is good enough

//** moving average **//
class MovAvg
{
public:
	MovAvg(int samples);
	~MovAvg();
	double getNext(double next);
private:
	int m_samples;
	int cur;
	double* buf;
};


#endif