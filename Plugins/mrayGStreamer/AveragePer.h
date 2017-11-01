

#ifndef __AVERAGEPER__
#define __AVERAGEPER__

#include "IDelegate.h"

namespace mray
{

class AveragePer
{
	uint m_count;
	ulong m_timestamp;

	float m_average;
	uint m_totalCount;
	uint m_per;
	uint m_samples;
public:

	DelegateEvent1< int> OnSample;

	AveragePer(int per = 1000);
	void Add(uint count);

	void Init(int per)
	{
		m_per = per;
		Reset();
	}
	void Reset();

	float GetAverage();

	void Update();
};
}


#endif
