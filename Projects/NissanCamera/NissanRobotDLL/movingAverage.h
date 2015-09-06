#include <stdio.h>


// 10 is good enough
#define _MOVING_AVERAGE_BUF_LENGTH_ 15

//** moving average **//
class MovAvg
{
public:
	MovAvg(int count = _MOVING_AVERAGE_BUF_LENGTH_);
	~MovAvg();
	double getNext(double next);
private:
	int cur;
	int _count;
	double *buf;
};
