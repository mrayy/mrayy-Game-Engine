#include "stdafx.h"
#include <stdio.h>
#include "movingAverage.h"

MovAvg::MovAvg()
{
	cur = 0;
	for(int i = 0;i< _MOVING_AVERAGE_BUF_LENGTH_;i++)
		buf[i] = 0.0;
}

double MovAvg::getNext(double next)
{
	double sum = 0.0;
	
	buf[cur++] = next;
	if(cur>_MOVING_AVERAGE_BUF_LENGTH_) cur = 0;

	for(int i = 0;i< _MOVING_AVERAGE_BUF_LENGTH_;i++)
		sum += buf[i];
	
	sum /= _MOVING_AVERAGE_BUF_LENGTH_;

	return sum;
}