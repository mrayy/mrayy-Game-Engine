

#ifndef __MODULESHAREDMEMORY__
#define __MODULESHAREDMEMORY__

#include "NetAddress.h"

namespace mray
{
namespace TBee
{

struct ModuleSharedMemory
{
public:
	bool UserConnected;	//indicate if the user being connected or not
	network::NetAddress userIP;	//target user ip
	int userCommPort;	//direct communication port with the user
	int hostPort;	//port for the host service to communicate with

};

}
}

#endif
