
#ifndef __SERVICELOADER__
#define __SERVICELOADER__

#include "shmem.h"
#include "ModuleSharedMemory.h"

namespace mray
{
	
class ServiceLoader
{
protected:
	TBee::ModuleSharedMemory m_memory;

	shmem m_sharedMemory;

	void _init(int argc, _TCHAR* argv[]);
	void _destroy();
public:
	ServiceLoader(int argc, _TCHAR* argv[]);
	virtual ~ServiceLoader();

	void Run();


};

}


#endif
