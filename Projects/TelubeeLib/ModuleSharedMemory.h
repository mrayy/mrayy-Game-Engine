

#ifndef __MODULESHAREDMEMORY__
#define __MODULESHAREDMEMORY__

#include "NetAddress.h"
#include "RobotCommunicator.h"
#include "IRobotController.h"
#if 0
#include <pthread.h>
#endif

namespace mray
{
namespace TBee
{

	class ModuleSharedMemoryBase
	{
		//Synchronization objects
#if 0
		pthread_mutex_t ipc_mutex;
		pthread_cond_t ipc_condvar;
#endif

	public:
		
		static void InitMaster(ModuleSharedMemoryBase *m)
		{
#if 0
			m->ipc_mutex = pthread_mutex_t();
			m->ipc_condvar = pthread_cond_t();

			//init sync objects
			pthread_mutexattr_t mutex_attr;
			pthread_mutexattr_init(&mutex_attr);
			if (pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED) != 0)
			{
				printf("Failed to set shared memory attribute! \n");
			}
			int err = pthread_mutex_init(&m->ipc_mutex, &mutex_attr);
			if(err!= 0)
			{
			
				printf("Failed to create shared memory mutex! %s \n", strerror(err));
			}

			pthread_condattr_t cond_attr;
			pthread_condattr_init(&cond_attr);
			pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
			pthread_cond_init(&m->ipc_condvar, &cond_attr);
#endif

		}

		static void DestroyMaster(ModuleSharedMemoryBase *m)
		{
#if 0
			pthread_mutex_destroy(&m->ipc_mutex);
			pthread_cond_destroy(&m->ipc_condvar);
#endif
		}

		static void Lock(ModuleSharedMemoryBase *m)
		{
#if 0
			printf("Locking\n");
			pthread_mutex_lock(&m->ipc_mutex);
#endif
		}

		static void Unlock(ModuleSharedMemoryBase *m)
		{
#if 0
			printf("Unlocking\n");
			pthread_mutex_unlock(&m->ipc_mutex);
#endif

		}
	};

	class SharedMemoryLock
	{
		ModuleSharedMemoryBase *_m;
	public:
		SharedMemoryLock(ModuleSharedMemoryBase *m)
		{
			_m = m;
			ModuleSharedMemoryBase::Lock(_m);
		}
		~SharedMemoryLock()
		{
			ModuleSharedMemoryBase::Unlock(_m);
		}
	};

class ModuleSharedMemory:public ModuleSharedMemoryBase
{
public:
	ModuleSharedMemory()
	{
		dataRate = 0;
	}
	//Shared Data
	bool UserConnected;	//indicate if the user being connected or not
	TBee::UserConnectionData userConnectionData;
	int userCommPort;	//direct communication port with the user
	int   dataRate;
	network::NetAddress hostAddress;	//host service address to communicate with

	//bool IsStarted; //indicate if the services should run or not
	RobotStatus robotData;	//robot control data
};

}
}

#endif
