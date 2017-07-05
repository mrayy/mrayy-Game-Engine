

#ifndef __BENCHMARKS__
#define __BENCHMARKS__


namespace mray
{
	class IBenchmark
	{
	public:
		virtual~IBenchmark(){}
		virtual void init(){}
		virtual double getCurrentValue(){ return 0; }
	};

class Benchmarks
{
protected:
public:
	Benchmarks();
	~Benchmarks();

	IBenchmark* CPUCurrentUsage;
	IBenchmark* CPUProcessUsage;
	IBenchmark* VirtualMemoryUsage;
	IBenchmark* PhysicalMemoryUsage;
};

}


#endif