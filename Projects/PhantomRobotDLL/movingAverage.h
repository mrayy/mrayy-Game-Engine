#include <stdio.h>


// 10 is good enough
#define _MOVING_AVERAGE_BUF_LENGTH_ 1

//** moving average **//
class MovAvg
{
public:
	MovAvg();
	double getNext(double next);
private:
	int cur;
	double buf[_MOVING_AVERAGE_BUF_LENGTH_];
};
