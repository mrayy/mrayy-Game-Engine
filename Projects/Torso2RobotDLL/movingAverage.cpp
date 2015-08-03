#include "stdafx.h"
#include <stdio.h>
#include "movingAverage.h"

MovAvg::MovAvg(int samples)
{
	m_samples = samples;
	buf = new double[m_samples];
	cur = 0;
	for (int i = 0; i< samples; i++)
		buf[i] = 0.0;
}

MovAvg::~MovAvg()
{
	delete[] buf;
}

double MovAvg::getNext(double next)
{
	double sum = 0.0;
	
	buf[cur++] = next;
	if (cur>m_samples) cur = 0;

	for (int i = 0; i< m_samples; i++)
		sum += buf[i];
	
	sum /= m_samples;

	return sum;
}