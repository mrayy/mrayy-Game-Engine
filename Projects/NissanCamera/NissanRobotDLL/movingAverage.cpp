#include "stdafx.h"
#include <stdio.h>
#include "movingAverage.h"

MovAvg::MovAvg(int count)
{
	cur = 0;
	_count = count;
	buf = new double[count];
	for (int i = 0; i< count; i++)
		buf[i] = 0.0;
}
MovAvg::~MovAvg()
{
	delete[]buf;
}
double MovAvg::getNext(double next)
{
	double sum = 0.0;
	
	buf[cur++] = next;
	if (cur>_count) cur = 0;

	for (int i = 0; i< _count; i++)
		sum += buf[i];
	
	sum /= _count;

	return sum;
}