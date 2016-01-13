#include "stdafx.h"
#include <stdio.h>
#include "movingAverage.h"

MovAvg::MovAvg(int len)
{
	this->len = len;
	cur = 0;
	buf = new double[len];
	for (int i = 0; i< len; i++)
		buf[i] = 0.0;
}

double MovAvg::getNext(double next)
{
	double sum = 0.0;
	
	buf[cur++] = next;
	if (cur>=len) cur = 0;

	for (int i = 0; i< len; i++)
		sum += buf[i];
	
	sum /= (double)len;

	return sum;
}