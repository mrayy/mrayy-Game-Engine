#include <stdio.h>


// 10 is good enough

//** moving average **//
class MovAvg
{
public:
	MovAvg(int len);
	~MovAvg(){ delete[]buf; }
	double getNext(double next);
private:
	int cur;
	double *buf;
	int len;
};
