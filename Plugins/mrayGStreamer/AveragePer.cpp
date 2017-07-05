

#include "stdafx.h"
#include "AveragePer.h"


#include "FPSCalc.h"
#include "Engine.h"
#include "ITimer.h"

namespace mray
{
AveragePer::AveragePer(int per){
	m_per = per;
	Reset();
}
void AveragePer::Add(uint count)
{
	m_count += count;
	m_totalCount += count;

	ulong ts = gEngine.getTimer()->getMilliseconds();
	if (ts - m_timestamp > m_per){
		m_timestamp = ts;
		m_average = m_count;
		m_count = 0;
		OnSample(m_average);
	}
}

void AveragePer::Reset()
{
	m_count = 0;
	m_totalCount = 0;
	m_average = 0;
	m_timestamp = gEngine.getTimer()->getMilliseconds();
}

uint AveragePer::GetAverage(){
	return m_average;
}
}