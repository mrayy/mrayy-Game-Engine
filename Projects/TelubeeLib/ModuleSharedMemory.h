

#ifndef __MODULESHAREDMEMORY__
#define __MODULESHAREDMEMORY__

#include "NetAddress.h"
#include "RobotCommunicator.h"

namespace mray
{
namespace TBee
{

struct ModuleSharedMemory
{
public:
	bool UserConnected;	//indicate if the user being connected or not
	TBee::UserConnectionData userConnectionData;
	int userCommPort;	//direct communication port with the user
	network::NetAddress hostAddress;	//host service address to communicate with

	//bool IsStarted; //indicate if the services should run or not

};

}
}

#endif
