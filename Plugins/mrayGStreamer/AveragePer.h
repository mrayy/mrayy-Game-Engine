

#ifndef __AVERAGEPER__
#define __AVERAGEPER__

#include "IDelegate.h"

namespace mray
{

class AveragePer
{
	uint m_count;
	ulong m_timestamp;

	uint m_average;
	uint m_totalCount;
	uint m_per;
public:

	DelegateEvent1< int> OnSample;

	AveragePer(int per = 1000);
	void Add(uint count);

	void Reset();

	uint GetAverage();
};
}


#endif
