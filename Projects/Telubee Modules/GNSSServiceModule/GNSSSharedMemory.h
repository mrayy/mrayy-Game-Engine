
#ifndef __GNSSSHAREDMEMORY__
#define __GNSSSHAREDMEMORY__


namespace mray
{
namespace TBee
{
class GNSSSharedMemory
{
protected:
public:
	GNSSSharedMemory()
	{
		latitude = 0;
		longitude = 0;
	}
	~GNSSSharedMemory(){}

public:
	double latitude;
	double longitude;
};

}
}

#endif

