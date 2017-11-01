

#include "stdafx.h"
#include "AveragePer.h"
#include "GStreamerCore.h"

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
	++m_samples;
}

void AveragePer::Update()
{
	ulong ts = gGStreamerCore->GetTimer()->getMilliseconds();
	if (ts - m_timestamp > m_per){
		m_timestamp = ts;
		m_average = (float)m_count;// / (float)m_samples;
		m_samples = 0;
		m_count = 0;
		OnSample(m_average);
	}
}

void AveragePer::Reset()
{
	m_count = 0;
	m_totalCount = 0;
	m_average = 0;
	m_timestamp = gGStreamerCore->GetTimer()->getMilliseconds();
}

float AveragePer::GetAverage(){
	return m_average;
}
}