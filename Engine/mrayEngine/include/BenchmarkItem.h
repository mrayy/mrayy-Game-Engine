

#ifndef ___BenchmarkItem___
#define ___BenchmarkItem___

#include "mString.h"
#include "GCPtr.h"
#include "CompileConfig.h"

namespace mray
{

class MRAY_DLL BenchmarkItem
{
public:
	typedef std::list<BenchmarkItem*> BenchmarkItemList;
protected:	
	core::string m_name;

	float m_totalTime;
	int m_samples;

	float m_enterTime;
	bool m_entered;
	float m_lastTime;

	BenchmarkItemList m_subItems;

public:

	BenchmarkItem(const core::string&name);
	virtual~BenchmarkItem();

	void reset();

	const core::string& getName()const;
	int getSamplesCount()const;
	float getTotalTime()const;
	float getAverageTime()const;
	float getLastTime()const;

	const BenchmarkItemList& getSubItems()const;

	void OnEnter();
	float OnExit();

	BenchmarkItem* getSubItem(const core::string&name);
	BenchmarkItem* addSubItem(const core::string&name);
	void addSubItem(BenchmarkItem* item);

	void removeSubItem(BenchmarkItem* item);
};

class BenchmarkHelper
{
	BenchmarkItem* m_item;
public:
	BenchmarkHelper(BenchmarkItem*i):m_item(i)
	{
		m_item->OnEnter();
	}
	~BenchmarkHelper(){m_item->OnExit();}
};

#ifdef _NO_AUTO_BENCHMARKING__
#define _AUTO_BENCHMARK(item)
#else
#define _AUTO_BENCHMARK(item)mray::BenchmarkHelper __benchmark_auto(item);
#endif
}



#endif



